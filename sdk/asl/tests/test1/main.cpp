#include <iostream>
#include <pthread.h>
#include <Asl/CApi.h>
#include <protocol.h>

int main(int argc, char *argv[])
{
    aslInit();
    aslSubscribe();

    AslEvent event;
    while (true) {        
        event = aslWaitEvent();
        
        if (event.type == AspEventMouseInput) {       
            if (event.inputEvent.mouseEvent.type == AspMouseEventPress) {
            }
            else if (event.inputEvent.mouseEvent.type == AspMouseEventRelease) {
            }
            else if (event.inputEvent.mouseEvent.type == AspMouseEventMove) {
            }            
        }
    }

    return 0;
}
