#include <zmq.hpp>
#include <memory>
#include <vector>
#include <sstream>
#include <iostream>
#include <thread>
#include <signal.h>
#include <protocol.h>

struct App
{
    TProcId pid;
};

std::vector<App> appList;
std::string gAppServerAddress;

#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds
const long INTERVAL_MS = 5 * NANO_SECOND_MULTIPLIER;
void avoidBusyWait(const long nsec = INTERVAL_MS)
{
#ifdef _WIN32
    Sleep(nsec / NANO_SECOND_MULTIPLIER);
#else
    timespec tim;
    tim.tv_sec  = 0;
    tim.tv_nsec = nsec;
    nanosleep(&tim, NULL);
#endif
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

bool sendAck(std::shared_ptr<zmq::socket_t> socket)
{
    int ack = 1;
    zmq::message_t msg(&ack, sizeof(uint8_t));
    return socket->send(msg);
}

bool recvAck(std::shared_ptr<zmq::socket_t> socket)
{
    int ack = 0;
    size_t receivedSize = socket->recv(&ack, sizeof(uint8_t));
    if (receivedSize <= 0 || ack != 1) {
        return false;
    }
    return true;
}

#ifdef _WIN32
uint8_t kill(uint32_t pid, uint8_t unused)
{
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
    DWORD status;
    GetExitCodeProcess(hProc, &status);
    if (status == STILL_ACTIVE)
        return 0;
    return -1;
}
#endif

void* processMonitor(void *ptr)
{
    std::shared_ptr<zmq::context_t> context = std::make_shared<zmq::context_t>();
    std::shared_ptr<zmq::socket_t> appServerSocket = std::make_shared<zmq::socket_t>(*context.get(), ZMQ_REQ);
    std::stringstream appServerAddr;
    appServerAddr << "tcp://" << gAppServerAddress << ":9000";
    appServerSocket->connect(appServerAddr.str());
    std::cout << "Connected to appserver on " << appServerAddr.str() << std::endl;
    
    while (true) {
        avoidBusyWait();
        for (int i = 0; i < appList.size(); i++) {
            App app = appList.at(i);
            if (kill(app.pid, 0) == -1) {
                try {
                    removePid(app.pid);                                        
                    
                    // Ask the appserver's to unregister the app
                    Asp_Request req1;
                    req1.type = AspRequestUnregister;
                    req1.field0 = app.pid;
                    zmq::message_t msg1(&req1, sizeof(Asp_Request));
                    appServerSocket->send(msg1);
                    
                    TAppId appId = 0;
                    appServerSocket->recv(&appId, sizeof(TAppId));

                    // Ask the appserver to stop the app's requester listener thread
                    std::shared_ptr<zmq::socket_t> reqSocket = std::make_shared<zmq::socket_t>(*context.get(), ZMQ_REQ);
                    std::stringstream reqSocketAddress;
                    reqSocketAddress << "tcp://" << gAppServerAddress << ":";
                    int reqSocketPort = AspReqListenerThreadPortValue + appId;
                    reqSocketAddress << reqSocketPort;
                    reqSocket->connect(reqSocketAddress.str());

                    Asp_Request req2;
                    req2.type = AspRequestUnregister;
                    zmq::message_t msg2(&req2, sizeof(Asp_Request));
                    reqSocket->send(msg2);

                    std::cout << "Removed app with pid=" << app.pid << " appid=" << appId << std::endl;
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
    gAppServerAddress = argc > 1 ? argv[1] : "localhost";

    std::shared_ptr<zmq::context_t> context = std::make_shared<zmq::context_t>();
    std::shared_ptr<zmq::socket_t> socket = std::make_shared<zmq::socket_t>(*context.get(), ZMQ_REP);
    socket->bind("tcp://*:9001");

    std::thread thread(processMonitor, nullptr);
    
    bool run = true;
    while (run) {
        try {
            Asp_SubscribeRequest subscribeReq;
            socket->recv(&subscribeReq, sizeof(Asp_SubscribeRequest));

            // Send appserver address's size
            size_t addressSize = gAppServerAddress.size();
            zmq::message_t msg1(&addressSize, sizeof(size_t));
            socket->send(msg1);

            recvAck(socket);

            // Send the actuall appserver's address (localhost or an IPv4 address)
            void *data = (void*)gAppServerAddress.c_str();
            zmq::message_t msg2(data, addressSize);
            socket->send(msg2);

            // Add app to the list
            App app;
            app.pid = subscribeReq.pid;
            appList.push_back(app);
            std::cout << "Registered new app with pid:" << app.pid << std::endl;
        }
        catch (zmq::error_t e) {
            std::cout << __func__ << ": " << e.what() << std::endl;
            exit(1);
        }
    }

    thread.join();

    return 0;
}
