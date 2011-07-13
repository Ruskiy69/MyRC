/************************************
*               MyRC                *
*                                   *
*   Created By: George Kudrayvtsev  *
*       (AKA patience and Ruskiy69) *
*                                   *
*   Finished On: 6/??/2011          *
*                                   *
*   Version: 0.1 Alpha              *
*                                   *
*   Tested On: Windows 7 64-Bit,    *
*       Ubuntu 9.10, BackTrack 4    *
*                                   *
*   License: Creative Commons       *
*                                   *
*   (C) Copyright Kudrayvtsev 2011  *
*                                   *
*************************************/

#include <string>

#include "AllMighty.h"

#define VERSION_STR         "0.1 Alpha"

using namespace std;

/* Functions for cli arguments */
void parse_args(int argc, char* argv[]);
void usage();

/* Global variables for argument-handling */
string hostname;
string port;
string nick;

int main(int argc, char* argv[])
{
    try
    {
        /* Set hostname, port, and nick based on args */
        parse_args(argc, argv);

        AllMighty  master;

        if(hostname == DEFAULT_HOSTNAME && port == DEFAULT_IRC_PORT)
        {
            string response;

            cout << "[*] Using default hostname and port..." << endl;
            cout << "[*] Hostname: " << hostname << endl;
            cout << "[*] Port: " << port << endl;
            cout << "[*] Do you wish to continue with default options?" << endl;
            cout << "[*] Y/N: ";

            getline(cin, response);
            toUpper(response);

            if(response == "Y" || response == "YES")
            {
                master.setupKingdom(hostname, port, nick);
                master.dictate();
            }

            else
            {
                cout << "[*] Enter a hostname," << endl;
                cout << "[*] Press ENTER for default: ";
                getline(cin, hostname);

                if(hostname.empty())
                    hostname = DEFAULT_HOSTNAME;

                cout << "[*] Hostname set to " << hostname << endl;

                cout << "[*] Enter a port," << endl;
                cout << "[*] Press ENTER for default: ";
                getline(cin, port);

                if(port.empty())
                    port = DEFAULT_IRC_PORT;

                cout << "[*] Port set to " << port << endl;

                cout << "[*] Enter a nickname," << endl;
                cout << "[*] Press ENTER for default: ";
                getline(cin, nick);

                if(nick.empty())
                    nick = DEFAULT_NICK;

                cout << "[*] Nickname set to " << nick << endl;

                master.setupKingdom(hostname, port, nick);
                master.dictate();
            }
        }
        else
        {
            master.setupKingdom(hostname, port, nick);
            master.dictate();
        }
    }

    catch(exception& e)
    {
        cout << "[-] Something went terribly wrong..." << endl;
        cout << "[-] The error was: " << e.what() << endl;
        cout << "[-] Exiting..." << endl;
        exit(1);
    }

    catch(...)
    {
        cout << "[-] Something went terribly wrong..." << endl;
        cout << "[-] Exiting..." << endl;
        exit(1);
    }

    return 0;
}

void usage()
{
    cout << endl;
    cout << "USAGE: MyRC.exe [hostname] [port] [nick]" << endl << endl;
    cout << "Options:" << endl;
    cout << "hostname   - The IRC server you wish to connect to, default is " << DEFAULT_HOSTNAME <<"." << endl;
    cout << "port       - The port to connect to, default is " << DEFAULT_IRC_PORT << ". SSL is NOT supported." << endl;
    cout << "nick       - Optional nick to use, can be changed later. Default is MyRC-User" << endl << endl;
    cout << "If you use only the nick argument, type: \"MyRC.exe NULL NULL [your nick]\"" << endl << endl;
    cout << "Version: " << VERSION_STR << endl;
    cout << endl << endl;

    exit(EXIT_SUCCESS);
}

void parse_args(int argc, char* argv[])
{
    if(argc > 4)
        usage();

    if(argc == 2 && ((strncmp(argv[1], "--help", 6) == 0)
        || (strncmp(argv[1], "-h", 2) == 0)))
    {
        usage();
    }

    if(argc == 4)
    {
        hostname    = argv[1];
        port        = argv[2];
        nick        = argv[3];

        if(hostname == "NULL")
            hostname    = DEFAULT_HOSTNAME;
        if(port        == "NULL")
            port        = DEFAULT_IRC_PORT;
    }
    else if(argc == 3)
    {
        hostname    = argv[1];
        port        = argv[2];
        nick        = DEFAULT_NICK;

        if(hostname == "NULL" || port == "NULL")
            usage();

    }
    else if(argc == 2)
    {
        hostname    = argv[1];
        port        = DEFAULT_IRC_PORT;
        nick        = DEFAULT_NICK;

        if(hostname == "NULL")
            usage();
    }
    else if(argc == 1)
    {
        hostname    = DEFAULT_HOSTNAME;
        port        = DEFAULT_IRC_PORT;
        nick        = DEFAULT_NICK;
    }
    else
    {
        usage();
    }
}
