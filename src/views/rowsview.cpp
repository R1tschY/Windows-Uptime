#include "rowsview.h"

#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVBoxLayout>

#include "../data/items.h"
#include "../data/itemmodel.h"

namespace WinUptime {

constexpr double SecondsPerHour = 3600;
constexpr double SecondsPerDay = SecondsPerHour * 24;

namespace {

//
// TimeLineViewItem

class TimeLineViewItem : public QGraphicsItem
{
public:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

QRectF TimeLineViewItem::boundingRect() const
{
  return QRectF(0, 0, SecondsPerDay, 1.0);
}

void TimeLineViewItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  painter->setPen(Qt::black);
  painter->drawRect(QRectF(0, 0, SecondsPerDay, 1.0));
}

//
// RangeViewItem

class RangeViewItem : public QGraphicsItem
{
public:
  RangeViewItem(const RangeItem& range, QGraphicsItem* parent)
    : QGraphicsItem(parent), range_(range)
  { }

public:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
  RangeItem range_;
};

QRectF RangeViewItem::boundingRect() const
{
  return QRectF(0,0, range_.end() - range_.start(), 1);
}

void RangeViewItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  painter->setPen(Qt::green);
  painter->drawRect(QRectF(0,0, range_.end() - range_.start(), 1));
}

} // namespace

RowsView::RowsView(QWidget* parent)
  : UptimeView(parent)
{
  QVBoxLayout* layout = new QVBoxLayout(this);

  scene_ = new QGraphicsScene(this);

  view_ = new QGraphicsView(this);
  view_->setScene(scene_);
  view_->scale(300./SecondsPerDay, 300.);
  view_->setInteractive(true);
  layout->addWidget(view_);

  time_lines_.reserve(31);
  auto* item = new TimeLineViewItem();
  scene_->addItem(item);
  time_lines_.emplace_back(item);
}

RowsView::~RowsView()
{ }

void RowsView::dataCleared()
{
}

void RowsView::dataLoaded()
{
  auto& items = model()->getItems();
  for (const RangeItem& range : items.ranges())
  {
    qDebug() << "start" << range.start().toDateTime();
    items_.emplace_back(new RangeViewItem(range, time_lines_[0].get()));
  }
}


} // namespace WinUptime
