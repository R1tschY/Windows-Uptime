#ifndef TIMELINEITEMDELEGATE_H
#define TIMELINEITEMDELEGATE_H

#include <QStyledItemDelegate>

namespace WinUptime {

class TimeLineItemDelegate : public QStyledItemDelegate
{
public:
  TimeLineItemDelegate(QObject *parent = 0);

  void paint(
      QPainter* painter,
      const QStyleOptionViewItem& option,
      const QModelIndex& index) const override;

  QWidget* createEditor(QWidget* parent,
          const QStyleOptionViewItem&,
          const QModelIndex &index) const override;
};

} // namespace WinUptime

#endif // TIMELINEITEMDELEGATE_H
