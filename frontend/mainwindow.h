#pragma once

#include <QMainWindow>
#include <QPointer>
#include "backend.h"
#include "processselect.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void handle_processSelect_refresh();
    void handle_processSelect_select(ProcessInfo process);

private slots:
    void pushButton_inject_clicked();
    void pushButton_selectDLL_clicked();
    void pushButton_selectProcess_clicked();
signals:
    void processSelect_refresh_response(int32_t result, std::vector<ProcessInfo> p_list);

private:
    Ui::MainWindow *ui;
    QPointer<ProcessSelect> pSelectWindow;
    Backend m_backend;
};
