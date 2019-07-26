#include "recv.h"
#include <string>
#include <fstream>
#include <QDebug>
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
    m_start = true;
    ifstream fin;
    fin.open("/Users/yueyang/yiqunyang/大一暑假/科研/control.txt");
    while(m_start)
    {
        qDebug()<<"get a packet\n";
        Data temp;
        string str;
        getline(fin,str);
        temp.data = new unsigned char[str.size()/2];
        hex_to_bin(str,temp.data);
        temp.size = str.size()/2;

        mutex->lock();
        manager->InputPacket(temp);
        mutex->unlock();
    }
}
