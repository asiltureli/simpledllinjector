#include "processselect.h"
#include <QMessageBox>
#include "ui_processselect.h"
ProcessSelect::ProcessSelect(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProcessSelect)
    , m_listView_pListModel{new QProcessInfoListModel(this)}
    , m_listView_proxyModel{new QSortFilterProxyModel(this)}
{
    ui->setupUi(this);
    ui->listView_processSelect_processList->setModel(m_listView_pListModel);
    setup_filter_proxy();
}

ProcessSelect::~ProcessSelect()
{
    delete ui;
}

void ProcessSelect::pushButton_processSelect_refresh_clicked()
{
    emit processSelect_refresh_request();
}
void ProcessSelect::pushButton_processSelect_select_clicked()
{
    auto selected_indexes = ui->listView_processSelect_processList->selectionModel()
                                ->selectedIndexes();
    if (selected_indexes.size() == 1) {
        QModelIndex source_index = m_listView_proxyModel->mapToSource(selected_indexes.first());

        auto selected_process = m_listView_pListModel->getSelectedProcess(source_index);

        emit processSelect_select_request(selected_process);
        this->accept();
    } else {
        QMessageBox::warning(this,
                             "Selection Error",
                             "Please select exactly one process from the list!");
    }
}

void ProcessSelect::onProcessSelect_refresh_response(int32_t result, std::vector<ProcessInfo> p_list)
{
    if (result < 0) {
        switch (result) {
        case -1:
            QMessageBox::critical(this, "Error", "TLHelp32 returned an invalid handle");
            break;
        case -2:
            QMessageBox::critical(this, "Error", "Could not retrieve process information");
            break;
        default:
            QMessageBox::critical(this, "Error", "Unknown error");
        }
        return;
    }

    m_listView_pListModel->refreshData(QList<ProcessInfo>{p_list.begin(), p_list.end()});
}

void ProcessSelect::setup_filter_proxy()
{
    m_listView_proxyModel->setSourceModel(m_listView_pListModel);
    m_listView_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->listView_processSelect_processList->setModel(m_listView_proxyModel);

    connect(ui->lineEdit_processSelect_search,
            &QLineEdit::textChanged,
            m_listView_proxyModel,
            &QSortFilterProxyModel::setFilterFixedString);
}
