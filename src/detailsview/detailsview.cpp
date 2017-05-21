#include "detailsview.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>

#include "detailsmodel.h"
#include "../data/items.h"

namespace WinUptime {

DetailsView::DetailsView(QWidget *parent)
  : QWidget(parent)
{
  table_ = new QTableView(this);
  model_ = new DetailsModel(this);
  table_->setModel(model_);
  table_->setSelectionBehavior(QAbstractItemView::SelectRows);
  table_->setShowGrid(false);
  table_->verticalHeader()->hide();
  auto* tableheader = table_->horizontalHeader();
  tableheader->setSectionResizeMode(0, QHeaderView::Fixed);
  table_->setColumnWidth(0, table_->fontMetrics().width("00:00") + 6);
  tableheader->setSectionResizeMode(1, QHeaderView::Fixed);
  table_->setColumnWidth(1, 28);
  table_->setIconSize(QSize(24, 24));
  tableheader->setSectionResizeMode(2, QHeaderView::Stretch);

  auto* page = new QVBoxLayout();
  setLayout(page);

  auto* header = new QHBoxLayout();
  page->addLayout(header, 0);
  createHeader(header);

  auto* container = new QHBoxLayout();
  page->addLayout(container);
  createContainer(container);
}

void DetailsView::setItems(const Items& items)
{
  model_->setSpots(items.spots());
}

void DetailsView::createHeader(QHBoxLayout* layout)
{
  auto* exitButton = new QPushButton();
  layout->addWidget(exitButton, 0);
  exitButton->setIcon(QIcon(QStringLiteral(":/img/arrow-left.png")));
  exitButton->setFlat(true);
  connect(exitButton, &QPushButton::clicked, this, &DetailsView::exit);

  layout->addStretch(1);
}

void DetailsView::createContainer(QHBoxLayout* layout)
{
  layout->addWidget(table_);
}

} // namespace WinUptime
