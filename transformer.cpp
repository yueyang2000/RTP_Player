#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
#include <cctype>
#include <set>
using namespace std;
# pragma pack (1)

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




/* 
void show_header(RTP_header* header){
    short V = (header->V_P_X_CC & 0xc0)>>6;
    short P = (header->V_P_X_CC &0x20)>>5;
    short X = (header->V_P_X_CC & 0x10)>>4;
    short CC = (header->V_P_X_CC & 0x0e);
    short seq_num = Tranverse16(header->sequence_number);
    //cout<<V<<' '<<P<<' '<<X<<' '<<' '<<CC<<seq_num<<endl;
    //cout<<(int)header->V_P_X_CC<<' '<<(int)header->MP_T<<' '<<(int)header->sequence_number<<endl;
}*/


void hex_to_bin(string& hex,  char* bin)
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
        bin[j] = (char) ans;
        j++;
    }
}

int main()
{
    ifstream fin;
    fin.open("control.txt");
    FILE* fout;
    fout = fopen("control.264","wb");
    ofstream log;
    log.open("log.txt");
    
    int counter = 0;
    string packet;
    set<int> seq;
    bool start = false;
    int max = -1;

    int count = 0;

    while(getline(fin,packet))
    {
        //getline(fin,packet);
        char buffer[2048]="";
        hex_to_bin(packet,buffer);
        
        int pos = 0;
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
        if(seq.count(seq_num)){continue;}
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
            while(pos<packet.size()/2)
            {
                unsigned short NALU_size;
                memcpy(&NALU_size,buffer+pos,2);
                NALU_size = Tranverse16(NALU_size);
                pos+=2;
                char NAL_header = buffer[pos];
                short NAL_type = NAL_header & 0x1f;
                cout<<NAL_type;
                if(NAL_type == 7){
                    count++;
                    cout<<"SPS, sequence number: "<<seq_num<<endl;
                }
                else if(NAL_type == 8){
                    cout<<"PPS, sequence number: "<<seq_num<<endl;
                }
                else if(NAL_type == 10){
                    cout<<"end of sequence, sequence number: "<<seq_num<<endl;
                }
                //cout<<pos<<endl;
                if(SECTION){
                    fwrite(&inteval,4,1,fout);
                    fwrite(&buffer[pos],NALU_size,1,fout);
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
                cout<<"SPS, sequence number: "<<seq_num<<endl;
            }
            else if(NAL_type == 8){
                cout<<"PPS, sequence number: "<<seq_num<<endl;
            }
            else if(NAL_type == 10){
                cout<<"end of sequence, sequence number: "<<seq_num<<endl;
            }


            pos++;
            if(S){
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
                }

            int size = packet.size()/2 - pos;
            if(SECTION)
            {
                if(S){
                    fwrite(&inteval,4,1,fout);
                    fwrite(&NAL_header,1,1,fout);
                    fwrite(buffer + pos, size,  1, fout);
                }
                else
                {
                    fwrite(buffer + pos, size,  1, fout);
                }
            }
        }
        else{
            //cout<<"NAL UNIT\n";
            //cout<<"other type:"<<type<<endl;
            short NAL_type = type;
            if(NAL_type == 7){
                count++;
                cout<<"SPS, sequence number: "<<seq_num<<endl;
            }
            else if(NAL_type == 8){
                cout<<"PPS, sequence number: "<<seq_num<<endl;
            }
            else if(NAL_type == 10){
                cout<<"end of sequence, sequence number: "<<seq_num<<endl;
            }


            int size = packet.size()/2 - pos;
            if(SECTION)
            {
                fwrite(&inteval,4,1,fout);
                fwrite(buffer + pos -1, size + 1,  1, fout);//要加上头部
            }
            //cout<<"size: "<<size<<endl;
            //cout<<"pos: "<<pos<<endl;
        }
        
        counter++;
        //if(counter == 30)
        //    break;
    }
    fin.close();
    fclose(fout);
    log.close();
    return 0;
}

/*
debug log:
seq number 1~9 went pretty well
things started to get wired when facing STAP packet with multipule NAL Units
namely seq_num 11

bug is fixed
now we can get raw .264 file
however we cannot play it...

 */