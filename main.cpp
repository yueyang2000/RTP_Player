#include "videostream.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VideoStream w;
    w.show();

    return a.exec();
}
