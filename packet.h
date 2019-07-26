#ifndef PACKET_H
#define PACKET_H
#include <queue>
#include <set>
struct Data{
    unsigned char* data;
    int size;
};

class PacketToNALU
{
    std::queue<Data> q;
    Data FU_buffer;
    std::set<int> seq;

public:
    PacketToNALU(){}
    void InputPacket(Data packet);  //传入一个包
    Data OutputNALU();  //返回一个NALU
};






#endif // PACKET_H
