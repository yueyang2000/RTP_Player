#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H


#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QImage>
#include <QPushButton>
#include "packet.h"
#include <QThread>
#include "recv.h"
#include <queue>

namespace Ui {
class VideoStream;
}

class VideoStream : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoStream(QWidget *parent = nullptr);
    ~VideoStream();



private:
    QString m_str_url;
    QTimer* m_timerPlay;
    QLabel* m_label;
    QPushButton* btnStart;
    QPushButton* btnStop;

    Manager* manager;
    Ui::VideoStream *ui;

    Recv_package recv;
    QPixmap pix;


private slots:
    void SetImageSlots();
    void startStream();
    void stopStream();
};

#endif // VIDEOSTREAM_H
