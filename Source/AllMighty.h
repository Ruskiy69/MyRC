#ifndef ALL_MIGHTY_H
#define ALL_MIGHTY_H

#include <sstream>
#include <string>

#include "MsgParser.h"

#define DEFAULT_HOSTNAME    "irc.freenode.net"
#define DEFAULT_IRC_PORT    "6667"
#define DEFAULT_NICK        "MyRC-User"
#define SSL_ENABLED         false

#define MAX_BUFFER          1024
#define MAX_MSG_LEN         510         // <apples> 510 + cr lf
#define TIMEOUT             2

class AllMighty
{
public:
    AllMighty();
    ~AllMighty();

    void setupKingdom(string hostname, string port, string nick);
    void dictate();

    /* Clean up function */
    void quit();

private:

    void getOrders();

    Socket*     irc;
    UI*         ui;
    MsgParser*  parser;
};

#endif // ALL_MIGHTY_H