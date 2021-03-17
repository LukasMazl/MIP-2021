#include "mbed.h"
#include "stm32469i_discovery.h"
#include "TS_DISCO_F469NI.h"
#include "LCD_DISCO_F469NI.h"
#include "F469_GUI.hpp"
#include <string>

#define START_TIME 68

volatile static int COUNTDOWN = START_TIME;
Thread thread;
bool thread_is_done = false;

string timeToString(int time) {
    int hour = time / 3600;
    short minute = (time - hour * 3600) / 60;
    short seconds = time % 60;

    return to_string(hour) + ":" + ((minute <= 10)? "0":"") + to_string(minute) + ":" + ((seconds < 10)? "0":"") + to_string(seconds);
} 

void countdown() {
    while(COUNTDOWN >= 0) {
        COUNTDOWN--;
        ThisThread::sleep_for(1s);
    }
    thread_is_done = true;
}

int main()
{
    BSP_LED_Init(DISCO_LED_BLUE);

    Label obj10(400, 20, "Cviceni 2: Budikobomba", Label::CENTER, Font16);
    
    Label time(400, 200, "00:00:00", Label::CENTER, Font24);
    

    Button addMinute(10, 54, 100, 80, "Add Minute");
    Button minusMinute(130, 54, 100, 80, "Minus Minute");

    Button addSecond(10, 254, 100, 80, "Add seconds");
    Button minusSecond(130, 254, 100, 80, "Minus seconds");

    Button resetButton(10, 354, 100, 80, "Reset");


    thread.start(countdown);
    while (true)
    {
        if (addMinute.Touched()) {
            addMinute.Activate();
            COUNTDOWN += 60;
        } 
        
        if (addSecond.Touched()) {
            addSecond.Activate();
            COUNTDOWN += 1;
        }

        if (minusMinute.Touched()) {
            minusMinute.Activate();
            COUNTDOWN -= 60;
        }

        if (minusSecond.Touched()) {
            minusSecond.Activate();
            COUNTDOWN -= 1;
        }

        if (resetButton.Touched()) {
            COUNTDOWN = START_TIME;
            resetButton.Activate();
            if(thread_is_done) {
                thread.start(countdown);
                thread_is_done = false;
            }
        }

        if(COUNTDOWN >= 0) {
            string timeString = timeToString(COUNTDOWN);
            time.Draw(timeString);

            ThisThread::sleep_for(200ms);
        } else {
            //Do animation
            string boomText = "boooom";
            for(int i = 0; i < 10; i++) {
                time.Draw(boomText);
                boomText = "!!" + boomText + "!!";
                ThisThread::sleep_for(2s);
            }
        }
    }
}