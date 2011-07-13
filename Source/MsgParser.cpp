#include "MsgParser.h"

vector<string> split(string& str, char token)
{
    vector<string>  tokened;
    stringstream    ss;
    string          tmp(str);

    while(getline(ss, tmp, token))
    {
        tokened.push_back(tmp);
    }

    return tokened;
}

vector<string> split(char* str, char token)
{
    vector<string> tokened;
    stringstream   ss;
    string         tmp(str);

    while(getline(ss, tmp, token))
    {
        tokened.push_back(tmp);
    }

    return tokened;
}

void toUpper(string& str)
{
    for(unsigned int i=0; i < str.length(); i++)
        str[i] = toupper(str[i]);
}

string toUpper(const string& str)
{
    string tmp(str);

    for(unsigned int i=0; i < str.length(); i++)
    {
        tmp[i] = toupper(tmp[i]);
    }

    return tmp;
}

MsgParser::MsgParser(Socket& irc, UI& ui)
{
    this->irc       = &irc;
    this->ui        = &ui;
}

void MsgParser::setChannel(string channel)
{
    this->channel   = channel;
}

void MsgParser::setNick(string nick)
{
    this->nick      = nick;
}

string MsgParser::getNick()
{
    return this->nick;
}

string MsgParser::getChannel()
{
    return this->channel;
}

void MsgParser::parseIn(string& msg)
{
    vector<string> msgs = this->ui->split(msg);

    for(unsigned int i=0; i < msgs.size(); i++)
    {
        string tmp;
        if(!msgs[i].empty())
        {
            tmp = msgs[i];

            if(tmp.find("PRIVMSG") != string::npos)
            {
                // Example of a raw PM:
                // :MyRC-User!MyRC-User@HTS.org PRIVMSG #bots :test
                try
                {
                    stringstream    final;
                    string user(tmp.substr(1, tmp.find('!') - 1));
                    tmp = tmp.substr(tmp.find("PRIVMSG") + 8, -1);
                    string channel(tmp.substr(tmp.find('#'), tmp.find(' ')));
                    string sent(tmp.substr(tmp.find(':') + 1, -1));
                    final << user << " (" << channel << "): " << sent;
                    this->ui->recvPrint(final.str().c_str());
                }
                catch(exception& e)
                {
                    this->ui->perror(e.what());
                    this->ui->recvPrint(tmp);
                }
            }

            else if(tmp.find("PING") != string::npos)
            {
                stringstream reply;
                reply << "PONG :" << tmp.substr(7, -1) << END;
                this->irc->sendall(reply.str().c_str());
            }

            else if(tmp.find("PONG") != string::npos)
            {
                stringstream reply;
                reply << "PING :" << tmp.substr(6, -1) << END;
                this->irc->sendall(reply.str().c_str());
            }
            // Example of a raw QUIT:
            //:patience_!patience_@HTS-787738E1.net QUIT :Quit: patience_ is leaving...
            else if(tmp.find("QUIT") != string::npos)
            {
                stringstream final;
                string user, q_msg, tmp2;

                user    = tmp.substr(1, tmp.find('!') - 1);
                tmp2    = tmp.substr(1, -1);
                q_msg   = tmp2.substr(tmp2.find(':'), -1);

                final << user << " has quit (" << q_msg << ").";
                this->ui->recvPrint(final.str().c_str());
            }

            else if((tmp.find("QUIT") != string::npos) &&
                (tmp.find(this->getNick()) != string::npos))
            {
                this->ui->perror("The server has forced us to quit!");
                exit(1);
            }

            else if(tmp.find("JOIN") != string::npos)
            {
                string user, channel;
                stringstream print;
                user = tmp.substr(1, tmp.find('!') - 1);
                channel = tmp.substr(tmp.find("JOIN") + 6, -1);
                print << user << " has joined " << channel;
                this->ui->recvPrint(print.str().c_str());
            }

            else if(tmp.find(this->getNick()) == string::npos)
            {
                vector<string> lines = split(tmp, ':');

                if(lines.size() == 1)
                    this->ui->recvPrint(lines[0]);
                else if(lines.size() == 2)
                    this->ui->recvPrint(lines[1]);
                else
                    this->ui->recvPrint(tmp);
            }
            else
            {
                string str(tmp.substr(tmp.find(this->getNick()) + this->getNick().length(), -1));
                this->ui->recvPrint(str);
            }
        }
    }
}

bool MsgParser::parseOut(string& msg)
{
    stringstream to_send;
    stringstream to_print;

    // If msg.find(some string) is equal to 0,
    // that means it's at the very beginning, 
    // signifying that the user wants the command
    // to be used.

    if(msg.find("/join") == 0)
    {
        to_print    << "You have joined " << msg.substr(6, -1);
        to_send     << "JOIN" << msg.substr(5, -1) << END;
        this->setChannel(msg.substr(6, -1));
    }
    else if(msg.find("/part") == 0)
    {
        if(msg.length() == 5)
        {
            to_print    << "You have left " << this->getChannel();
            to_send     << "PART " << this->getChannel() << END;
        }
        else
        {
            to_print    << "You have left " << msg.substr(6, -1);
            to_send     << "PART " << msg.substr(6, -1) << END;
        }
    }
    else if(msg.find("/nick") == 0)
    {
        this->setNick(msg.substr(6, -1));
        to_print    << "You are now known as " << this->getNick();
        to_send     << "NICK" << msg.substr(5, -1) << END;
    }
    else if(msg.find("/msg") == 0)
    {
        string user(msg.substr(msg.find(' ') + 1, -1));
        string message = user.substr(user.find(' ') + 1, -1);
        user = user.substr(0, user.find(' '));

        to_print    << this->getNick() << " (" << user << "): " << message;
        to_send     << "PRIVMSG " << user << " :" << message << END;
    }
    else if(msg.find("/connect") == 0)
    {
        return true;
    }
    else if(msg.find("/raw") == 0)
    {
        to_print    << this->getNick() << " (" << this->getChannel() << "): " << msg;
        to_send     << msg.substr(5, -1) << END;
    }
    else if(msg.find("/help") == 0)
    {
        this->help();
    }
    else if(msg.find("/quit") == 0)
    {
        if(msg.length() == 5)
            to_send << "QUIT :" << this->getNick() << " is leaving..." << END;
        else
            to_send << "QUIT :" << msg.substr(6, -1) << END;
        this->irc->sendall(to_send.str().c_str());
        exit(0);
    }
    else    // Default action is to just chat on the current channel.
    {
        if(msg.find('/') == 0)
            this->ui->perror("Unrecognized command");
        else if(!msg.empty())
        {
            to_print.str(string());
            to_send     << "PRIVMSG " << this->getChannel() << " :" << msg << END;
        }
    }

    if(!to_send.str().empty())
    {
        if(!to_print.str().empty())
        {
            this->ui->sentPrint(to_print.str().c_str());
            to_print.str(string());
        }
        this->irc->sendall(to_send.str().c_str());
        to_send.str(string());
    }

    return false;
}

void MsgParser::help()
{
	this->ui->plog("\n\n\n\n\n");
	this->ui->plog("/connect [hostname] \t-Connect to a new server, default port is 6667");
	this->ui->plog("/msg [recipient] [message]\t-Private message");
	this->ui->plog("/join [channel]\t\t-Join a channel");
	this->ui->plog("/nick [nickname]\t\t-Change nickname");
	this->ui->plog("/raw [IRC command]\t\t-Send a raw IRC command");
    this->ui->plog("/part <channel>\t\t-Leave a channel");
    this->ui->plog("/quit <Quit Message>\t-Quit the server, leaving an optional departure message");
	this->ui->plog("/help \t\t\t-This message");
	this->ui->plog("\n\n");
}
