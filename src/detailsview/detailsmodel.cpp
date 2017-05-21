#include "detailsmodel.h"

#include <QDebug>

#include "../data/items.h"

namespace WinUptime {

DetailsModel::DetailsModel(QObject *parent)
  : QAbstractTableModel(parent)
{
}

DetailsModel::~DetailsModel() = default;

QVariant DetailsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation != Qt::Horizontal)
    return QVariant();

  if (role == Qt::DisplayRole)
  {
    switch (section)
    {
    case ColumnTime: return tr("Time");
    case ColumnIcon: return QVariant();
    case ColumnName: return tr("Name");
    }
  }

  return QVariant();
}

int DetailsModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return int(spots_.size());
}

int DetailsModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return Columns;
}

QVariant DetailsModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || size_t(index.row()) >= spots_.size())
    return QVariant();

  if (role == Qt::DisplayRole)
  {
    auto& spot = spots_[size_t(index.row())];
    switch (index.column())
    {
    case ColumnTime:
      return spot.time().time().toString(Qt::DefaultLocaleShortDate);
    case ColumnIcon:
      return QVariant();
    case ColumnName:
      return spot.caption();
    }
  }
  else if (role == Qt::DecorationRole && index.column() == ColumnIcon)
  {
    auto& spot = spots_[size_t(index.row())];
    return spot.icon();
  }

  return QVariant();
}

void DetailsModel::setSpots(const std::vector<SpotItem>& spots)
{
  beginResetModel();
  spots_  = spots;
  endResetModel();
}

} // namespace WinUptime
