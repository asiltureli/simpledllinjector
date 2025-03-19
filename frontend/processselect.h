#pragma once

#include <QDialog>
#include <QSortFilterProxyModel>
#include "qprocessinfolistmodel.h"

namespace Ui {
class ProcessSelect;
}

class ProcessSelect : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessSelect(QWidget *parent = nullptr);
    ~ProcessSelect();
public slots:
    void onProcessSelect_refresh_response(int32_t result, std::vector<ProcessInfo> p_list);
signals:
    void processSelect_refresh_request();
    void processSelect_select_request(ProcessInfo process);
private slots:
    void pushButton_processSelect_refresh_clicked();
    void pushButton_processSelect_select_clicked();

private:
    Ui::ProcessSelect *ui;
    QProcessInfoListModel *m_listView_pListModel;
    QSortFilterProxyModel *m_listView_proxyModel;

    void setup_filter_proxy();
};
