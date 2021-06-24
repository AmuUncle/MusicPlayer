#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QUrl>

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

    void closeEvent(QCloseEvent *event);

public slots:
    void OnCurrentIndexChanged(int nIndex);
    void OnDurationChanged(qint64 duration);
    void OnPositionChanged(qint64 duration);
    void OnValueChange(int nValue);
    void OnStartBtnClicked();
    void OnMoreBtnClicked();
    void OnMenuTriggered(QAction *action);

private:
    Ui::MusicPlayer *ui;
    QMediaPlaylist *m_pMedialist;
    QMediaPlayer *m_player;

    bool m_bCloseAnimationState;
};

#endif // MUSICPLAYER_H
