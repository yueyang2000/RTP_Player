#ifndef RECV_H
#define RECV_H

#include <QThread>
#include <QUdpSocket>
#include "packet.h"
class Recv_package: public QThread
{
private:
    QUdpSocket* socket;
    Manager* manager;
    bool m_start = false;

protected:
    virtual void run();

public:
    Recv_package(){}
    void init(QUdpSocket* so,Manager* ma ){
        socket = so;
        manager = ma;
    }
};

#endif // RECV_H
#ifndef RECV_H
#define RECV_H

#endif // RECV_H
