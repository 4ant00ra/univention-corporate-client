#ifndef foopulsecoresockethfoo
#define foopulsecoresockethfoo

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>

#define ESHUTDOWN       WSAESHUTDOWN
#define ECONNRESET      WSAECONNRESET
#define ECONNABORTED    WSAECONNABORTED
#define ENETRESET       WSAENETRESET
#define EINPROGRESS     WSAEINPROGRESS
#define EAFNOSUPPORT    WSAEAFNOSUPPORT
#define ETIMEDOUT       WSAETIMEDOUT
#define ECONNREFUSED    WSAECONNREFUSED
#define EHOSTUNREACH    WSAEHOSTUNREACH
#define EWOULDBLOCK     WSAEWOULDBLOCK
#define EADDRINUSE      WSAEADDRINUSE

typedef long suseconds_t;

#endif

#ifdef HAVE_WS2TCPIP_H
#include <ws2tcpip.h>
#endif

#endif
