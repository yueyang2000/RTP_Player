#ifndef RECV_H
#define RECV_H

#include <QThread>
#include <QUdpSocket>
#include "packet.h"
class Recv_package: public QThread
{
private:
    QUdpSocket* socket;
    PacketToNALU* manager;
    QMutex* mutex;
    bool m_start = false;

protected:
    virtual void run();

public:
    Recv_package(){}
    void init(QUdpSocket* so,PacketToNALU* ma ,QMutex* mu){
        socket = so;
        manager = ma;
        mutex = mu;
    }
};

#endif // RECV_H
#ifndef RECV_H
#define RECV_H

#endif // RECV_H
