#include "UI.h"

UI::UI()
{
}

void UI::perror(const char* error)
{
    string data(error);

    vector<string> lines = this->split(data);

    for(unsigned int i=0; i < lines.size(); i++)
    {
        cout << "[-] " << lines[i] << endl;
    }
}

void UI::plog(const char* log)
{
    string data(log);

    vector<string> lines = this->split(data);

    for(unsigned int i=0; i < lines.size(); i++)
    {
        cout << "[*] " << lines[i] << endl;
    }
}

void UI::print(const char* str)
{
    string data = (string)str;

    vector<string> lines = this->split(data);

    for(unsigned int i=0; i < lines.size(); i++)
    {
        this->plog(lines[i].c_str());
    }
}

void UI::recvPrint(const char* msg)
{
    string data = (string)msg;

    if(data.empty())
        return;

    vector<string> lines = this->split(data);

    for(unsigned int i=0; i < lines.size(); i++)
    {
        cout << "<<< " << lines[i] << endl;
    }
}

void UI::sentPrint(const char* msg)
{
    string data = (string)msg;

    if(data.empty())
        return;

    vector<string> lines = this->split(data, false);

    for(unsigned int i=0; i < lines.size(); i++)
    {
        cout << ">>> " << lines[i] << endl;
    }
}

string UI::prompt(const char* prompt)
{
    string buffer;
    cout << "\n>>> " << prompt;
    getline(cin, buffer);
    return buffer;
}

v_str UI::split(string& str, bool max_char)
{
    vector<string> lines;

    int index;

    do
    {
        index = str.find("\r\n", 0);

        string tmp = str.substr(0, index);

        if(max_char)
        {
            while(tmp.length() > MAX_LEN)
            {
                lines.push_back(tmp.substr(0, MAX_LEN));
                tmp = tmp.substr(MAX_LEN, -1);
            }
        }

        lines.push_back(tmp);

        if(index != -1)
        {
            str = str.substr(index + 2, -1);
        }
    }
    while(index != -1);

    return lines;
}

void UI::perror(string& error)
{
    cerr << "[-] " << error << endl;
}

void UI::plog(string& log)
{
    cout << "[*] " << log << endl;
}

void UI::print(string& data)
{
    vector<string> lines = this->split(data);

    for(unsigned int i=0; i < lines.size(); i++)
    {
        this->plog(lines[i].c_str());
    }
}

void UI::recvPrint(string& msg)
{
    if(msg.empty())
        return;

    vector<string> lines = this->split(msg);

    for(unsigned int i=0; i < lines.size(); i++)
    {
        cout << "<<< " << lines[i] << endl;
    }
}

void UI::sentPrint(string& msg)
{
    if(msg.empty())
        return;

    vector<string> lines = this->split(msg, false);

    for(unsigned int i=0; i < lines.size(); i++)
    {
        cout << ">>> " << lines[i] << endl;
    }
}

string UI::prompt(string& prompt)
{
    string buffer;
    cout << "\n>>> " << prompt;
    getline(cin, buffer);
    return buffer;
}
