#include "mbed.h"
#include <cctype>
#include <chrono>
#include <stdio.h>
#include <ctype.h>
#include <map>
#include <list>
#include <string>
#include "cctype"

#define LONG 1000ms
#define SHORT 500ms
#define SPACE 500ms
#define PAUSE 1000ms
#define END 2000ms


DigitalOut myled(LED1);

static const string name = "Lukas";

 static std::map<char, std::list<chrono::milliseconds>> ALFABET 
 {
     {'A', {SHORT, LONG}},
     {'B', {LONG, SHORT, SHORT, SHORT}},
     {'C', {LONG, SHORT, LONG, SHORT}},
     {'D', {LONG, SHORT, SHORT}},
     {'E', {SHORT}},
     {'F', {SHORT, SHORT, LONG, SHORT}},
     {'G', {LONG, LONG, SHORT}},
     {'H', {SHORT, SHORT, SHORT, SHORT}},
     {'I', {SHORT, SHORT}},
     {'J', {SHORT, LONG, LONG, LONG}},
     {'K', {LONG, SHORT, LONG}},
     {'L', {SHORT, LONG, SHORT, SHORT}},
     {'M', {LONG, LONG}},
     {'N', {LONG, SHORT}},
     {'O', {LONG, LONG, LONG}},
     {'P', {SHORT, LONG, LONG, SHORT}},
     {'Q', {LONG, LONG, SHORT, LONG}},
     {'R', {SHORT, LONG, SHORT}},
     {'S', {SHORT, SHORT, SHORT}},
     {'T', {LONG}},
     {'U', {SHORT, SHORT, LONG}},
     {'V', {SHORT, SHORT, SHORT, LONG}},
     {'W', {SHORT, LONG, LONG}},
     {'X', {LONG, SHORT, SHORT, LONG}},
     {'Y', {LONG, SHORT, LONG, LONG}},
     {'Z', {LONG, LONG, SHORT, SHORT}},
     
 };
 
 void make_blink(chrono::milliseconds miliseconds) {
     myled = 1;
     ThisThread::sleep_for(miliseconds);
     myled = 0;
 }

int main()
{
    while (true) 
    {
        for(char c : name) 
        {
            if (ALFABET.count(c) > 0) 
            {
                std:list<chrono::milliseconds> list = ALFABET[toupper(c)];
                for (chrono::milliseconds n : list) 
                {
                    make_blink(n);
                    ThisThread::sleep_for(SPACE);
                }
            } 
        }
        ThisThread::sleep_for(END);
    }
}
