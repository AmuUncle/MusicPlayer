#include "playerui.h"
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

PlayerUi::PlayerUi(QWidget *parent) : QWidget(parent)
{
}

void PlayerUi::SetParam(MusicInfos MusicInf)
{
    m_MusicInfo = MusicInf;

    if (m_MusicInfo.pixImage.isNull())
        m_MusicInfo.pixImage = QPixmap(":/img/image/sky.png");

    ParseLrcFile();
    update();
}

void PlayerUi::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QFont ft = painter.font();
    QRect rcClient = rect();

    painter.save();
    QRect rcBgImg(rcClient);
    rcBgImg.setBottom(rcBgImg.top() + rcBgImg.width());
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#45544C"));
    painter.drawRoundedRect(rcBgImg, 4, 4);

    QRect rcImg(rcBgImg);
    rcImg = rcImg.marginsRemoved(QMargins(10, 10, 10, 10));

    QPixmap p = m_MusicInfo.pixImage.scaled(rcImg.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(rcImg, p);

    painter.restore();

    painter.save();
    QRect rcSong(rcClient);
    rcSong.setTop(rcBgImg.bottom() + 20);
    rcSong.setBottom(rcSong.top() + 20);
    painter.setPen(QColor("#D30320"));
    QFont ftTemp = ft;
    ftTemp.setPointSize(12);
    painter.setFont(ftTemp);
    painter.drawText(rcSong, Qt::AlignHCenter | Qt::AlignBottom, m_MusicInfo.strSong);
    painter.setFont(ft);

    QRect rcSinger(rcSong);
    rcSinger.setTop(rcSong.bottom() + 5);
    rcSinger.setBottom(rcSinger.top() + 40);
    painter.setPen(QColor("#666666"));
    ftTemp = ft;
    ftTemp.setPointSize(9);
    painter.setFont(ftTemp);
    painter.drawText(rcSinger, Qt::AlignHCenter | Qt::AlignTop, m_MusicInfo.strSinger);
    painter.setFont(ft);

    if (!m_strCurLrc.isEmpty())
    {
        QRect rcLrc(rcSinger);
        painter.setPen(QColor("#909090"));
        ftTemp = ft;
        ftTemp.setPointSize(8);
        painter.setFont(ftTemp);
        painter.drawText(rcLrc, Qt::AlignHCenter | Qt::AlignBottom, m_strCurLrc);
        painter.setFont(ft);
    }

    painter.restore();
}

void PlayerUi::ParseLrcFile()
{
    m_mapLrc.clear();

    qDebug() << m_MusicInfo.strLrcPath;

    QString p = "\\[([0-9]+:[0-9]+.[0-9]+)\\]";
    QFile file(m_MusicInfo.strLrcPath);
    if (file.open(QFile::ReadOnly))
    {
        QTextStream text_stream(&file);
        while(!text_stream.atEnd())
        {
            QString content = text_stream.readLine(0);

            QString pattern(p);
            QRegExp rx(pattern);

            QString strGeci = content.right(content.size() - (content.lastIndexOf("]")+1));

            while (content.indexOf(rx)>=0)
            {
                if (rx.matchedLength() > 1)
                {
                    QString strTime = rx.capturedTexts().at(1);

                    int nMin = strTime.mid(0, 2).toInt();
                    int nSec = strTime.mid(3, 2).toInt();
                    int nSSec = strTime.mid(6, 2).toInt();

                    qint64 nPos =  (nMin * 60 + nSec) * 100 + nSSec;
                    m_mapLrc.insert(nPos, strGeci);
                }

                content = content.mid(10);
            }
        }

        file.close();
    }
}

void PlayerUi::OnPlayPosChange(qint64 position)
{
    QString strCurLrc = "";

    position /= 10;

    foreach (const qint64 &nPos, m_mapLrc.keys())
    {
        if (nPos > position)
            break;

        strCurLrc = m_mapLrc.value(nPos);
    }

    if (strCurLrc != m_strCurLrc)
    {
        m_strCurLrc = strCurLrc;
        update();
    }
}
