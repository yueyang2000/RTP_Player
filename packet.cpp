#include "packet.h"
#include <QDebug>
#define Tranverse16(X)                 ((((short)(X) & 0xff00) >> 8) |(((short)(X) & 0x00ff) << 8))
struct RTP_header
{
    char V_P_X_CC;//协议版本号V，填充标志P，扩展标志X
    char MP_T;//标记和有效载荷类型
    short sequence_number;//序列号
    int time_stamp;//时间戳
    int SSRC;
};
const int inteval = 0x01000000;//输出方式很迷？？
# define SECTION  count>0
void PacketToNALU::InputPacket(Data packet)
{
    //getline(fin,packet);
    unsigned char* buffer = packet.data;

    int pos = 0;
    static int count = 0;
    //read rtp header
    RTP_header* header = new RTP_header;
    memcpy(header,buffer,sizeof(RTP_header));
    pos+=12;
    //cout<<counter<<endl;
    short V = (header->V_P_X_CC & 0xc0)>>6;
    short P = (header->V_P_X_CC &0x20)>>5;
    short X = (header->V_P_X_CC & 0x10)>>4;
    short CC = (header->V_P_X_CC & 0x0e);
    short seq_num = Tranverse16(header->sequence_number);
    //log<<seq_num<<endl;
    //去重
    if(seq.count(seq_num)) return;
    else{
        seq.insert(seq_num);
        //log<<seq_num<<endl;
        }
    //cout<<"Sequence Number: "<<seq_num<<endl;


    //show_header(header);
    if(X){
        //profile extension
        short define;
        short length;
        length =  buffer[pos+3];//suppose not so long extension
        pos+=4;
        pos += (length * 4);
        //cout<<"extension length: "<<length<<endl;
        //cout<<pos<<endl;
    }

    //payload header
    char payload_header = buffer[pos];
    short type = payload_header & 0x1f;//后五位
    pos++;
    if(type == 24){
        //cout<<"STAP-A\n";
        while(pos<packet.size)
        {
            unsigned short NALU_size;
            memcpy(&NALU_size,buffer+pos,2);
            NALU_size = Tranverse16(NALU_size);
            pos+=2;
            char NAL_header = buffer[pos];
            short NAL_type = NAL_header & 0x1f;
            //cout<<NAL_type;
            if(NAL_type == 7){
                count++;
                //cout<<"SPS, sequence number: "<<seq_num<<endl;
            }
            else if(NAL_type == 8){
                //cout<<"PPS, sequence number: "<<seq_num<<endl;
            }
            else if(NAL_type == 10){
                //cout<<"end of sequence, sequence number: "<<seq_num<<endl;
            }
            //cout<<pos<<endl;
            if(SECTION){
                //fwrite(&inteval,4,1,fout);
                //fwrite(&buffer[pos],NALU_size,1,fout);
                Data d;
                d.data = new unsigned char[NALU_size+4];
                d.size = NALU_size+4;
                memcpy(d.data,&inteval,4);
                memcpy(d.data+4,&buffer[pos],NALU_size);
                q.push(d);
            }

            pos+=NALU_size;
        }
    }
    else if(type == 28)
    {
        //cout<<"FU-A\n";
        //FU header
        char FU_header = buffer[pos];
        bool S = FU_header & 0x80;
        bool E = FU_header & 0x40;

        //重建NAL header
        char NAL_header = (payload_header & 0xe0) | (FU_header & 0x1f);
        short NAL_type = FU_header & 0x1f;
        if(NAL_type == 7){
            count++;
            //cout<<"SPS, sequence number: "<<seq_num<<endl;
        }
        else if(NAL_type == 8){
            //cout<<"PPS, sequence number: "<<seq_num<<endl;
        }
        else if(NAL_type == 10){
            //cout<<"end of sequence, sequence number: "<<seq_num<<endl;
        }


        pos++;
        /*if(S){
            //log<<"start\n";
            start = true;
            }
        else if(E){
            //log<<"end\n";
            //if(start==false){log<<seq_num<<"error!\n";}
            start = false;
            }
        else{
            //log<<"middle\n";
            //if(start==false){log<<seq_num<<"error!\n";}
            }*/

        int size = packet.size - pos;
        if(SECTION)
        {
            if(S){
                //fwrite(&inteval,4,1,fout);
                //fwrite(&NAL_header,1,1,fout);
                //fwrite(buffer + pos, size,  1, fout);
                if(FU_buffer.size!=0){delete []FU_buffer.data;FU_buffer.size = 0;}
                FU_buffer.data = new unsigned char[size+5];
                FU_buffer.size = size+5;
                memcpy(FU_buffer.data,&inteval,4);
                memcpy(FU_buffer.data+4,&NAL_header,1);
                memcpy(FU_buffer.data+5,buffer+pos,size);
            }
            else
            {
                //fwrite(buffer + pos, size,  1, fout);
                unsigned char* temp = new unsigned char [FU_buffer.size+size];
                memcpy(temp,FU_buffer.data,FU_buffer.size);
                memcpy(temp+FU_buffer.size,buffer+pos,size);
                if(FU_buffer.size != 0) delete []FU_buffer.data;
                FU_buffer.data = temp;
                FU_buffer.size += size;
            }
            if(E){
                Data d;
                d.size = FU_buffer.size;
                d.data = new unsigned char[d.size];
                memcpy(d.data,FU_buffer.data,d.size);
                q.push(d);
                delete []FU_buffer.data;
                FU_buffer.size = 0;
            }
        }
    }
    else{
        //cout<<"NAL UNIT\n";
        //cout<<"other type:"<<type<<endl;
        short NAL_type = type;
        if(NAL_type == 7){
            count++;
            //cout<<"SPS, sequence number: "<<seq_num<<endl;
        }
        else if(NAL_type == 8){
            //cout<<"PPS, sequence number: "<<seq_num<<endl;
        }
        else if(NAL_type == 10){
            //cout<<"end of sequence, sequence number: "<<seq_num<<endl;
        }


        int size = packet.size - pos;
        if(SECTION)
        {
            //fwrite(&inteval,4,1,fout);
            //fwrite(buffer + pos -1, size + 1,  1, fout);//要加上头部
            Data d;
            d.data = new unsigned char[size+5];
            d.size = size+5;
            memcpy(d.data,&inteval,4);
            memcpy(d.data+4,buffer+pos-1,size+1);
            q.push(d);
        }
        //cout<<"size: "<<size<<endl;
        //cout<<"pos: "<<pos<<endl;
    }
    qDebug()<<"receive a packet, save success!";
}

Data PacketToNALU::OutputNALU()
{
   Data tmp = q.front();
   q.pop();
   qDebug()<<"return a NAL unit!";
   return tmp;
}
