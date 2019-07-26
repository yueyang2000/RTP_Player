#ifndef RECV_H
#define RECV_H

#include <QThread>
#include <QUdpSocket>
#include "packet.h"
class Recv_package : public QThread
{
    Q_OBJECT
private:
    QUdpSocket* socket;
    PacketToNALU* manager;
    QMutex* mutex;
    bool m_start = false;
protected:
    void run() Q_DECL_OVERRIDE;
public:
    Recv_package(){}
    void init(QUdpSocket* so,PacketToNALU* ma ,QMutex* mu){
        socket = so;
        manager = ma;
        mutex = mu;
    }
    void stop(){m_start = false;}
    void start(){m_start = true;}
};

#endif // RECV_H
