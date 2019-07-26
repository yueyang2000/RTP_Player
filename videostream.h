#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

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

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QImage>
#include <QPushButton>
#include "packet.h"
#include <QThread>
#include "recv.h"

namespace Ui {
class VideoStream;
}

class VideoStream : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoStream(QWidget *parent = nullptr);
    ~VideoStream();
    void setUrl(QString url);



private:
    QMutex mutex;
    AVPicture  pAVPicture;
    AVFormatContext *pAVFormatContext;
    AVCodecContext *pAVCodecContext;
    AVFrame *pAVFrame;
    SwsContext * pSwsContext;
    AVPacket pAVPacket;

    int videoStreamIndex;
    int videoWidth;
    int videoHeight;

    QString m_str_url;
    QTimer* m_timerPlay;
    QLabel* m_label;
    QPushButton* btnStart;
    QPushButton* btnStop;
    int m_i_frameFinished;
    bool init();
    bool playing = false;

    PacketToNALU* manager;
    Ui::VideoStream *ui;

    Recv_package recv;



private slots:
    void SetImageSlots(const QImage &image);
    void playSlots();
    void startStream();
    void stopStream();
signals:
    void GetImage(QImage);
};

#endif // VIDEOSTREAM_H
