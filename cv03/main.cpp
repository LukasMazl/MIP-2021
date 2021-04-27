#include "mbed.h"
#include "rtos.h"
#include "stm32469i_discovery.h"
#include "TS_DISCO_F469NI.h"
#include "LCD_DISCO_F469NI.h"
#include "F469_GUI.hpp"
#include <cstdint>
#include <cstdio>
#include <string>

#define MAX_THREADS 5
#define X_POSITION 400
#define Y_POSITION 200
#define DEFAULT_LABEL_VALUE ""
#define COUNTDOWN 25

TS_StateTypeDef TS_State;

static Semaphore semaphore(1);
Label* threadLogger = new Label(X_POSITION, Y_POSITION, DEFAULT_LABEL_VALUE, Label::CENTER, Font16);

bool touch() {
    BSP_TS_GetState(&TS_State);
    return TS_State.touchDetected;
}

void doThreadWork(const string* threadName) {
    BSP_LCD_Clear(LCD_COLOR_LIGHTRED);
    int countDown = COUNTDOWN;
    while(countDown-- > 0) {
        threadLogger->Draw(*threadName + ": " + to_string(countDown) + "ms");
        ThisThread::sleep_for(100ms);
    }
    
}

void saveThreadContext() {
    ThisThread::sleep_for(200ms); // Just simulation
}

void thread_task(const string* threadName) {
    
    while(true) {
        semaphore.acquire();
        Thread doingThread;
        doingThread.start(callback(doThreadWork, threadName));
        while((doingThread.get_state() == Thread::Running ||
                doingThread.get_state() == Thread::Ready ||
                doingThread.get_state() == Thread::WaitingDelay) &&
                !touch()) {
           continue;
        }

        saveThreadContext();
        semaphore.release();
    }
}

int main()
{
    BSP_LCD_DisplayOn();

    Thread threads[MAX_THREADS];
    for(int i = 0; i < MAX_THREADS; i++) {
        const string* threadName = new string("Thread " + to_string(i));
        threads[i].start(callback(thread_task, threadName));
    }
    string tr = ThisThread::get_name();
    thread_task(&tr);
}
