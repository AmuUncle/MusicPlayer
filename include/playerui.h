#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <QWidget>
#include <QMap>
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

private:
    void ParseLrcFile();

public slots:
    void OnPlayPosChange(qint64 position);

private:
    MusicInfos m_MusicInfo;
    QString m_strCurLrc;
    QMap<qint64, QString> m_mapLrc;
};

#endif // PLAYERUI_H
