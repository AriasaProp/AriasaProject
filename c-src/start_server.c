#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
// external library
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <ctype.h>

#include "page_gen.h"
#include "string_util.h"
#include "common.h"
#include "log.h"

// max text memorize
// max threads run on background to serv client
#define NUM_WORKERS 16

static SSL_CTX *sslctx;
static bool alive;
static int pipe_fifo[2] = {0}, active_workers;
static pthread_mutex_t pmtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t pcond = PTHREAD_COND_INITIALIZER;

static void handling_signal(int UNUSED_ARG(s)) {
  LOGI("Ctrl + C pressed ...");
  alive = false;
}
static void *serving_client(void *); // workers thread

int main(int UNUSED_ARG(argc), char **UNUSED_ARG(argv)) {
#define MAX_MAIN_BUFFER 512
  char temps[MAX_MAIN_BUFFER];
  LOGI("Welcome to "PROJECT_NAME" v"SEM_VER);
  LOGI("Hostname: "SERVER_HOSTNAME ", Port: " STRINGIFY(SERVER_PORT));
  alive = true;
  active_workers = 0;
  int server_sock, maxfd, t, ret = EXIT_FAILURE;
  fd_set mset, dset;
  SSL *ssl;
  pthread_t workers[NUM_WORKERS];
  (void) signal(SIGINT, handling_signal);
  // init ssl
  IS_ERROR(!OPENSSL_init_ssl (OPENSSL_INIT_LOAD_SSL_STRINGS, 0)) {
    LOGE("Fail to initialize OpenSSL SSL");
    goto main_end;
  }
  SSL_library_init();
  OpenSSL_add_ssl_algorithms();
  IS_ERROR(!BIO_get_ktls_send()) {
    LOGE("BIO not support Kernel TLS-path send");
    goto main_ssl;
  }
  TODO("Do we need kernel level for receive data from client?");
  // IS_ERROR(!BIO_get_ktls_recv()) {
  //   LOGE("BIO not support Kernel TLS-path recv");
  //   goto main_ssl;
  // }
  // init ssl ctx
  IS_ERROR(!(sslctx = SSL_CTX_new(TLS_server_method ()))) {
    LOGE("Failed to create ssl context");
    goto main_ssl;
  }
  IS_ERROR((SSL_CTX_use_certificate_file (sslctx, "bin/cert/crt.pem", SSL_FILETYPE_PEM) <= 0) ||
      (SSL_CTX_use_PrivateKey_file (sslctx, "bin/cert/key.pem", SSL_FILETYPE_PEM) <= 0) ||
      (SSL_CTX_check_private_key(sslctx) <= 0)) {
    LOGE("Failed to use ssl certificate or private key");
    goto main_ssl_ctx;
  }
  {
    struct addrinfo hints, *reisult;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_TCP;
    IS_ERROR((t = getaddrinfo(SERVER_HOSTNAME, SERVER_PORT, &hints, &result))) {
      LOGE("getaddrinfo: %s", gai_strerror(t));
      goto main_ssl_ctx;
    }
    IS_ERROR((server_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) < 0) {
      freeaddrinfo (result);
      LOGE("create socket fail");
      goto main_ssl_ctx;
    }
    IS_ERROR(bind(server_sock, result->ai_addr, result->ai_addrlen) < 0 ||
      listen(server_sock, SOMAXCONN) < 0) {
      freeaddrinfo (result);
      LOGE("Socket failure (bind/listen)");
      goto main_sock;
    }
    freeaddrinfo (result);
  }
  IS_ERROR(pipe(pipe_fifo) < 0) {
    LOGE("Pipe failure");
    goto main_sock;
  }
  // make thread pool
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  for (t = 0; t < NUM_WORKERS; ++t)
    if (pthread_create(&workers[t], &attr, serving_client, NULL))
      break;
  pthread_attr_destroy(&attr);
  IS_ERROR(t < NUM_WORKERS) {
    LOGE("Insufficient resources to create thread");
    goto kill_worker;
  }
  
  // prepare sets
  FD_ZERO(&mset);
  FD_SET(server_sock, &mset);
  maxfd = server_sock + 1;
  LOGI("Server is started. Ctrl + C to stop serving ...");

main_select:
  dset = mset;
  TODO("need to spesify sigmask?");
  if (pselect(maxfd, &dset, NULL, NULL, NULL, NULL) == -1) {
    // signal may cause EINTR and alive false
    if ((errno != EINTR) || alive) {
      LOGW("Unexpected! pselect got error, %s", strerror(errno));
      goto main_select;
    } // fall through kill workers
  } else {
    t = accept(server_sock, NULL, NULL);
    IS_ERROR((t > 0) && (write(pipe_fifo[1], &t ,sizeof(int)) < sizeof(int))) {
      LOGW("Unexpected! write to pipe fail");
      close (t);
    }
    goto main_select;
  }
  // safe closing :-)
  ret = EXIT_SUCCESS;
kill_worker:
  t = -1;
  for (int i = 0; i < NUM_WORKERS; ++i) write(pipe_fifo[1], &t ,sizeof(int));
  while(active_workers) pthread_cond_wait(&pcond, &pmtx);
  FD_ZERO(&mset); // do i need it?
  close(pipe_fifo[0]);
  close(pipe_fifo[1]);
main_sock:
  close(server_sock);
main_ssl_ctx:
  SSL_CTX_free(sslctx);
main_ssl:
  EVP_cleanup();
main_end:
  LOGI("Server stoped");
  if (ret == EXIT_FAILURE) {
    if (errno) LOGE("\033[31mSystem: %s\033[0m", strerror(errno));
    for (unsigned long e = ERR_get_error (); e; e = ERR_get_error ()) {
      ERR_error_string_n(e, temps, MAX_MAIN_BUFFER);
      LOGE("\033[31mSSL: %s\033[0m", error_msg);
    }
  }
  return ret;
}

typedef struct {
  StringView name, value;
} header;
enum {
  
}

static int ssl_read(SSL *, char *, size_t);
static int ssl_write(SSL *, const char *, size_t);
static int ssl_sendfile(SSL *, int, size_t);

static void *serving_client(void *UNUSED_ARG(arg)) {
  unsigned long idT = (unsigned long) pthread_self();
  pthread_mutex_lock(&pmtx);
  ++active_workers;
  pthread_mutex_unlock(&pmtx);

#define MAX_CLIENT_BUFFER 4096
#define WORKER_INFO(x, ...) LOGI("\033[34mWorker[%lu]:\033[0m " x , idT, __VA_ARGS__)
#define WORKER_WARN(x, ...) LOGW("\033[33mWorker[%lu]:\033[0m " x , idT, __VA_ARGS__)
#define WORKER_ERR(x, ...) LOGE("\033[31mWorker[%lu]:\033[0m " x , idT, __VA_ARGS__)

  int client_sock = 0, t, fd, ret = 1;
  struct stat st;
  char buffer[MAX_CLIENT_BUFFER] = {0};
  
  struct {
    struct {
      unsigned short major, minor;
    } version;
    char method[16];
    char path[256];
    enum {
      INVALID = 0,
      BAD_REQUEST,
    } type;
  } response_format = {0};
  struct {
    char t1[128];
    char t2[128];
  } bt = {0};
  
  SSL *ssl;
get_job:
  IS_ERROR(read(pipe_fifo[0], &client_sock ,sizeof(int)) < sizeof(int)) {
    TODO("this may cause loop when read fail at kill_worker moment");
    WORKER_WARN("Unexpected! Failed to read pipe. %s", strerror(errno));
    goto get_job;
  }
  if (client_sock == -1) goto serv_end;
  if (!(ssl = SSL_new(sslctx))) {
    WORKER_WARN("SSL failed to create ssl");
    goto client_end;
  }
  SSL_set_fd(ssl, client_sock);
  IS_ERROR(SSL_accept(ssl) <= 0) {
    WORKER_WARN("SSL failed to perform handshake");
    goto client_stop;
  }
  memset(&response_format, 0, sizeof(response_format));
  request_headers.count = 0;
  t = 0;
  // read request
  IS_ERROR(ssl_read(ssl, buffer, MAX_CLIENT_BUFFER) <= 0) {
    WORKER_WARN("read return <= 0");
    goto client_close;
  }
  // parse request line
  IS_ERROR(sscanf(buffer, "HTTP/%hu.%hu %s %[^\r]\r\n", &response_format.version.major, &response_format.version.minor, response_format.path, response_format.method) < 3) {
    WORKER_WARN("Invalid request line: %s", buffer);
    goto client_clse;
  }
  WORKER_INFO("HTTP:%hu.%hu, method %s, Path %s", response_format.http_version.major, response_format.version.minor, response_format.path, response_format.method);
  // parse request headers
  for (const char *track_buffer = strtok(buffer, "\r\n") + 2; !track_buffer; track_buffer = strtok(NULL, "\r\n") + 2) {
    if(sscanf(track_buffer, "%[^:]: %[^\r]\r\n", bt.t1, bt.t2) < 2) break;
    WORKER_INFO("%s: %s", bt.t1, bt.t2);
  }
  // make response
  switch (response_format.type) {
    default:
    case INVALID: // 
      break;
    default:
    case BAD_REQUEST:
      const char *msg = 
        ST_REQ("400 Bad Request")
        HEAD_SERVER
        HEAD_CCLOSE
        ENDLINE;
      SSL_write(ssl, msg, strlen(msg))
      break;
  }
  ret = 0;
client_close:
  SSL_shutdown(ssl);
client_stop:
  SSL_free(ssl);
client_end:
  close(client_sock);
  if (ret) {
    WORKER_ERR("SSL Error code: %x", ret);
    if (errno) {
      WORKER_ERR("System: %s", strerror(errno));
      errno = 0;
    }
    for (unsigned long e = ERR_get_error (); e; e = ERR_get_error ()) {
      ERR_error_string_n(e, buffer, MAX_MAIN_BUFFER);
      WORKER_ERR("SSL: %s", buffer);
    }
  }
  ret = 1;
  goto get_job;
#undef WORKER_ERR
#undef WORKER_WARN
#undef WORKER_INF
#undef MAX_CLIENT_BUFFER
serv_end:
  pthread_mutex_lock(&pmtx);
  --active_workers;
  pthread_cond_signal(&pcond);
  pthread_mutex_unlock(&pmtx);
  return NULL;
}

static int ssl_read(SSL *ssl, char *buffer, size_t max_len) {
  int read;
  size_t readed = 0;
  do {
    read = SSL_read(ssl, buffer, max_len);
    if (read <= 0) {
      switch(SSL_get_error(ssl, read)) {
        default: // unknown possible error
          return -1;
        case SSL_ERROR_ZERO_RETURN: // 0 request, just close client ssl
          break;
        case SSL_ERROR_WANT_READ:
          continue;
      }
    }
    max_len -= read;
    buffer += read;
    readed += read;
  } while(max_len && memcmp(buffer - 4, "\r\n\r\n", 4));
  // ssl buffer must ended with \r\n\r\n
  return readed;
}

static int ssl_write(SSL *ssl, const char *msg, size_t len) {
  int write;
  size_t writen = 0;
  do {
    write = SSL_write(ssl, msg, len);
    if (write <= 0) {
      switch(SSL_get_error(ssl, write)) {
        default: // unknown possible error
        case SSL_ERROR_NONE: // might never happen
          return -1;
        case SSL_ERROR_ZERO_RETURN: // 0 request, just close client ssl
          break;
        case SSL_ERROR_WANT_WRITE:
          continue;
      }
    }
    writen += write;
    msg += write;
    len -= write;
  } while (len);
  return writen;
}
static int ssl_sendfile(SSL *ssl, int fd, size_t len) {
  size_t sended = 0;
  int send;
  do {
    send = SSL_sendfile(ssl, fd, sended, len, 0);
    if (send < 0) {
      switch(SSL_get_error(ssl, send)) {
        default: // unknown possible error
        case SSL_ERROR_NONE: // might never happen
          return -1;
        case SSL_ERROR_ZERO_RETURN: // 0 request, just close client ssl
          break;
        case SSL_ERROR_WANT_WRITE:
          continue;
      }
    }
    sended += send;
    len -= send;
  } while (len);
  return sended;
}