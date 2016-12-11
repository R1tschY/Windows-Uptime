#include "uptimeview.h"

#include "../data/itemmodel.h"

namespace WinUptime {

UptimeView::UptimeView(QWidget *parent) : QWidget(parent)
{

}

ItemModel* UptimeView::model() const
{
  return model_;
}

void UptimeView::setModel(ItemModel* model)
{
  if (model_ == model)
    return;

  if (model_)
  {
    disconnect(model_, SIGNAL(beginLoading()), this, SLOT(dataCleared()));
    disconnect(model_, SIGNAL(endLoading()), this, SLOT(dataLoaded()));
  }

  model_ = model;

  if (model_)
  {
    connect(model_, SIGNAL(beginLoading()), this, SLOT(dataCleared()));
    connect(model_, SIGNAL(endLoading()), this, SLOT(dataLoaded()));
  }
}

void UptimeView::dataCleared()
{

}

void UptimeView::dataLoaded()
{

}

} // namespace WinUptime
