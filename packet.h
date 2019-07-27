#ifndef PACKET_H
#define PACKET_H
#include <queue>
#include <set>
#include <QPixmap>
#include <QImage>
#include <QTimer>
#include <QMutex>
//必须加以下内容,否则编译不能通过,为了兼容C和C99标准

#ifndef INT64_C
#define INT64_C
#define UINT64_C
#endif

//引入ffmpeg头文件
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#include <libavutil/pixfmt.h>
}

struct Data{
    unsigned char* data;
    int size;
};

class Manager
{
    std::queue<Data> q;
    Data FU_buffer;
    std::set<int> seq;
    std::queue<QPixmap> pics;

    AVPicture  pAVPicture;
    AVFormatContext *pAVFormatContext;
    AVCodecContext *pAVCodecContext;
    AVFrame *pAVFrame;
    SwsContext * pSwsContext;
    AVPacket pAVPacket;
    AVCodecParserContext* pAVCodecParserContext;
    QMutex mutex;
    int result = 0;

public:
    Manager();
    void InputPacket(Data packet);  //传入一个包
    void InputNALU();
    bool Outputpic(QPixmap&);  //返回一张图
    int size(){return q.size();}
    void stop();
    QMutex* get_mutex(){return &mutex;}
};
extern QMutex m;


#endif // PACKET_H
