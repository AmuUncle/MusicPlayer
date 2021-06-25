#ifndef CFGDLG_H
#define CFGDLG_H

#include <QWidget>
#include "boderpane.h"

namespace Ui {
class CfgDlg;
}

class CfgDlg : public BoderPane
{
    Q_OBJECT

public:
    explicit CfgDlg(QWidget *parent = 0);
    ~CfgDlg();

private slots:
    void on_m_btnModify_clicked();

private:
    void CreateAllChildWnd();
    void InitCtrl();
    void InitSolts();
    void Relayout();

    bool event(QEvent *event);

private:
    Ui::CfgDlg *ui;
};

#endif // CFGDLG_H
