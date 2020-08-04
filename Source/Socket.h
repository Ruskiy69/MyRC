/**
 * Holy crap this code is embarrasing to look at 9 years later...
 *
 * Provides easy socket API access, a simple wrapper for both server and client
 * sockets. Somewhat modeled after the Python `socket` module.
 **/

#pragma once

#include <stdio.h>

#include "Errors.h"

#ifdef _WIN32
  #define WS2_INCLUDED
  #include <WinSock2.h>
  #include <WS2tcpip.h>
  #pragma comment(lib, "ws2_32.lib")
#else
  #include "Errors.h"
  #include <stdlib.h>
  #include <unistd.h>
  #include <errno.h>
  #include <string.h>
  #include <netdb.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
#endif  // _WIN32

#define LOCALHOST    "127.0.0.1"
#define DEFAULT_PORT "2012"

enum SOCK_TYPE {TCP_CLIENT, TCP_SERVER, UDP_CLIENT, UDP_SERVER};

class Socket
{
public:

    /**
     * If on Windows, initializes the WinSock2 API. Sets up the addrinfo struct
     * based on the given criteria.
     **/
    Socket(const int family, const int sock_type, SOCK_TYPE socket_type = TCP_CLIENT);
    ~Socket();

    /// Connects to the given host on the given port.
    void  connect(const char* hostname, const char* port);

    /*
        Server-side functions
    */

    /// Awaits a connection from an incoming client.
    int   accept();

    /// Binds the server socket to a given address:port (default: localhost:2012).
    void  bind(const char* host = LOCALHOST, const char* port = DEFAULT_PORT);


    /// Sets a maximum amount of connections to pend to the socket. After the
    /// previous connection is closed, the server will accept data from the next
    /// incoming connection (default: 5).
    void  listen(const int backlog = 5);

    /*
        Functions for both server and client sockets
    */

    /// Sends the given data to the server. [TCP]
    void  sendall(const char* data);

    /// Sends the given data to a server, without confirmation. [UDP]
    void  sendto(const char* hostname, const char* port, const char* data, const int size);

    char* recv(const int bufsize = 4096);

    /// Receives data from the server, storing it into a given buffer.
    void  recv_into(char* buffer, const int size);

    char* nonBlockRecv(const int bufsize = 4096);

    void  setTimeOut(const int timeout);
    int   getTimeOut(){return this->timeout;}

    char* getpeername();
    char* getsockname();

    int   getSock();
    int   getClientSock();

    /**
     * Shuts down the main socket, specifying whether it should shut down only
     * sends (SD_SEND [WIN] or SHUT_WR [*NIX]), receives (SD_RECV [WIN] or
     * SHUT_RD [*NIX]), or both (SD_BOTH [WIN] or SHUT_RDWR [*NIX]). The default
     * is shutting down both sends and receives.
     **/
#ifdef _WIN32
    void shutdown(const int how = SD_BOTH);
#else
    void shutdown(const int how = SHUT_RDWR);
#endif // _WIN32

private:
    int                     sock;
    int                     client_sock;

    SOCK_TYPE               socket_type;

    struct addrinfo         hints;
    struct addrinfo*        serv_info;
    struct sockaddr_storage client_addr;
    socklen_t               addr_size;

    int                     timeout;
};
