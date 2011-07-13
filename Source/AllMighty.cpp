#include "AllMighty.h"

AllMighty::AllMighty()
{
    this->irc       = new Socket(AF_INET, SOCK_STREAM);
    this->ui        = new UI;
    this->parser    = new MsgParser(*(this->irc), *(this->ui));
}

AllMighty::~AllMighty()
{
    this->ui->print("Quitting...");
    this->irc->sendall("QUIT :User has quit.");
    this->irc->shutdown();

    this->ui->raw("\n\n");

    delete this->irc;
    delete this->ui;
    delete this->parser;
}

void AllMighty::setupKingdom(string hostname, string port, string nick)
{
    /* 
        Set up the chosen nickname and register the user.
    */

    stringstream setup;

    this->parser->setNick(nick);
    this->irc->setTimeOut(2);

    Debug("Connecting...");

    cout << "[*] Connecting to " << hostname << " on port " << port << "..." << endl;

    // Establish initial connection
    this->irc->connect(hostname.c_str(), port.c_str());

    char* msg = NULL;

    // Check for errors concerning connection
    // If there aren't any errors, this will
    // display the first messsages from the server.
    while((msg = irc->nonBlockRecv()) != NULL)
    {
        string tmp(msg);
        toUpper(tmp);

        if(tmp.find("ERROR") != string::npos)
        {
            this->ui->perror("There was an error connecting to the server!");
            this->ui->perror("The server has sent: ");
            this->ui->perror(msg);
            exit(1);
        }
    }

    cout << "[*] " << "Connected to " << hostname << "!" << endl;

    Debug("Setting up nick");

    // Send nick
    setup << "NICK " << nick << END;
    this->irc->sendall(setup.str().c_str());

    // Check for nick errors
    while((msg = irc->nonBlockRecv()) != NULL)
    {
        string tmp(msg);
        toUpper(tmp);

        if(tmp.find("NICK") != string::npos)
        {
            ui->perror("This nickname is either already in use, or has illegal characters.");
            nick = ui->prompt("Please choose a new nickname: ");
            setup.str(string());
            setup << "NICK " << nick << END;
            this->irc->sendall(setup.str().c_str());
        }
    }

    this->parser->setNick(nick);

    // Clear the stringstream.
    setup.str(string());

    // Register user
    setup << "USER " << nick + " " << nick.length() << " * " + nick + END;
    this->irc->sendall(setup.str().c_str());

    Debug("MOTD:");

    // Get the Message Of The Day
    this->getOrders();

    Debug("Done");
}

void AllMighty::dictate()
{
    /*
        The main loop. Select()'ing through the socket 
        and stdin, (platform dependent) when the user starts
        typing, it will do stdin, but if the user isn't, it 
        will continuously check for incoming data.

        WINDOWS issue: If the user starts typing, it won't check
        for new data on the socket until he or she presses 
        ENTER. This can lead to ping timeouts and kicks from a 
        server. Never stop typing in the middle of a message and
        not send.
        If you don't have anything typed and it seems to be not
        receiving messages, press ENTER to force a receive.
    */

    for(int i=0; i < 50; i++)
        cout << endl;
    this->parser->help();

    string msg;

#ifdef _WIN32
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
#else
    fd_set          read_fds;
    struct timeval  to;
    int             retval;
#endif // _WIN32

    while(msg.find("/quit") == -1)
    {
        msg.clear();

#ifdef _WIN32
        this->getOrders();
        cout << "\r>>> " << this->parser->getNick() << ": ";
#else
        cout << "\r>>> " << this->parser->getNick() << ": ";
        this->getOrders();
#endif // _WIN32

#ifdef _WIN32

        if(WaitForSingleObject(handle, 0) == WAIT_OBJECT_0)
        {
            getline(cin, msg);
        }

#else
        FD_ZERO(&read_fds);
        FD_SET(fileno(stdin), &read_fds);

        to.tv_sec   = 1;
        to.tv_usec  = 0;

        if(select(fileno(stdin) + 1, &read_fds, NULL, NULL, &to) > 0)
        {
            if(FD_ISSET(fileno(stdin), &read_fds))
            {
                getline(cin, msg);
            }
        }

#endif // _WIN32

        if(!msg.empty())
        if(this->parser->parseOut(msg))     // User wants a new server
        {
            this->irc->sendall("QUIT");
            this->irc->shutdown();

            this->irc = new Socket(AF_INET, SOCK_STREAM);
            this->parser->changeIrcSocket(this->irc);

            string host, port;

            // 9 is the length of the preceding "/connect ".

            // The second part of the substr() checks if there's
            // a space in the hostname, meaning the user
            // specified a port.

            host = msg.substr(9, -1);
            port = DEFAULT_IRC_PORT;

            this->setupKingdom(host, port, this->parser->getNick());
        }
    }
}

void AllMighty::getOrders()
{
    /*
        As long as there is a message in the socket, 
        receive it, parse it, and print it out nicely
        for the user.
    */

    // To overwrite the nick prompt, just in case.
    this->ui->raw("\r");

    char*           msg         = NULL;
    int             first_to    = this->irc->getTimeOut();

    stringstream    reply;

    this->irc->setTimeOut(1);

    while((msg = irc->nonBlockRecv()) != NULL)
    {
        string tmp(msg);

        // For pings from the server
        if(tmp.find("PING") != string::npos)
        {
            reply << "PONG :" << tmp.substr(7, -1) << END;
            this->irc->sendall(reply.str().c_str());
        }
        else if(tmp.find("PONG") != string::npos)
        {
            reply << "PING :" << tmp.substr(6, -1) << END;
            this->irc->sendall(reply.str().c_str());
        }
        else
        {
            this->parser->parseIn(tmp);
        }
    }

    this->irc->setTimeOut(first_to);
}