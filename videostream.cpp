#include "videostream.h"
#include "ui_videostream.h"

#include <QDebug>
#include <QDateTime>
#include <QLayout>
#define swap32Big2Little(x)    (   ( (x)&(0x0000ffff) ) << 32 |  ( (x)&(0xffff0000) >> 32   ) )

VideoStream::VideoStream(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VideoStream)
{
    manager = new Manager;

    m_label = new QLabel;
    btnStart = new QPushButton;
    btnStart->setText("start");
    btnStop = new QPushButton;
    btnStop->setText("Stop");
    m_timerPlay = new QTimer;
    m_timerPlay->setInterval(40);

    /*socket
    socket = new QUdpSocket(this);
    port = 2002;
    bool result = socket->bind(port);
    if(!result){
        qDebug()<<"bind failed...";
    }
    connect(socket,SIGNAL(readyRead()),this,SLOT(startStream()));
*/


    //关联信号与槽
    connect(m_timerPlay,SIGNAL(timeout()),this,SLOT(SetImageSlots()));
    connect(btnStart,SIGNAL(clicked()),this, SLOT(startStream()));
    connect(btnStop,SIGNAL(clicked()),this,SLOT(stopStream()));
    //setUrl("/Users/yueyang/yiqunyang/大一暑假/科研/control.264");



    ui->setupUi(this);
    QVBoxLayout* lay = new QVBoxLayout;
    lay->addWidget(m_label);
    lay->addWidget(btnStart);
    lay->addWidget(btnStop);
    ui->centralWidget->setLayout(lay);
    qDebug()<<"window construct successful\n";
}

VideoStream::~VideoStream()
{
    delete ui;
}

void VideoStream::SetImageSlots()
{
    QPixmap pix;
    if(manager->Outputpic(pix))
    {
        m_label->setPixmap(pix);
    }
}

void VideoStream::startStream()
{

    btnStart->setEnabled(false);
    btnStop->setEnabled(true);
    qDebug()<<"recv.init() success";
    recv.init(socket,manager);
    recv.start();
    QTime t;
    t.start();
    while(t.elapsed()<300);
    //等一会
    qDebug()<<"startStream";

    m_timerPlay->start();
}

void VideoStream::stopStream()
{
    btnStart->setEnabled(true);
    btnStop->setEnabled(false);

    m_timerPlay->stop();
    manager->stop();

}


