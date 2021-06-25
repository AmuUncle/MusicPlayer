#include "cfgdlg.h"
#include "ui_cfgdlg.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include "musicmgr.h"


#ifdef WIN32
#include "Windows.h"
#pragma comment(lib, "User32.lib")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


CfgDlg::CfgDlg(QWidget *parent) :
    BoderPane(parent),
    ui(new Ui::CfgDlg)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setProperty("form", "cfgdlg");
    setAttribute(Qt::WA_StyledBackground);  // 禁止父窗口样式影响子控件样式

    CreateAllChildWnd();
    InitCtrl();
    InitSolts();
    Relayout();
}

CfgDlg::~CfgDlg()
{
    delete ui;
}

void CfgDlg::CreateAllChildWnd()
{

}

void CfgDlg::InitCtrl()
{

}

void CfgDlg::InitSolts()
{

}

void CfgDlg::Relayout()
{
    QVBoxLayout *layoutMain = new QVBoxLayout();
    layoutMain->addWidget(ui->m_widgetCenter);
    layoutMain->setMargin(8);

    centralWidget()->setLayout(layoutMain);
}

bool CfgDlg::event(QEvent *event)
{
    static bool class_amended = false;
    if (event->type() == QEvent::WinIdChange)
    {
        HWND hwnd = reinterpret_cast<HWND>(winId());
        if (class_amended == false)
        {
            class_amended = true;
            DWORD class_style = ::GetClassLong(hwnd, GCL_STYLE);
            class_style &= ~CS_DROPSHADOW;
            ::SetClassLong(hwnd, GCL_STYLE, class_style); // windows系统函数
        }
    }
    else if (event->type() == QEvent::Show)
    {
        ui->m_editDir->setText(DATAMGR->GetRootDir());
    }

    return BoderPane::event(event);
}

void CfgDlg::on_m_btnModify_clicked()
{
    QString strCurDir = ui->m_editDir->text();
    if (strCurDir.isEmpty())
        strCurDir = QDir::currentPath();   // 获取系统当前目录

    QString strDir = QFileDialog::getExistingDirectory(this, tr("选择目录"), strCurDir);
    if (strDir.isEmpty())
        return;

    ui->m_editDir->setText(strDir);
    DATAMGR->SetRootDir(strDir);
}
