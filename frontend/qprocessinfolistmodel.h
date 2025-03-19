#include <QAbstractListModel>
#include <QList>
#include <QDebug>
#include "processinfo.h"

class QProcessInfoListModel : public QAbstractListModel
{
public:
    QProcessInfoListModel(QObject *parent = nullptr)
        : QAbstractListModel(parent){};
    QProcessInfoListModel(QList<ProcessInfo> &&pInfoList)
        : m_procInfoList{std::move(pInfoList)} {};

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return (int) m_procInfoList.size();
    }

    template<typename T>
    void addProcess(T &&pInfo)
    {
        beginInsertRows(QModelIndex(), m_procInfoList.size(), m_procInfoList.size());
        m_procInfoList.emplace_back(std::forward<T>(pInfo));
        endInsertRows();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_procInfoList.size()) {
            return QVariant();
        }
        const auto &process = m_procInfoList.at(index.row());

        switch (role) {
        case Qt::DisplayRole:
            return QString::fromStdString(process.pretty());
        default:
            return QVariant();
        }
    }

    ProcessInfo getSelectedProcess(const QModelIndex &index) const
    {
        return m_procInfoList[index.row()];
    }
    void refreshData(QList<ProcessInfo> &&pInfoVec)
    {
        beginResetModel();
        m_procInfoList = std::move(pInfoVec);
        endResetModel();
    }

    void clear()
    {
        beginResetModel();
        m_procInfoList.clear();
        endResetModel();
    }

private:
    QList<ProcessInfo> m_procInfoList;
};
