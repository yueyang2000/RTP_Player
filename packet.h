#ifndef PACKET_H
#define PACKET_H
#include <queue>

struct Data{
    char* data;
    int size;
};

class PacketToNALU{
    std::queue<Data> StoreNALU;
    bool unitFinished;
    int CountSequenceNumber;

public:
    PacketToNALU():unitFinished(true), CountSequenceNumber(-1){ }
    void InputPacket(Data packet);  //传入一个包
    Data OutputNALU();  //返回一个NALU
};






#endif // PACKET_H
