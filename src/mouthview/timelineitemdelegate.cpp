#include "timelineitemdelegate.h"

#include <QStyleOptionViewItem>
#include <QPainter>
#include <QDebug>
#include <QApplication>

#include "daystablemodel.h"
#include "../data/items.h"

namespace WinUptime {

static int msecsOfDay(const QDateTime& dateTime)
{
  return dateTime.time().msecsSinceStartOfDay();
}

TimeLineItemDelegate::TimeLineItemDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{ }

void TimeLineItemDelegate::paint(
    QPainter* painter,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
  auto widget = option.widget;
  QStyle *style = widget ? widget->style() : QApplication::style();

  style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, widget);

  auto* model = qobject_cast<const DaysTableModel*>(index.model());
  if (model == nullptr)
    return;

  painter->save();
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(Qt::NoPen);

  double width = option.rect.width();
  double height = option.rect.height();
  double msec2x = width / (24. * 3600. * 1000.);

  auto& item = model->getItems(index);
  for (auto& range : item.ranges())
  {
    auto start = msecsOfDay(range.start());
    auto end = msecsOfDay(range.end());
    painter->fillRect(
          QRectF(
            option.rect.left() + start * msec2x,
            option.rect.top(),
            (end - start) * msec2x,
            height
          ),
          QBrush(range.color()));
  }

  painter->restore();

  if (option.state & QStyle::State_HasFocus)
  {
    QStyleOptionFocusRect o;
    o.QStyleOption::operator=(option);
    o.state |= QStyle::State_KeyboardFocusChange;
    o.state |= QStyle::State_Item;
    QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled)
                  ? QPalette::Normal : QPalette::Disabled;
    o.backgroundColor = option.palette.color(
          cg,
          (option.state & QStyle::State_Selected)
          ? QPalette::Highlight : QPalette::Window
    );
    style->drawPrimitive(
          QStyle::PE_FrameFocusRect,
          &o,
          painter,
          widget);
  }
}

QWidget* TimeLineItemDelegate::createEditor(
    QWidget*,
    const QStyleOptionViewItem&,
    const QModelIndex&) const
{
  return 0;
}

} // namespace WinUptime
