#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char **argv) {
  int cr = creat("bay.txt", S_IRUSR | S_IWUSR);
  dprintf(cr, "OK");
  close(cr);
  return 0;
}