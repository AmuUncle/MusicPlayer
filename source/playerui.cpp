#include "playerui.h"
#include <QPainter>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

PlayerUi::PlayerUi(QWidget *parent) : QWidget(parent)
{
    m_pixImage = QPixmap(":/img/image/chengdu.jpg");
    m_strSong = tr("成都");
    m_strSinger = tr("赵雷");
}

void PlayerUi::SetParam(QPixmap pixImage, QString strSong, QString strSinger)
{
    m_pixImage = pixImage;
    m_strSong = strSong;
    m_strSinger = strSinger;
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

    QPixmap p = m_pixImage.scaled(rcImg.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
    painter.drawText(rcSong, Qt::AlignHCenter | Qt::AlignBottom, m_strSong);
    painter.setFont(ft);

    QRect rcSinger(rcSong);
    rcSinger.setTop(rcSong.bottom() + 5);
    rcSinger.setBottom(rcSinger.top() + 40);
    painter.setPen(QColor("#666666"));
    ftTemp = ft;
    ftTemp.setPointSize(9);
    painter.setFont(ftTemp);
    painter.drawText(rcSinger, Qt::AlignHCenter | Qt::AlignTop, m_strSinger);
    painter.setFont(ft);


    painter.restore();

}
