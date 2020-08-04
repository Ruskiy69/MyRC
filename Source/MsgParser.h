#pragma once

#include <sstream>
#include <string>
#include <exception>

#include "UI.h"
#include "Socket.h"

#ifdef  _DEBUG
#define Debug(x)    cout << "[DEBUG] " << x << endl;
#else
#define Debug(x)
#endif // _DEBUG

#define END     "\r\n"

vector<string>  split(string& str,  string& token);
vector<string>  split(char* str,    string& token);

void            toUpper(string& str);
string          toUpperNoChange(const string& str);

class MsgParser
{
public:
    MsgParser(Socket& irc, UI& ui);

    void    changeIrcSocket(Socket* socket){this->irc = socket;}

    void    setChannel(string channel);
    void    setNick(string nick);

    string  getNick();
    string  getChannel();

    /*
        Parses the outgoing message, and creates a stringstream
        with the properly formatted raw IRC message. An un-elegant
        solution to the dilemma of setting up a new socket is
        solved by returning true when the user wants to "/connect".
        False is return otherwise.
    */
    bool    parseOut(string& msg);

    /*
        Parses the incoming message, prettifying it for the user.
    */
    void    parseIn(string& msg);

    /*
        A help message to provide the user with basic functions.
    */
    void    help();


private:
    Socket*     irc;
    UI*         ui;

    string channel;
    string nick;
};
