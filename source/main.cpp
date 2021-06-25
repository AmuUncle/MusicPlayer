#include "musicplayer.h"
#include <QApplication>
#include <QFile>
#include <QFont>
#include "appinit.h"
#include "iconhelper.h"
#include "musicmgr.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setFont(QFont("Microsoft Yahei", 9));
    a.setWindowIcon(QIcon(":/img/image/Music.ico"));

    //加载样式表
    QFile file(":/qss/css/musicplayer.css");
    if (file.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }

    AppInit::Instance()->start();
    IconHelper::Load();
    DATAMGR->Init();

    MusicPlayer w;
    w.show();

    return a.exec();
}
