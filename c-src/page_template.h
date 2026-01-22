#ifndef _PAGE_TEMPLATE_INCLUDE_
#define _PAGE_TEMPLATE_INCLUDE_

#include "common.h"

#ifndef SERVER_HOSTNAME
  #define NEWLINE "\n"
  #define TB      "  "
  #define TB2X    "    "
  #define TB3XXX  "      "
  #define TB4XXXX "        "
#else
  #define ENDLINE "\r\n"
  #define NEWLINE
  #define TB      " "
  #define TB2X    " "
  #define TB3XXX  " "
  #define TB4XXXX " "
  
  #define HOST SERVER_HOSTNAME ":" STRINGIFY(SERVER_PORT)
  #define SERVER PROJECT_NAME"\/"SEM_VER
  
  #define ST_REQ(x)    "HTTP\/1.0 " x ENDLINE
  #define HEAD_SERVER  "Server: " SERVER " (Unix)" ENDLINE
  #define HEAD_CCLOSE  "Connection: Close" ENDLINE
  #define HEAD_CALIVE  "Connection: Keep-Alive" ENDLINE
  #define HEAD_LOC     "Location: https:\/\/" HOST "\/" ENDLINE
  #define HEAD_PAGE    "Content-Type: text\/html; charset=UTF-8" ENDLINE \
                       "Content-Language: en" ENDLINE \
                       "X-UA-Compatible: ie=edge" ENDLINE
  #define HEAD_CONTENT "Content-Length: %zu" ENDLINE\
                       "Content-Type: %s" ENDLINE
  // "Accept-Ranges: bytes"ENDLINE
  TODO("Accept-Ranges head was tell client about server is capable to resume data that fail to load, but i can't");
#endif // SERVER_HOSTNAME

static const char * const base_page_template[] = {
  "<!DOCTYPE html>"NEWLINE
  "<html lang='en'>"NEWLINE
  "<head>"NEWLINE
  TB"<title>%s</title>"NEWLINE
  TB"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"NEWLINE
  TB"<meta name='author' content='AriasaProp'>"NEWLINE
  TB"<meta name='description' content='%s'>"NEWLINE
  TB"<link rel='icon' type='image/x-icon' href='/assets/favicon.ico'>"NEWLINE
  TB"<link rel='icon' type='image/png' sizes='16x16' href='/assets/favicon-16x16.png'>"NEWLINE
  TB"<link rel='icon' type='image/png' sizes='32x32' href='/assets/favicon-32x32.png'>"NEWLINE
  TB"<link rel='apple-touch-icon' sizes='180x180' href='/assets/apple-touch-icon.png'>"NEWLINE
  TB"<link rel='preconnect' href='https://fonts.googleapis.com' crossorigin>"NEWLINE
  TB"<link rel='preconnect' href='https://fonts.gstatic.com' crossorigin>"NEWLINE
  TB"<link rel='preload' href='https://fonts.googleapis.com/css2?family=Poppins:wght@200;400;700' as='style' onload=\"this.onload=null;this.rel='stylesheet'\">"NEWLINE
  TB"<link rel='preload' href='https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght@28,700' as='style' onload=\"this.onload=null;this.rel='stylesheet'\">"NEWLINE
  TB"<noscript>"NEWLINE
  TB2X"<link rel='stylesheet' href='https://fonts.googleapis.com/css2?family=Poppins:wght@200;400;700'>"NEWLINE
  TB2X"<link rel='stylesheet' href='https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:wght,opsz@200,28'>"NEWLINE
  TB"</noscript>"NEWLINE
  TB"<link rel='stylesheet' href='/assets/common.css'>"NEWLINE
  TB"<link rel='stylesheet' href='/assets/%s.css'>"NEWLINE
  TB"<script src='/assets/common.js'></script>"NEWLINE
  "</head>"NEWLINE
  "<body>"NEWLINE
  TB"<noscript><span id='blocked'>Blocked: [need javascript to load!]</span></noscript>"NEWLINE
  TB"<header>"NEWLINE
  TB2X"<nav>"NEWLINE
  TB3XXX"<a%s><h2>"PROJECT_NAME"</h2></a>"NEWLINE
  TB3XXX"<span></span>"NEWLINE,
  TB3XXX"<a href='/%s' class='material-symbols-outlined'>%s</a>"NEWLINE,
  TB3XXX"<input id='ai' type='checkbox'>"NEWLINE
  TB3XXX"<label id='al' for='ai' class='material-symbols-outlined'></label>"NEWLINE
  TB3XXX"<form id='ab' action='/' method='GET'>"NEWLINE
  TB4XXXX"<input name='s' type='text' placeholder='Search' required>"NEWLINE
  TB4XXXX"<button type='reset' class='material-symbols-outlined'>&#xe6d0;</button>"NEWLINE
  TB4XXXX"<button type='submit' class='material-symbols-outlined'>&#xe8b6;</button>"NEWLINE
  TB3XXX"</form>"NEWLINE
  TB2X"</nav>"NEWLINE
  TB"</header>"NEWLINE,
  NEWLINE
  TB"<footer>"NEWLINE
  TB2X"<a href='mailto:ikomangwidiadaariasa12@gmail.com'><p>Google mail</p></a>"NEWLINE
  TB2X"<a href='https://github.com/AriasaProp'><p>Github</p></a>"NEWLINE
  TB2X"<a href='#'><p>Instagram</p></a>"NEWLINE
  TB2X"<a href='#'><p>Facebook</p></a>"NEWLINE
  TB2X"<a href='#'><p>Twitter</p></a>"NEWLINE
  TB2X"<hr/>"NEWLINE
  TB2X"<p>2025 AriasaProp | Made for fun!</p>"NEWLINE
  TB"</footer>"NEWLINE
  "</body>"NEWLINE
  "</html>"
};

#endif // _PAGE_TEMPLATE_INCLUDE_