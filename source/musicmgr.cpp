#include "musicmgr.h"
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>


using namespace libZPlay;

#define CFG_INI_FILENAME "MusicPlayer"

MusicMgr* MusicMgr::m_pMusicMgr = NULL;

MusicMgr::MusicMgr(QObject *parent) : QObject(parent)
{
    m_pZPlayer = NULL;
}

void MusicMgr::ScanMusicFiles()
{
    m_listMusics.clear();

    MusicInfos item;

    //设置要遍历的目录
    QDir dir(m_strRootDir);
    //设置文件过滤器
    QStringList nameFilters;
    //设置文件过滤格式
    nameFilters << "*.mp3";
    //将过滤后的文件名称存入到files列表中
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    foreach (QString strFile, files)
    {
        item.pixImage = QPixmap(QString("%1/cover/%2.png").arg(m_strRootDir).arg(strFile.section(".",0,0)));
        item.strLrcPath = QString("%1/lyric/%2.lrc").arg(m_strRootDir).arg(strFile.section(".",0,0));
        item.strSongUrl = QUrl::fromLocalFile(QString("%1/%2").arg(m_strRootDir).arg(strFile));
        GetMusicInfo(QString("%1/%2").arg(m_strRootDir).arg(strFile), item);
        m_listMusics << item;
    }

    emit SignalMusicListChange();
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

void MusicMgr::ReadCfg(QString &strCfg)
{
    QSettings settingsread(GetIniFilePath(), QSettings::IniFormat);
    settingsread.setIniCodec("utf-8");

    QString strKey = "Basics/path";
    strCfg = settingsread.value(strKey, "null").toString();
}

void MusicMgr::WriteCfg(QString strCfg)
{
    QSettings settings(GetIniFilePath(), QSettings::IniFormat);
    settings.setIniCodec("utf-8");
    settings.beginGroup("Basics");
    settings.setValue("path", strCfg);
    settings.endGroup();
}

QString MusicMgr::GetIniFilePath()
{
    QString strDocuments = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);

    QString strPath = QDir::toNativeSeparators(QString("%1/%2/").arg(strDocuments).arg(CFG_INI_FILENAME));

    QDir dir;
    dir.mkpath(strPath);

    strPath = QDir::toNativeSeparators(QString("%1/%2/%2.ini").arg(strDocuments).arg(CFG_INI_FILENAME));

    qDebug() << strPath;

    return strPath;
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
    QString strCfg;
    ReadCfg(strCfg);

    if (0 != strCfg.compare("null"))
    {
        SetRootDir(strCfg);
    }
    else
    {
        QString strMusicLocation = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
        SetRootDir(strMusicLocation);
    }
}

void MusicMgr::SetRootDir(QString strRootDir)
{
    m_strRootDir = strRootDir;
    WriteCfg(m_strRootDir);
    ScanMusicFiles();
}

void MusicMgr::GetMusicInfo(int nIndex, MusicInfos &item)
{
    if (nIndex < 0 || nIndex >= m_listMusics.size())
        return;

    item = m_listMusics.at(nIndex);
}
