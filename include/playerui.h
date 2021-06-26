#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <QWidget>
#include <QMap>
#include <QTimer>
#include "musicmgr.h"

class PlayerUi : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerUi(QWidget *parent = 0);

public:
    void SetParam(MusicInfos MusicInf);

protected:
    void paintEvent(QPaintEvent *);
    void DrawBg(QPainter *painter);

private:
    void ParseLrcFile();

public slots:
    void OnPlayPosChange(qint64 position);
    void OnTimerZhuanTimeOut();
    void OnStop();
    void OnStart();

private:
    MusicInfos m_MusicInfo;
    QString m_strCurLrc;
    QMap<qint64, QString> m_mapLrc;
    QTimer *m_pTimerZhuan;
    int m_nRotate;

    QPixmap m_pixImageBase;
    QPixmap m_pixImageZhuan;
    QPixmap m_pixYuanhuan;
};

#endif // PLAYERUI_H
