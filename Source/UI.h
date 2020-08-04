#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <vector>

#define MAX_LEN     100

using namespace std;

/* Didn't want to mess up beautiful spacing */
typedef vector<string> v_str;

class UI
{
public:
    UI();

    void    perror(const char* error);
    void    plog(const char* log);
    void    print(const char* str);
    void    recvPrint(const char* msg);
    void    sentPrint(const char* msg);
    string  prompt(const char* prompt);

    v_str   split(string& str, bool max_char = true);

    void    perror(string& error);
    void    plog(string& log);
    void    print(string& data);
    void    recvPrint(string& msg);
    void    sentPrint(string& msg);
    string  prompt(string& prompt);

    void    raw(char* str){cout << str;}
    void    raw(const string& str){cout << str;}
};

#endif // UI_H
