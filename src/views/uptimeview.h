#ifndef UPTIMEVIEW_H
#define UPTIMEVIEW_H

#include <QWidget>

namespace WinUptime {

class ItemModel;

class UptimeView : public QWidget
{
  Q_OBJECT
public:
  explicit UptimeView(QWidget *parent = 0);

  ItemModel* model() const;
  void setModel(ItemModel* model);

protected slots:
  virtual void dataCleared();
  virtual void dataLoaded();

private:
  ItemModel* model_ = nullptr;
};

} // namespace WinUptime

#endif // UPTIMEVIEW_H
