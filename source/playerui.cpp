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
    m_pTimerZhuan = NULL;
    m_nRotate = 0;

    m_pixImageBase.load(":/img/image/v2.0/图层 9.png");
    m_pixImageZhuan.load(":/img/image/v2.0/amuuncle·amuuncl·eamuuncle·amuuncle·.png");
    m_pixYuanhuan.load(":/img/image/v2.0/椭圆 5.png");

    m_pTimerZhuan = new QTimer(this);
    connect(m_pTimerZhuan, SIGNAL(timeout()), this, SLOT(OnTimerZhuanTimeOut()));
    m_pTimerZhuan->setInterval(100);

    OnStop();
}

void PlayerUi::SetParam(MusicInfos MusicInfo)
{
    m_MusicInfo = MusicInfo;

    if (m_MusicInfo.pixImage.isNull())
        m_MusicInfo.pixImage = QPixmap(":/img/image/sky.png");

    ParseLrcFile();
    update();
}

void PlayerUi::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    DrawBg(&painter);
}

void PlayerUi::DrawBg(QPainter *painter)
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing, true); // 抗锯齿和使用平滑转换算法

    QRect rcClient = rect();
    QFont ft = painter->font();

    painter->translate(rcClient.left(), rcClient.top());  // 移动绘制原点
    painter->scale(rcClient.width() / 800.0, rcClient.height() / 598.0);

    QRect recthaibao(281, 157, 233.0, 233.0);
    QPixmap pHaibao = m_MusicInfo.pixImage.scaled(recthaibao.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter->drawPixmap(recthaibao, pHaibao);

    QRect rectBase(0, 0, 800.0, 598.0);
    QPixmap p = m_pixImageBase.scaled(rectBase.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter->drawPixmap(rectBase, p);

    QRect rectzhuan(281, 157, 233.0, 233.0);
    QPixmap pZhuan = m_pixImageZhuan.scaled(rectzhuan.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter->save();
    painter->translate(rectzhuan.center());
    painter->rotate(m_nRotate);
    QRect rectzhuan1(-233.0 / 2, -233.0 / 2, 233.0, 233.0);
    painter->drawPixmap(rectzhuan1, pZhuan);
    painter->restore();

    QRect rectCd(550, 20, 160.0, 160.0);
    QPixmap pCd = QPixmap(":/img/image/v2.0/cd.png").scaled(rectCd.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter->save();
    painter->translate(rectCd.center());
    painter->rotate(m_nRotate);
    QRect rectCd1(-160.0 / 2, -160.0 / 2, 160.0, 160.0);
    painter->drawPixmap(rectCd1, pCd);
    painter->restore();

    painter->save();
    painter->translate(302, 90);
    painter->rotate(-7);
    painter->setPen(QColor("#DFF348"));
    QFont ftTemp = ft;
    ftTemp.setPointSize(15);
    painter->setFont(ftTemp);
    QRect rcSong(0, 0, 156.0, 37.0);
    painter->drawText(rcSong, Qt::AlignRight, m_MusicInfo.strSong);
    painter->restore();

    painter->save();
    painter->translate(426 - 80 + 35, 104);
    painter->rotate(-7);
    painter->setPen(QColor("#FFFFFF"));
    ftTemp.setPointSize(12);
    painter->setFont(ftTemp);
    QRect rcSingerg(0, 0, 80.0, 25.0);
    painter->drawText(rcSingerg, Qt::AlignRight, m_MusicInfo.strSinger);
    painter->restore();

    QRect rectYuanhuan(250, 216, 292.0, 115.0);
    QPixmap pYuanhuan = m_pixYuanhuan.scaled(rectYuanhuan.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter->drawPixmap(rectYuanhuan, pYuanhuan);

    if (m_strCurLrc.length() > 0)
    {
        painter->save();
        painter->setPen(QColor("#5D31FF"));
        ftTemp.setPointSize(14);
        ftTemp.setBold(true);
        painter->setFont(ftTemp);
        QRect rcLrc(10, 492, 300.0, 22.0);
        painter->drawText(rcLrc, Qt::AlignLeft, m_strCurLrc);
        painter->restore();
    }
}

void PlayerUi::ParseLrcFile()
{
    m_mapLrc.clear();

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

void PlayerUi::OnTimerZhuanTimeOut()
{
    m_nRotate += 1;

    if (m_nRotate > 360)
        m_nRotate = 0;

    update();
}

void PlayerUi::OnStop()
{
    m_pTimerZhuan->stop();
}

void PlayerUi::OnStart()
{
    m_pTimerZhuan->start();
}
