#include "videostream.h"
#include "ui_videostream.h"

#include <QDebug>
#include <QDateTime>
#include <QLayout>

VideoStream::VideoStream(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VideoStream)
{
    manager = new PacketToNALU;

    m_label = new QLabel;
    btnStart = new QPushButton;
    btnStart->setText("start");
    btnStop = new QPushButton;
    btnStop->setText("Stop");
    m_timerPlay = new QTimer;
    m_timerPlay->setInterval(40);
    m_i_frameFinished = 0;
    //关联信号与槽
    connect(m_timerPlay,SIGNAL(timeout()),this,SLOT(playSlots()));
    connect(this,SIGNAL(GetImage(QImage)),this,SLOT(SetImageSlots(QImage)));
    connect(btnStart,SIGNAL(clicked()),this, SLOT(startStream()));
    connect(btnStop,SIGNAL(clicked()),this,SLOT(stopStream()));
    setUrl("/Users/yueyang/yiqunyang/大一暑假/科研/control.264");
    ui->setupUi(this);
    QVBoxLayout* lay = new QVBoxLayout;
    lay->addWidget(m_label);
    lay->addWidget(btnStart);
    lay->addWidget(btnStop);
    ui->centralWidget->setLayout(lay);
    //qDebug()<<"construct successful\n";
}

VideoStream::~VideoStream()
{
    delete ui;
}

void VideoStream::setUrl(QString url)
{
    m_str_url = url;
}

void VideoStream::startStream()
{
    if(playing == true) return;
    playing = true;
    videoStreamIndex=-1;
    av_register_all();//注册库中所有可用的文件格式和解码器
    avformat_network_init();//初始化网络流格式,使用RTSP网络流时必须先执行
    pAVFormatContext = avformat_alloc_context();//申请一个AVFormatContext结构的内存,并进行简单初始化
    pAVFrame=av_frame_alloc();

    if (this->init())
    {
        m_timerPlay->start();
    }
}

void VideoStream::stopStream()
{
    if(playing == false) return;
    playing = false;
    m_timerPlay->stop();

    avformat_free_context(pAVFormatContext);
    av_frame_free(&pAVFrame);
    sws_freeContext(pSwsContext);
}

bool VideoStream::init()
{
    if(m_str_url.isEmpty())
           return false;
       /*打开视频流
       int result=avformat_open_input(&pAVFormatContext, m_str_url.toStdString().c_str(),NULL,NULL);
       if (result<0){
           qDebug()<<"打开视频流失败";
           return false;
       }

       //获取视频流信息
       result=avformat_find_stream_info(pAVFormatContext,NULL);
       if (result<0){
           qDebug()<<"获取视频流信息失败";
           return false;
       }

       获取视频流索引
       videoStreamIndex = -1;
       for (uint i = 0; i < pAVFormatContext->nb_streams; i++) {
           if (pAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
               videoStreamIndex = i;
               break;
           }
       }
      //videoStreamIndex = 0

       if (videoStreamIndex==-1){
           qDebug()<<"获取视频流索引失败";
           return false;
       }

       //获取视频流的分辨率大小
       pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
       videoWidth=pAVCodecContext->width;//1280
       videoHeight=pAVCodecContext->height;//720
        */
       avpicture_alloc(&pAVPicture,AV_PIX_FMT_RGB24,1280,720);
        //照抄
       AVCodec *pAVCodec;

       //获取视频流解码器
       pAVCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
       pAVCodecContext = avcodec_alloc_context3(pAVCodec);

       pSwsContext = sws_getContext(1280,720,AV_PIX_FMT_YUV420P,1280,720,AV_PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);
        //这段照搬
       //打开对应解码器
       avcodec_open2(pAVCodecContext,pAVCodec,NULL);
       /*if (result<0){
           qDebug()<<"打开解码器失败";
           return false;
       }*/

       qDebug()<<"初始化视频流成功";
       return true;
}

void VideoStream::SetImageSlots(const QImage &image)
{
    if (image.height()>0){
        QPixmap pix = QPixmap::fromImage(image.scaled(600,400));
        m_label->setPixmap(pix);
        //m_label->setText("hello!!!");
    }
}
/*
void VideoStream::playSlots()
{
    //一帧一帧读取视频
    //需要一些构建pAVFormatContext或者pAVPacket的知识
    if (av_read_frame(pAVFormatContext, &pAVPacket) >= 0){

        if(pAVPacket.stream_index==videoStreamIndex){
            qDebug()<<"开始解码"<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            //qDebug()<<"packet size: "<<pAVPacket.size<<endl;
            avcodec_decode_video2(pAVCodecContext, pAVFrame, &m_i_frameFinished, &pAVPacket);
            if (m_i_frameFinished){
                //qDebug()<<"frameFinished!\n";
                mutex.lock();
                sws_scale(pSwsContext,(const uint8_t* const *)pAVFrame->data,pAVFrame->linesize,0,videoHeight,pAVPicture.data,pAVPicture.linesize);
                //发送获取一帧图像信号
                QImage image(pAVPicture.data[0],videoWidth,videoHeight,QImage::Format_RGB888);


                //image.save("/Users/yueyang/yiqunyang/大一暑假/科研/pic/test.jpg","JPG",100);
                emit GetImage(image);
                mutex.unlock();
            }
        }
    }
    av_free_packet(&pAVPacket);//释放资源,否则内存会一直上升
}*/
void VideoStream::playSlots()
{
    while(m_i_frameFinished == 0){
        Data NALU = manager->OutputNALU();
        pAVPacket.data;
        pAVPacket.size;
        avcodec_decode_video2(pAVCodecContext,pAVFrame,&m_i_frameFinished,&pAVPacket);
    }
    mutex.lock();
    sws_scale(pSwsContext,(const uint8_t* const *)pAVFrame->data,pAVFrame->linesize,0,videoHeight,pAVPicture.data,pAVPicture.linesize);
    QImage image(pAVPicture.data[0],videoWidth,videoHeight,QImage::Format_RGB888);
    emit GetImage(image);
    mutex.unlock();
}
