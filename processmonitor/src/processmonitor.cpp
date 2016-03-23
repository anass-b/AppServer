#include <zmq.hpp>
#include <memory>
#include <vector>
#include <sstream>
#include <iostream>
#include <pthread.h>
#include <signal.h>
#include <protocol.h>

struct App
{
    TProcId pid;
    TAppId id;
};

std::shared_ptr<zmq::context_t> context;
std::vector<App> appList;

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
    for (auto iter = appList.begin(); iter != appList.end(); ++iter) {
        App app = *iter;
        if (app.pid == pid) {
            appList.erase(iter);
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
        for (int i = 0; i < appList.size(); i++) {
            App app = appList.at(i);
            if (kill(app.pid, 0) == -1) {
                try {
                    removePid(app.pid);
                    
                    std::cout << "Removed app with pid " << app.pid << std::endl;
                    
                    // Ask the appserver's to unregister the app
                    Asp_Request unregisterReq1;
                    unregisterReq1.type = AspRequestUnregister;
                    unregisterReq1.field0 = app.pid;
                    zmq::message_t zmqUnregisterReq1(&unregisterReq1, sizeof(Asp_Request));
                    appServerSocket->send(zmqUnregisterReq1);
                    
                    // ACK
                    int ack = 0;
                    size_t receivedSize = appServerSocket->recv(&ack, sizeof(int));
                    if (receivedSize <= 0 || ack != 1) {
                        exit(1);
                    }
                    
                    std::cout << "Told appserver to remove the app with pid " << app.pid << std::endl;

                    // Ask the appserver to stop the app's requester listener thread
                    std::shared_ptr<zmq::socket_t> reqSocket = std::make_shared<zmq::socket_t>(*context.get(), ZMQ_REQ);
                    std::stringstream reqSocketAddress;
                    reqSocketAddress << "tcp://localhost:";
                    int reqSocketPort = AspReqListenerThreadPortValue + app.id;
                    reqSocketAddress << reqSocketPort;
                    reqSocket->connect(reqSocketAddress.str());

                    Asp_Request unregisterReq2;
                    unregisterReq2.type = AspRequestUnregister;
                    zmq::message_t zmqUnregisterReq2(&unregisterReq2, sizeof(Asp_Request));
                    reqSocket->send(zmqUnregisterReq2);
                    std::cout << "Told server to stop the request listener thread of the app with pid " << app.pid << std::endl;
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
            Asp_SubscribeRequest subscribeReq;
            socket->recv(&subscribeReq, sizeof(subscribeReq));
            
            // ACK
            int ack = 1;
            zmq::message_t ackResponse(&ack, sizeof(int));
            socket->send(ackResponse);
            
            App app;
            app.pid = subscribeReq.pid;
            app.id = subscribeReq.clientId;
            appList.push_back(app);
            std::cout << "Registered new app with pid:" << app.pid << " clientId:" << app.id << std::endl;
        }
        catch (zmq::error_t e) {
            std::cout << __func__ << ": " << e.what() << std::endl;
        }
    }

    return 0;
}
