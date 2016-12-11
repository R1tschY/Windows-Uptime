#ifndef WINUPTIME_ROWSVIEW_H
#define WINUPTIME_ROWSVIEW_H

#include <memory>

#include "uptimeview.h"

class QGraphicsView;
class QGraphicsScene;
class QGraphicsItem;

namespace WinUptime {

class RowsView : public UptimeView
{
  Q_OBJECT
public:
  RowsView(QWidget *parent = 0);
  ~RowsView();

protected slots:
  void dataCleared() override;
  void dataLoaded() override;

private:
  QGraphicsView* view_ = nullptr;
  QGraphicsScene* scene_ = nullptr;

  std::vector<std::unique_ptr<QGraphicsItem>> items_;
  std::vector<std::unique_ptr<QGraphicsItem>> time_lines_;
};

} // namespace WinUptime

#endif // WINUPTIME_ROWSVIEW_H
