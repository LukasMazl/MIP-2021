#include "mbed.h"
#include "rtos.h"
#include "stm32469i_discovery.h"
#include "TS_DISCO_F469NI.h"
#include "LCD_DISCO_F469NI.h"
#include "F469_GUI.hpp"
#include <cstdint>
#include <cstdio>
#include <string>
#include <list>

#define X_POSITION 400
#define Y_POSITION 200
#define DEFAULT_LABEL_VALUE ""

static Semaphore semaphore(1);

namespace std {

    enum Event {
        RADIO_OFF,
        RADIO_ON,
        MP3,
        ON_STAGE_CHANGE,
        HANDS_FREE,
        DEBUG,
        AM_FM,
        MAKE_CALL,
        INCOMING_CALL
    };

    static Label* threadLogger = new Label(X_POSITION, Y_POSITION, DEFAULT_LABEL_VALUE, Label::CENTER, Font16);
    static Label* currentState = new Label(X_POSITION, 10, DEFAULT_LABEL_VALUE, Label::CENTER, Font16);

    class RadioContext {

        public:
            RadioContext() {
                this->currentState = RADIO_OFF;
                this->previousState = RADIO_OFF;
                this->running = false;
            }

            void setNewState(Event newState) {
                this->previousState = this->currentState;
                this->currentState = newState;
            }

            Event getCurrentState() {
                return currentState;
            }

            Event getPreviousState() {
                return previousState;
            }

            void setRunning() {
                this->running = true;
            }

            void setStopped() {
                this->running = false;
                this->setNewState(RADIO_OFF);
            }

            bool isRunnig() {
                return running;
            }

        private:
            Event currentState;
            Event previousState;
            bool running;
    };

    static RadioContext radioContext;

    class ThreadWrapper {

        public:
            ThreadWrapper(std::list<Event> event, mbed::Callback<void()> task) {
                this->eventListening = event;
                this->task = task;
                this->thread = new Thread();
                this->fired = false;
            }

            ThreadWrapper(Event event, mbed::Callback<void()> task) {
                std::list<Event> events = {event};
                this->eventListening = events;
                this->task = task;
                this->thread = new Thread();
                this->fired = false;
            }

            void setTask(mbed::Callback<void()> task) {
                this->task = task;
            }

            std::list<Event> getEventListening() {
                return eventListening;
            }

            mbed::Callback<void()> getOnEventCome() {
                return task;
            }

            osStatus start() {
               if(this->fired) {
                   this->thread = new Thread();
               }
               this->fired = true;
               return this->thread->start(task);
            }

        private:
            std::list<Event> eventListening;
            mbed::Callback<void()> task;
            Thread* thread;
            bool fired;
    };

    void fireEvent(Event event);

    void onRadioOnState() {
        semaphore.acquire();
        std::currentState->Draw("Radio ON");
        Event previousState = radioContext.getPreviousState();
        if (previousState != RADIO_OFF) {
            fireEvent(previousState);
            radioContext.setNewState(previousState);
        }
        radioContext.setRunning();
        semaphore.release();
    }

    void onMp3State() {
        semaphore.acquire();
        std::currentState->Draw("MP3 State");
        radioContext.setNewState(MP3);
        threadLogger->Draw("Hmm sounds good!");
        semaphore.release();
    }

    void onHandsFree() {
        semaphore.acquire();
        std::currentState->Draw("Handsfree");
        threadLogger->Draw("Volaaame!");
        radioContext.setNewState(HANDS_FREE);
        semaphore.release();
    }
    
    void onOffRadio() {
        semaphore.acquire();
        std::currentState->Draw("Radio off");
        radioContext.setStopped();
        threadLogger->Draw("zzzzzzzzzzzzzzzzzzzz....");
        semaphore.release();
    }

    void onMakeCall() {
        semaphore.acquire();
        std::currentState->Draw("Handsfree");
        threadLogger->Draw("Komu zavolame?");
        radioContext.setNewState(MAKE_CALL);
        semaphore.release();
    }

    void onDebug() {

    }

    void onAffmState() {
        semaphore.acquire();
        std::currentState->Draw("Radio AF/FM");
        threadLogger->Draw("raaaaaaadio! :)))");
        radioContext.setNewState(AM_FM);
        semaphore.release();
    }

    std::list<ThreadWrapper*> initThreads() {
        return std::list<ThreadWrapper*> {
            #ifdef DEBUG_MODE
                new ThreadWrapper(DEBUG, onDebug),
            #endif
            new ThreadWrapper(RADIO_OFF, onOffRadio),
            new ThreadWrapper(RADIO_ON, onRadioOnState),
            new ThreadWrapper(HANDS_FREE, onHandsFree),
            new ThreadWrapper(MP3, onMp3State),
            new ThreadWrapper(AM_FM, onAffmState),
            new ThreadWrapper(MAKE_CALL, onMakeCall)
        };
   }
    static std::list<ThreadWrapper*> threadWrapperlist = initThreads();

    void fireEvent(Event event) {
        for (ThreadWrapper* threadWrapper : threadWrapperlist) {
            bool found = false;
            for (Event threadListenEvent : threadWrapper->getEventListening()) {
                if (event == threadListenEvent) {
                    found = true;
                    break;
                }
            }
            if(found) {
                threadWrapper->start();
            }
        }
    }

}

int main()
{
    BSP_LCD_DisplayOn();
    
    Button powerOnButton(115, 80, 100, 80, "ON");
    Button powerOffButton(10, 80, 100, 80, "OFF");

    Button MP3(10, 160, 100, 80, "MP3");
    Button handfree(10, 240, 100, 80, "Handsfree");

    Button AM_FM(10, 320, 100, 80, "AM/FM");
  //  Button INCOMING_CALL(10, 250, 100, 80, "Incoming Call");
    Button make_call(10, 400, 100, 80, "Make call");

    while(true) {

        if(powerOnButton.Touched()) {
            fireEvent(RADIO_ON);
            powerOnButton.Activate();
        }

        if(powerOffButton.Touched()) {
            fireEvent(RADIO_OFF);
            powerOffButton.Activate();
        }

        if(MP3.Touched()) {
            fireEvent(std::MP3);
            MP3.Activate();
        }

        if(handfree.Touched()) {
            fireEvent(HANDS_FREE);
            handfree.Activate();
        }

        if(AM_FM.Touched()) {
            fireEvent(std::AM_FM);
            AM_FM.Activate();
        }

        if(make_call.Touched()) {
            fireEvent(std::MAKE_CALL);
            make_call.Activate();
        }

        ThisThread::sleep_for(100ms);
    }
}
