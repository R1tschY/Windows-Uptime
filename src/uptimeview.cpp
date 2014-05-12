#include <QSettings>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QScrollBar>
#include <QMenu>
#include <QClipboard>
#include <QApplication>
#include <QDebug>
#include <QTime>

#include "uptimeview.h"

EventModel::EventModel(QObject* parent) :
  QAbstractTableModel(parent),
  rows_()
{
}

int EventModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return rows_.size();
}

int EventModel::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return (int)Column::Max;
}

QVariant EventModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();

  if (index.row() >= static_cast<int>(rows_.size()) || index.row() < 0)
    return QVariant();

  int column = index.column();
  int row = index.row();

  if (role == Qt::DisplayRole) {
    Row data = rows_.at(row);
    switch ((Column)column) {
    case Column::Day:
      return std::get<DataColumn_Day>(data);

    case Column::Uptime:
      return std::get<DataColumn_Uptime>(data).toString("hh'h' mm'm' ss's'");

    case Column::Ontime:
     return std::get<DataColumn_Ontime>(data).toString("hh'h' mm'm' ss's'");

    default:
      return QVariant();
    }
  }

  return QVariant();
}

QVariant EventModel::headerData(int section,
                                 Qt::Orientation orientation,
                                 int role) const
{
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      switch ((Column)section) {
      case Column::Day:
        return "Day";

      case Column::Uptime:
        return "Uptime";

      case Column::Ontime:
        return "Ontime";

      default:
        return QVariant();
      }
    }
  }

  return QVariant();
}

void EventModel::addRow(unsigned day, QTime uptime, QTime ontime) {
  rows_.emplace_back(day, uptime, ontime);
}

bool EventModel::update() {
  if (rows_.size() > 0) {
    beginInsertRows(QModelIndex(), 0, rows_.size()-1);
    endInsertRows();
  }
}

void EventModel::clear() {
  if (rows_.size() > 0) {
    beginRemoveRows(QModelIndex(), 0, rows_.size()-1);
    rows_.clear();
    endRemoveRows();
  }
}

bool EventModel::setData(const QModelIndex &,
                          const QVariant &,
                          int)
{
  return false;
}

Qt::ItemFlags EventModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractTableModel::flags(index);
}
