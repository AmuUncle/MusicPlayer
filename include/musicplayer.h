#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QUrl>
#include <QSlider>
#include "cfgdlg.h"


namespace Ui {
class MusicPlayer;
}

class QMediaPlaylist;
class QMediaPlayer;

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPlayer(QWidget *parent = 0);
    ~MusicPlayer();

    enum EMenuItem
    {
        MENUITEM_QUIT,
        MENUITEM_TRAY,
    };

    enum EPlaybackMode
    {
        PM_CURRENTITEMINLOOP,
        PM_SEQUENTIAL,
        PM_LOOP,
        PM_RANDOM,
        PM_MAX
    };

    struct MusicInfo
    {
        QPixmap pixImage;
        QString strSong;
        QString strSinger;
        QUrl strSongUrl;
    };

private:
    void CreateAllChildWnd();
    void InitCtrl();
    void InitSolts();
    void Relayout();
    void UpdateCtrlText();
    void InitTrayIcon();
    void PlayCloseAnimation();
    void GetMusicInfo(QString strPath, MusicInfo &tMusicInfo);
    void UpdatePlayLoopMode();

    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *evt);

public slots:
    void OnCurrentIndexChanged(int nIndex);
    void OnDurationChanged(qint64 duration);
    void OnPositionChanged(qint64 duration);
    void OnValueChange(int nValue);
    void OnStartBtnClicked();
    void OnMoreBtnClicked();
    void OnVolumeBtnClicked();
    void OnTransparencyBtnClicked();
    void OnCollectionBtnClicked();
    void OnWrapModeBtnClicked();
    void OnMenuTriggered(QAction *action);
    void OnVolumeChanged(int nValue);
    void OnTransparencyChanged(int nValue);
    void OnMusicListChange();

private:
    Ui::MusicPlayer *ui;
    QMediaPlaylist *m_pMedialist;
    QMediaPlayer *m_player;

    QWidget *m_widgetVolume;
    QSlider *m_sliderVolume;

    QWidget *m_widgetTransparency;
    QSlider *m_sliderTransparency;

    CfgDlg *m_pCfgDlg;

    bool m_bCloseAnimationState;

    EPlaybackMode m_eCurPlaybackMode;
};

#endif // MUSICPLAYER_H
