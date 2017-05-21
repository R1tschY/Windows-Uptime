#include "itemmodel.h"

#include <QtConcurrent>

#include "../modules/module.h"

namespace WinUptime {

ItemModel::ItemModel(QObject* parent)
  : QObject(parent), log_()
{
  connect(&watcher_, SIGNAL(finished()), this, SLOT(onItemsReady()));
}

ItemModel::~ItemModel() = default;

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
  log_ = std::make_unique<EventLog>(EventLog::getFileLog(file.toStdWString()));
  emit sourceChanged();

  update();
}

void ItemModel::setLocalhostAsSource()
{
  log_ = std::make_unique<EventLog>(EventLog::getLocalChannel(L"System"));
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
  if (!log_)
    return;

  emit beginLoading();

  items_.clear();
  items_future_ = QtConcurrent::run([=](){
    return load(*log_.get(), models_, start_, end_);
  });
  watcher_.setFuture(items_future_);
}

void ItemModel::onItemsReady()
{
  if (!items_future_.isFinished())
    return;

  items_ = items_future_.result();

  emit endLoading();
}

QDateTime ItemModel::getEnd() const
{
    return end_;
}

QDateTime ItemModel::getStart() const
{
    return start_;
}

Items ItemModel::load(EventLog& log,
                      std::vector<ModuleModel*> models,
                      QDateTime start, QDateTime end)
{
  Items items;
  for (ModuleModel* model : models)
  {
     items.merge(model->scanRange(start, end, log));
  }
  return items;
}

} // namespace WinUptime
