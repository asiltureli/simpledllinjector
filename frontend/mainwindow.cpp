#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QErrorMessage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pSelectWindow { nullptr }
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pushButton_inject_clicked()
{
    auto result = m_backend.inject();
    QErrorMessage *qMes = new QErrorMessage{this};
    if (result == 0) {
        qDebug() << "Worked";
        qMes->showMessage("Succesfully injected");

    } else {
        qDebug() << "Error Code: " << result;
        qMes->showMessage("Error while injecting");
    }
    qMes->exec();
}
void MainWindow::pushButton_selectDLL_clicked()
{
    QString dll_path = QFileDialog::getOpenFileName(this,
                                                    "Select DLL File",
                                                    "",
                                                    "DLL Files (*.dll)");
    if (dll_path.size() == 0) {
        return;
    }
    qDebug() << dll_path;
    auto result = m_backend.set_dll_path(dll_path.toStdString());
    if (result) {
        qDebug() << "Error while setting DLL path";
    } else {
        ui->label_dllPath->setText(dll_path);
    }
}
void MainWindow::pushButton_selectProcess_clicked()
{
    if(!pSelectWindow)
    {
        pSelectWindow = new ProcessSelect(this);
        //pSelectWindow->setAttribute(Qt::WA_DeleteOnClose);

        connect(pSelectWindow, &ProcessSelect::processSelect_refresh_request, this, &MainWindow::handle_processSelect_refresh);
        connect(pSelectWindow, &ProcessSelect::processSelect_select_request, this, &MainWindow::handle_processSelect_select);
        connect(this,
                &MainWindow::processSelect_refresh_response,
                pSelectWindow,
                &ProcessSelect::onProcessSelect_refresh_response);
    }

    pSelectWindow->show();
    pSelectWindow->raise();
    pSelectWindow->activateWindow();

}

void MainWindow::handle_processSelect_refresh()
{
    std::vector<ProcessInfo> p_list;
    auto result = m_backend.get_active_processes(p_list);

    emit processSelect_refresh_response(result, p_list);
}
void MainWindow::handle_processSelect_select(ProcessInfo process)
{
    auto result = m_backend.set_process(process);
    if (result) {
        qDebug() << "Error while setting process";
    } else {
        ui->label_processName->setText(QString::fromStdString(process.pretty()));
    }
}
