#include "musicplayer.h"
#include "ui_musicplayer.h"
#include <QGraphicsDropShadowEffect>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "iconhelper.h"
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QPropertyAnimation>
#include "libzplay.h"
#include "musicmgr.h"


#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

using namespace libZPlay;

MusicPlayer::MusicPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);

    m_pMedialist = NULL;
    m_player = NULL;
    m_bCloseAnimationState = false;

    ui->m_widgetMain->setAttribute(Qt::WA_StyledBackground);  // 禁止父窗口样式影响子控件样式
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
    ui->m_widgetMain->setProperty("form", "mainwnd");
    setProperty("canMove", "true");

    CreateAllChildWnd();
    InitCtrl();
    InitSolts();
    Relayout();
    InitTrayIcon();
}

MusicPlayer::~MusicPlayer()
{
    delete ui;
}

void MusicPlayer::CreateAllChildWnd()
{
#define NEW_OBJECT(pObj, TYPE) \
    if (NULL == pObj) { pObj = new TYPE(this); }

    NEW_OBJECT(m_pMedialist, QMediaPlaylist);
    NEW_OBJECT(m_player, QMediaPlayer);
}

void MusicPlayer::InitCtrl()
{
    // 设置边框阴影
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    // 设置具体阴影
    QGraphicsDropShadowEffect *shadowBorder = new QGraphicsDropShadowEffect(ui->m_widgetMain);
    shadowBorder->setOffset(0, 0);
    // 阴影颜色
    shadowBorder->setColor(QColor(0x44, 0x44, 0x44, 127));
    // 阴影半径
    shadowBorder->setBlurRadius(20);
    ui->m_widgetMain->setGraphicsEffect(shadowBorder);

    ui->m_btnPre->setProperty("m_btnPre", "true");
    ui->m_btnStart->setProperty("m_btnStart", "true");
    ui->m_btnNext->setProperty("m_btnNext", "true");

    IconHelper::SetIcon(ui->m_btnPre, QChar(0xe676), 15);
    IconHelper::SetIcon(ui->m_btnStart, QChar(0xe612), 24);
    IconHelper::SetIcon(ui->m_btnNext, QChar(0xe677), 15);

    IconHelper::SetIcon(ui->m_btnCollection, QChar(0xe600), 15);
    IconHelper::SetIcon(ui->m_btnWrapMode, QChar(0xe71f), 15);
    IconHelper::SetIcon(ui->m_btnRecovery, QChar(0xe601), 15);
    IconHelper::SetIcon(ui->m_btnMore, QChar(0xe603), 15);

    ui->m_btnCollection->setCheckable(true);

    ui->m_Slider->EnableRange(false);
    ui->m_Slider->SetRange(0, 3 * 60 + 54);
    ui->m_Slider->SetStep(1);
    ui->m_Slider->SetCurPos(0);
    ui->m_Slider->EnablePercent(true);
    ui->m_Slider->EnableTimeMode(true);

    setWindowOpacity(0.5);

    DATAMGR->Init();

    QList<MusicInfos> listMusics;
    DATAMGR->GetMusicList(listMusics);
    foreach (MusicInfos item, listMusics)
    {
        m_pMedialist->addMedia(item.strSongUrl);
    }

    m_pMedialist->setPlaybackMode(QMediaPlaylist::Loop);
    m_player->setPlaylist(m_pMedialist);
    m_player->setVolume(50);
    m_player->play();

    OnCurrentIndexChanged(0);
}

void MusicPlayer::InitSolts()
{
    connect(m_pMedialist, SIGNAL(currentIndexChanged(int)), this, SLOT(OnCurrentIndexChanged(int)));
    connect(m_player, SIGNAL(durationChanged(qint64)), this, SLOT(OnDurationChanged(qint64)));
    connect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(OnPositionChanged(qint64)));
    connect(m_player, SIGNAL(positionChanged(qint64)), ui->m_widgetMain, SLOT(OnPlayPosChange(qint64)));
    connect(ui->m_Slider, SIGNAL(SignalValueChange(int)), this, SLOT(OnValueChange(int)));

    connect(ui->m_btnStart, SIGNAL(clicked()), this, SLOT(OnStartBtnClicked()));
    connect(ui->m_btnMore, SIGNAL(clicked()), this, SLOT(OnMoreBtnClicked()));
    connect(ui->m_btnPre, SIGNAL(clicked()), m_pMedialist, SLOT(previous()));
    connect(ui->m_btnNext, SIGNAL(clicked()), m_pMedialist, SLOT(next()));
}

void MusicPlayer::Relayout()
{

}

void MusicPlayer::UpdateCtrlText()
{

}

void MusicPlayer::InitTrayIcon()
{

}

void MusicPlayer::OnCurrentIndexChanged(int nIndex)
{
    MusicInfos item;
    DATAMGR->GetMusicInfo(nIndex, item);
    ui->m_widgetMain->SetParam(item);
}

void MusicPlayer::OnDurationChanged(qint64 duration)
{
    ui->m_Slider->SetRange(0, duration / 1000);
}

void MusicPlayer::OnPositionChanged(qint64 duration)
{
    ui->m_Slider->SetCurPos(duration / 1000);
}

void MusicPlayer::OnValueChange(int nValue)
{
    m_player->setPosition(nValue * 1000);
}

void MusicPlayer::OnStartBtnClicked()
{
    if (m_player->state() == QMediaPlayer::PlayingState)
    {
        m_player->pause();
        IconHelper::SetIcon(ui->m_btnStart, QChar(0xe656), 24);
    }
    else
    {
        m_player->play();
        IconHelper::SetIcon(ui->m_btnStart, QChar(0xe612), 24);
    }
}

void MusicPlayer::OnMoreBtnClicked()
{
    //创建菜单对象
    QMenu *pMenu = new QMenu();

    QAction *pQuit = new QAction(tr("退出"), pMenu);
    pQuit->setData(MENUITEM_QUIT);

    QAction *pTray = new QAction(tr("最小化到托盘"), pMenu);
    pTray->setData(MENUITEM_TRAY);

    //把QAction对象添加到菜单上
    pMenu->addAction(pTray);
    pMenu->addAction(pQuit);

    connect(pMenu, SIGNAL(triggered(QAction*)), this, SLOT(OnMenuTriggered(QAction*)));

    QPoint ptBtn = ui->m_btnMore->mapToGlobal(ui->m_btnMore->pos());
    QPoint point(ptBtn);
    point.setX(ptBtn.x() - ui->m_btnMore->pos().x() + 15);
    point.setY(ptBtn.y() - ui->m_btnMore->pos().y() - 4 - 30 * 2);
    pMenu->exec(point);

    //释放内存
    QList<QAction*> list = pMenu->actions();
    foreach (QAction* pAction, list)
        delete pAction;

    delete pMenu;
}

void MusicPlayer::OnMenuTriggered(QAction *action)
{
    EMenuItem eItem = (EMenuItem)action->data().toInt();
    switch (eItem)
    {
    case MENUITEM_QUIT:
        this->close();
        break;
    default:
        break;
    }
}

void MusicPlayer::PlayCloseAnimation()
{
    setDisabled(true);
    setMinimumSize(0,0);
    QPropertyAnimation* closeAnimation = new QPropertyAnimation(this,"geometry");
    closeAnimation->setStartValue(geometry());
    closeAnimation->setEndValue(QRect(geometry().x(), geometry().y()+height()/2, width(), 0));
    closeAnimation->setDuration(500);
    m_bCloseAnimationState = true;

    closeAnimation->setEasingCurve(QEasingCurve::OutBounce);  // 缓和曲线风格

    connect(closeAnimation,SIGNAL(finished()),this,SLOT(close()));
    closeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MusicPlayer::GetMusicInfo(QString strPath, MusicPlayer::MusicInfo &tMusicInfo)
{
    ZPlay *player = CreateZPlay();

    // chek if we have class instance
    if(player == 0)
    {
        printf("Error: Can't create class instance !\nPress key to exit.\n");
        return;
    }

    TID3InfoEx id3_info;
    if(player->OpenFile(strPath.toLocal8Bit(), sfAutodetect))
    {
        if(player->LoadID3Ex(&id3_info,1))
        {
            tMusicInfo.strSong = QString::fromLocal8Bit(id3_info.Title);
            tMusicInfo.strSinger = QString::fromLocal8Bit(id3_info.Artist);
        }
    }
}

void MusicPlayer::closeEvent(QCloseEvent *event)
{
    if(!m_bCloseAnimationState)
    {
        PlayCloseAnimation();
        event->ignore();
    }
    else{
        event->accept();
    }
}
