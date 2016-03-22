#include <zmq.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <signal.h>
#include <protocol.h>

std::shared_ptr<zmq::context_t> context;
std::vector<TProcId> pidList;

#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds
const long INTERVAL_MS = 5 * NANO_SECOND_MULTIPLIER;
void avoidBusyWait(const long nsec = INTERVAL_MS)
{
    timespec tim;
    tim.tv_sec  = 0;
    tim.tv_nsec = nsec;
    nanosleep(&tim, NULL);
}

void removePid(TProcId pid)
{
    for (auto iter = pidList.begin(); iter != pidList.end(); ++iter) {
        if (pid == *iter) {
            pidList.erase(iter);
            return;
        }
    }
}

void* processMonitor(void *ptr)
{
    std::shared_ptr<zmq::socket_t> appServerSocket = std::make_shared<zmq::socket_t>(*context.get(), ZMQ_REQ);
    appServerSocket->connect("tcp://localhost:9000");
    
    while (true) {
        avoidBusyWait();
        for (int i = 0; i < pidList.size(); i++) {
            TProcId pid = pidList.at(i);
            if (kill(pid, 0) == -1) {
                try {
                    removePid(pid);
                    
                    std::cout << "Removed pid " << pid << std::endl;
                    
                    Asp_Request unsubscribeReq;
                    unsubscribeReq.type = AspRequestUnregister;
                    unsubscribeReq.field0 = pid;
                    zmq::message_t request(&unsubscribeReq, sizeof(Asp_Request));
                    appServerSocket->send(request);
                    
                    // ACK
                    int ack = 0;
                    size_t receivedSize = appServerSocket->recv(&ack, sizeof(int));
                    if (receivedSize <= 0 || ack != 1) {
                        exit(1);
                    }
                    
                    std::cout << "Told appserver to remove pid " << pid << std::endl;
                }
                catch (zmq::error_t e) {
                    std::cout << __func__ << ": " << e.what() << std::endl;
                }
            }
        }
    }
    return nullptr;
}

int main(int argc, char **argv)
{
    context = std::make_shared<zmq::context_t>(1);
    std::shared_ptr<zmq::socket_t> socket = std::make_shared<zmq::socket_t>(*context.get(), ZMQ_REP);
    socket->bind("tcp://*:9001");

    pthread_t thread;
    if (pthread_create(&thread, NULL, processMonitor, NULL)) {
        exit(1);
    }
    
    while (true) {
        try {
            TProcId pid = -1;
            socket->recv(&pid, sizeof(TProcId));
            
            // ACK
            int ack = 1;
            zmq::message_t ackResponse(&ack, sizeof(int));
            socket->send(ackResponse);
            
            if (pid != -1) {
                pidList.push_back(pid);
                std::cout << "Registered new pid " << pid << std::endl;
            }
        }
        catch (zmq::error_t e) {
            std::cout << __func__ << ": " << e.what() << std::endl;
        }
    }

    return 0;
}
