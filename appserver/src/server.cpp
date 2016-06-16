//
//  server.cpp
//  appserver
//
//  Created by Anass on 09/09/14.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <signal.h>
#include <server.h>
#include <sdlcompositor.h>
#include <sdleventsource.h>
#include <minilzo/minilzo.h>
#include <QApplication>
#include <QBackingStore>
#include <QPaintDevice>
#include <QPainter>
#include <qtwindow.h>

using namespace appserver;

Server* Server::_sharedInst = nullptr;

Server::Server()
{
    _context = std::make_shared<zmq::context_t>();
    _socket = std::make_shared<zmq::socket_t>(*_context.get(), ZMQ_REP);
    int serverSocketTimeout = 10000;
    _socket->setsockopt(ZMQ_SNDTIMEO, serverSocketTimeout);
    _socket->setsockopt(ZMQ_RCVTIMEO, serverSocketTimeout);
    _socket->bind("tcp://*:9000");
}

void* Server::requestListener(void* ptr)
{
    Server* server = Server::getSingleton();
    std::shared_ptr<zmq::socket_t> socket = server->getSocket().lock();
    for (;;) {
        try {
            Asp_Request req;
            size_t receivedSize = socket->recv(&req, sizeof(Asp_Request));
            if (receivedSize > 0) {
                if (req.type == AspRequestRegister) {
                    //emit server->needToCreateWindow();
                    std::shared_ptr<App> app = std::make_shared<App>(req.field0);
                    app->startRequestListener();

                    // Send the client ID back to the app
                    Asp_Event evt;
                    evt.field0 = app->getId();
                    zmq::message_t response(&evt, sizeof(Asp_Event));
                    socket->send(response);

                    server->addApp(app);
                }
                else if (req.type == AspRequestUnregister) {
                    TAppId appId = server->removeAppByPid(req.field0);

                    zmq::message_t appIdMsg(&appId, sizeof(TAppId));
                    socket->send(appIdMsg);

                    std::cout << "Removed app with pid " << req.field0 << std::endl;
                }
            }
        }
        catch (zmq::error_t e) {
            std::cout << __func__ << ": " << e.what() << std::endl;
        }
    }
    return nullptr;
}

void Server::createWindow(quint32 id, Rect* frame, int rasterType, bool visible)
{
    QtWindow* window = new QtWindow(id, *frame, rasterType, visible);
    window->setGeometry(frame->location.x, frame->location.y, frame->size.width, frame->size.height);
    window->show();

    QBackingStore *m_backingStore = new QBackingStore(window);

    QRect rect(0, 0, window->width(), window->height());
    m_backingStore->beginPaint(rect);

    QPaintDevice *device = m_backingStore->paintDevice();
    QPainter painter(device);

    painter.fillRect(0, 0, width(), height(), Qt::white);
    painter.drawText(QRectF(0, 0, window->width(), window->height()), Qt::AlignCenter, QStringLiteral("QWindow"));

    m_backingStore->endPaint();
    m_backingStore->flush(rect);
}

void Server::run(int argc, char** argv)
{
    if (lzo_init() != LZO_E_OK) {
        printf("internal error - lzo_init() failed !!!\n");
        printf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
    }

    setWindowTitle("appserver");
    resize(400, 300);
    show();

    connect(this, SIGNAL(needToCreateWindow(quint32, Rect*, int, bool)), this, SLOT(createWindow(quint32, Rect*, int, bool)));
    //connect(this, SIGNAL(needToCreateWindow(TWindowId, Rect*, int, bool)), this, SLOT(createWindow(TWindowId, Rect*, int, bool)));
    //connect(this, SIGNAL(needToCreateWindow()), this, SLOT(createWindow()));

    _messageDispatcher = std::make_shared<std::thread>(Server::requestListener, nullptr);

    //_compositor = std::make_shared<SDLCompositor>();
    //_inputSource = std::make_shared<SDLEventSource>();
    //_windowManager = std::make_shared<WindowManager>();

    /*while (true) {
        _compositor->compose();

        std::shared_ptr<Event> evt = _inputSource->pollEvent();
        if (evt != nullptr) {
            uint8_t eventType = evt->getType();
            if (eventType == AspEventTypeQuit) {
                break;
            }
            else {
                _windowManager->sendEvent(evt);
            }
        }

        Server::avoidBusyWait(10 * NANO_SECOND_MULTIPLIER);
    }*/
}

void Server::avoidBusyWait(const long int nsec)
{
#ifdef _WIN32
    Sleep(nsec / NANO_SECOND_MULTIPLIER);
#else
    timespec tim;
    tim.tv_sec = 0;
    tim.tv_nsec = nsec;
    nanosleep(&tim, NULL);
#endif
}

void Server::addApp(std::shared_ptr<App> app)
{
    _apps.push_back(app);
}

TAppId Server::removeAppByPid(TProcId pid)
{
    for (auto iter = _apps.begin(); iter != _apps.end(); ++iter) {
        std::shared_ptr<App> a = *iter;
        if (a->getPid() == pid) {
            TAppId id = a->getId();
            _compositor->removeWindows(id);
            _apps.erase(iter);
            return id;
        }
    }
    return 0;
}

std::weak_ptr<App> Server::findApp(TAppId id) const
{
    for (auto iter = _apps.begin(); iter != _apps.end(); ++iter) {
        std::shared_ptr<App> app = *iter;
        if (app->getId() == id) {
            return app;
        }
    }
    return std::weak_ptr<App>();
}

std::weak_ptr<Compositor> Server::getCompositor() const
{
    return _compositor;
}

std::weak_ptr<WindowManager> Server::getWindowManager() const
{
    return _windowManager;
}

std::weak_ptr<zmq::context_t> Server::getSocketContext() const
{
    return _context;
}

std::weak_ptr<zmq::socket_t> Server::getSocket() const
{
    return _socket;
}

Server* Server::getSingleton()
{
    if (_sharedInst == nullptr) {
        _sharedInst = new Server;
    }
    return _sharedInst;
}

void Server::setAppsHost(std::string host)
{
    _appsHost = host;
}

std::string Server::getAppsHost() const
{
    return _appsHost;
}

Server::~Server()
{
}
