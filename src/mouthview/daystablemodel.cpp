#include "daystablemodel.h"

#include <QDebug>

#include "../utils.h"
#include "../data/itemmodel.h"
#include "../data/items.h"

namespace WinUptime {

struct DaysTableModelEntry
{
  float uptime;
  Items items;
};

DaysTableModel::DaysTableModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  days_.reserve(31);
}

DaysTableModel::~DaysTableModel() = default;

QVariant DaysTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation != Qt::Horizontal)
    return {};

  if (role == Qt::DisplayRole)
  {
    switch (section)
    {
    case ColumnDay:       return tr("Day");
    case ColumnTimeline:  return tr("Timeline");
    case ColumnUptime:    return tr("Uptime");
    }
  }

  return QVariant();
}

int DaysTableModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return days_.size();
}

int DaysTableModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return NumberOfColumns;
}

QVariant DaysTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  int row = index.row();
  int column = index.column();
  if (size_t(row) >= days_.size() || column >= NumberOfColumns)
    return QVariant();

  if (role == Qt::DisplayRole)
  {
    switch (column)
    {
    case ColumnDay: return row + 1;
    case ColumnTimeline: return QVariant();
    case ColumnUptime:
      return QTime::fromMSecsSinceStartOfDay(days_[row].uptime * 1000)
          .toString("hh:mm:ss");
    }
  }

  return QVariant();
}

const Items& DaysTableModel::getItems(const QModelIndex& index) const
{
  if (!index.isValid())
    throw std::invalid_argument("invalid index in DaysTableModel::getItems");

  return days_.at(size_t(index.row())).items;
}

ItemModel* DaysTableModel::getModel() const
{
  return model_;
}

void DaysTableModel::setModel(ItemModel* model)
{
  beginResetModel();

  if (model_)
  {
    model_->disconnect(this);
  }

  model_ = model;

  if (model_)
  {
    connect(
          model_, &ItemModel::endLoading,
          this, &DaysTableModel::onDataChanged
    );
  }

  endResetModel();
}

void DaysTableModel::onDataChanged()
{
  static const QString onType = QStringLiteral("powerstate.on");

  beginResetModel();
  // TODO: scope(exit) { endResetModel(); }

  QDateTime model_start = model_->getStart();
  QDateTime model_end = model_->getEnd();

  days_.resize(model_start.date().daysInMonth());
  for (auto& day : days_)
  {
    day.uptime = 0;
    day.items.clear();
  }

  auto& items = model_->getItems();
  for (const RangeItem& range : items.ranges())
  {
    QDateTime start = range.start();
    QDateTime end = range.end();

    start = std::max(start, model_start);
    end = std::min(end, model_end);

    bool up = range.type() == onType;

    QDateTime curstart = start;
    QDateTime cur = start;
    while (true)
    {
      cur = cur.addDays(1);
      cur.setTime(QTime());

      size_t dayIdx = int(curstart.date().day() - 1);

      if (cur > end)
      {
        RangeItem item = range;
        item.setRange(curstart, end);
        days_[dayIdx].items.addRange(item);
        if (up)
          days_[dayIdx].uptime += curstart.msecsTo(end) / 1000.f;
        break;
      }

      RangeItem item = range;
      item.setRange(curstart, cur.addSecs(-1));
      days_[dayIdx].items.addRange(item);
      if (up)
        days_[dayIdx].uptime += curstart.msecsTo(end) / 1000.f;

      curstart = cur;
    }
  }

  for (const SpotItem& spot : items.spots())
  {
    int dayIdx = spot.time().date().day() - 1;
    days_.at(dayIdx).items.addSpot(spot);
  }

  endResetModel();
}

} // namespace WinUptime
