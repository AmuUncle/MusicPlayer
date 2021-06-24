#ifndef MUSICMGR_H
#define MUSICMGR_H

#include <QObject>
#include <QPixmap>
#include <QUrl>
#include "libzplay.h"


struct MusicInfos
{
    QPixmap pixImage;
    QString strSong;
    QString strSinger;
    QString strLrcPath;
    QUrl strSongUrl;
};

class MusicMgr : public QObject
{
    Q_OBJECT

public:
    static MusicMgr* GetInstance();
    static void ExitInstance();

public:
    void Init();
    void GetMusicList(QList<MusicInfos> &listMusic) { listMusic = m_listMusics; }
    void GetMusicInfo(int nIndex, MusicInfos& item);

private:
    explicit MusicMgr(QObject *parent = 0);

    void ScanMusicFiles();
    void GetMusicInfo(QString strPath, MusicInfos &tMusicInfo);

private:
    static MusicMgr *m_pMusicMgr;
    QList<MusicInfos> m_listMusics;
    libZPlay::ZPlay *m_pZPlayer;
};

#define DATAMGR MusicMgr::GetInstance()

#endif // MUSICMGR_H
