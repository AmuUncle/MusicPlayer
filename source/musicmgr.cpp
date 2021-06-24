#include "musicmgr.h"
#include <QDir>
#include <QCoreApplication>
#include <QDebug>

using namespace libZPlay;

MusicMgr* MusicMgr::m_pMusicMgr = NULL;

MusicMgr::MusicMgr(QObject *parent) : QObject(parent)
{
    m_pZPlayer = NULL;
}

void MusicMgr::ScanMusicFiles()
{
    m_listMusics.clear();

    MusicInfos item;

    QString fileName = QCoreApplication::applicationDirPath();

    QString dirpath = QString("%1/music/").arg(fileName);
    //设置要遍历的目录
    QDir dir(dirpath);
    //设置文件过滤器
    QStringList nameFilters;
    //设置文件过滤格式
    nameFilters << "*.mp3";
    //将过滤后的文件名称存入到files列表中
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    foreach (QString strFile, files)
    {
        item.pixImage = QPixmap(QString("%1/music/cover/%2.png").arg(fileName).arg(strFile.section(".",0,0)));
        item.strLrcPath = QString("%1/music/lyric/%2.lrc").arg(fileName).arg(strFile.section(".",0,0));
        item.strSongUrl = QUrl::fromLocalFile(QString("%1/music/%2").arg(fileName).arg(strFile));
        GetMusicInfo(QString("%1/music/%2").arg(fileName).arg(strFile), item);
        m_listMusics << item;
    }
}

void MusicMgr::GetMusicInfo(QString strPath, MusicInfos &tMusicInfo)
{
    if (NULL == m_pZPlayer)
        m_pZPlayer = CreateZPlay();

    TID3InfoEx id3_info;
    if(m_pZPlayer->OpenFile(strPath.toLocal8Bit(), sfAutodetect))
    {
        if(m_pZPlayer->LoadID3Ex(&id3_info,1))
        {
            tMusicInfo.strSong = QString::fromLocal8Bit(id3_info.Title);
            tMusicInfo.strSinger = QString::fromLocal8Bit(id3_info.Artist);
        }
    }
}

MusicMgr *MusicMgr::GetInstance()
{
    if (NULL == m_pMusicMgr)
        m_pMusicMgr = new MusicMgr();

    return m_pMusicMgr;
}

void MusicMgr::ExitInstance()
{
    if (m_pMusicMgr != NULL)
    {
        delete m_pMusicMgr;
        m_pMusicMgr = NULL;
    }
}

void MusicMgr::Init()
{
    ScanMusicFiles();
}

void MusicMgr::GetMusicInfo(int nIndex, MusicInfos &item)
{
    if (nIndex < 0 || nIndex >= m_listMusics.size())
        return;

    item = m_listMusics.at(nIndex);
}
