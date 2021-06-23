#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <QWidget>

class PlayerUi : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerUi(QWidget *parent = 0);

public:
    void SetParam(QPixmap pixImage, QString strSong, QString strSinger);

protected:
    void paintEvent(QPaintEvent *);

private:
    QPixmap m_pixImage;
    QString m_strSong;
    QString m_strSinger;
};

#endif // PLAYERUI_H
