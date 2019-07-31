#include "recv.h"
#include "packet.h"
#include <string>
#include <fstream>
#include <QDebug>
#include "videostream.h"
#define SOCKET
using namespace std;


void hex_to_bin(string& hex,  unsigned char* bin)
{
    int j = 0;
    for(int i = 0; i<hex.size();i+=2){
        int ans = 0;
        if(isdigit(hex[i])){
            ans += 16*(hex[i] - '0');
        }
        else{
            ans += 16*(hex[i] - 'a' + 10);
        }
        if(isdigit(hex[i+1])){
            ans += (hex[i+1] - '0');
        }
        else{
            ans += (hex[i+1] - 'a' + 10);
        }
        bin[j] = (unsigned char) ans;
        j++;
    }
}

void Recv_package::run()
{
    qDebug()<<"recv running!\n";
#ifdef FILE
    //from file stream
    ifstream fin;
    fin.open("/Users/yueyang/yiqunyang/大一暑假/科研/control.txt");
    string str;
    while(getline(fin,str))
    {
        //qDebug()<<"get a packet";
        Data temp;
        temp.data = new unsigned char[str.size()/2];
        hex_to_bin(str,temp.data);
        temp.size = str.size()/2;

        manager->InputPacket(temp);
        delete []temp.data;


    }
    fin.close();
#endif

#ifdef SOCKET
    //socket
    socket = new QUdpSocket();
    port = 2002;
    bool result = socket->bind(port);
    if(!result){
        qDebug()<<"bind failed";
    }
    else {
        qDebug()<<"bind succeed";
    }

   //from socket
    while(true){
    if(socket->hasPendingDatagrams())
    {
        qDebug()<<"receive datagram!";
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());

        socket->readDatagram(datagram.data(),datagram.size());
        Data temp;
        temp.size = datagram.size();
        temp.data = new unsigned char[temp.size];
        memcpy(temp.data,datagram.data(),temp.size);

        //m.lock();
        manager->InputPacket(temp);
        delete []temp.data;
        //m.unlock();
    }
    }

#endif
}

