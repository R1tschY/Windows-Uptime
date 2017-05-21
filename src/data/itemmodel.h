#ifndef WINUPTIME_ITEMMODEL_H
#define WINUPTIME_ITEMMODEL_H

#include <windows.h>

#include <vector>
#include <QDateTime>
#include <QObject>
#include <QString>
#include <QTime>
#include <QFuture>
#include <QFutureWatcher>

#include "../winevt/eventlog.h"

#include "items.h"

namespace WinUptime {

class Module;
class ModuleModel;

class ItemModel : public QObject
{
  Q_OBJECT
public:
  ItemModel(QObject* parent = 0);
  ~ItemModel();

  const Items& getItems() const;

  void addSubModel(ModuleModel* model);

  void setFileSource(const QString& file);
  void setLocalhostAsSource();

  void setRange(QDateTime start, QDateTime end);
  QDateTime getStart() const;
  QDateTime getEnd() const;

  void update();


signals:
  void beginLoading();
  void endLoading();

  void sourceChanged();
  void rangeChanged();

private slots:
  void onItemsReady();

private:
  Items items_;
  QFuture<Items> items_future_; // TODO: refactor me out of here
  QFutureWatcher<Items> watcher_; // TODO: refactor me out of here

  std::unique_ptr<EventLog> log_;
  QDateTime start_;
  QDateTime end_;

  std::vector<ModuleModel*> models_;

  static Items load(EventLog&, std::vector<ModuleModel*>, QDateTime, QDateTime);
};

} // namespace WinUptime

#endif // WINUPTIME_ITEMMODEL_H
