#include "itemmodel.h"

#include <QtConcurrent>

#include "../modules/module.h"

namespace WinUptime {

ItemModel::ItemModel(QObject* parent)
  : QObject(parent), log_(EventLog::getLocalChannel(L"System"))
{
  connect(&watcher_, SIGNAL(finished()), this, SLOT(onItemsReady()));
}

ItemModel::~ItemModel()
{
}

const Items& ItemModel::getItems() const
{
  return items_;
}

void ItemModel::addSubModel(ModuleModel* model)
{
  models_.push_back(model);
}

void ItemModel::setFileSource(const QString& file)
{
  log_ = EventLog::getFileLog(file.toStdWString());
  emit sourceChanged();

  update();
}

void ItemModel::setLocalhostAsSource()
{
  log_ = EventLog::getLocalChannel(L"System");
  emit sourceChanged();

  update();
}

void ItemModel::setRange(QDateTime start, QDateTime end)
{
  start_ = start;
  end_ = end;

  update();
}

void ItemModel::update()
{
  emit beginLoading();

  items_.clear();
  items_future_ = QtConcurrent::run([=](){ return load(log_, models_, start_, end_); });
  watcher_.setFuture(items_future_);
}

void ItemModel::onItemsReady()
{
  // TODO: check if future has changed

  items_ = items_future_.result();

  emit endLoading();
}

Items ItemModel::load(EventLog& log,
                      std::vector<ModuleModel*> models,
                      QDateTime start, QDateTime end)
{
  qDebug()<< __PRETTY_FUNCTION__ << __LINE__;
  Items items;
  for (ModuleModel* model : models)
  {
     items.merge(model->scanRange(start, end, log));
  }
  return std::move(items);
}

} // namespace WinUptime
