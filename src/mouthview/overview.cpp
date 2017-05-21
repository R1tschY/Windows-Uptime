#include "daystablemodel.h"
#include "overview.h"
#include "timelineitemdelegate.h"

#include <QPushButton>
#include <QMenu>
#include <QLabel>
#include <QTableView>
#include <QHBoxLayout>
#include <QIcon>
#include <QString>
#include <QHeaderView>
#include <QStackedLayout>
#include <QApplication>

#include "../data/itemmodel.h"
#include "../modules/powerstatemodule.h"
#include "../detailsview/detailsview.h"

namespace WinUptime {

static
QPushButton* CreateChooserButton(const QIcon& icon)
{
  auto btn = new QPushButton();
  btn->setIcon(icon);
  btn->setFlat(true);
  btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  btn->resize(32, 32);
  return btn;
}

static
QLabel* CreateHeaderLabel(const QString& text)
{
  auto lbl = new QLabel(text);
  lbl->setAlignment(Qt::AlignCenter);
  QFont font = lbl->font();
  font.setBold(true);
  font.setPointSize(10);
  lbl->setFont(font);
  return lbl;
}

Overview::Overview(QWidget* parent)
  : QWidget(parent)
{
  // current date
  date_ = QDate::currentDate();
  date_ = date_.addDays(1 - date_.day());

  // Main screen
  main_layout_ = new QStackedLayout();
  setLayout(main_layout_);
  main_screen_ = new QWidget();
  main_layout_->addWidget(main_screen_);
  auto layout = new QVBoxLayout(main_screen_);

  createHeader(layout);

  // Details view
  details_screen_ = new DetailsView();
  main_layout_->addWidget(details_screen_);
  connect(details_screen_, &DetailsView::exit, this, &Overview::showOverview);

  // backend
  createTable(layout);
}

ItemModel* Overview::model() const
{
  return model_;
}

void Overview::setModel(ItemModel* model)
{
  model_ = model;

  viewmodel_->setModel(model);
  update();
}

void Overview::createMenu()
{
  popup_menu_ = new QMenu();

//  auto eventLog = new QAction(tr("&Save event log ..."), this);
//  connect(eventLog, &QAction::triggered, this, &Overview::onSaveEventLog);
//  popup_menu_->addAction(eventLog);

//  auto uptimeLog = new QAction(tr("&Save uptime log ..."), this);
//  uptimeLog->setShortcuts(QKeySequence::Save);
//  connect(uptimeLog, &QAction::triggered, this, &Overview::onSaveUptimeLog);
//  popup_menu_->addAction(uptimeLog);

//  popup_menu_->addSeparator();

//  auto about = new QAction(tr("&About ..."), this);
//  connect(about, &QAction::triggered, this, &Overview::onAbout);
//  popup_menu_->addAction(about);

  auto aboutQt = new QAction(tr("&About Qt ..."), this);
  connect(aboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
  popup_menu_->addAction(aboutQt);
}

void Overview::createHeader(QBoxLayout* layout)
{
  QIcon arrow_left = QIcon(QStringLiteral(":/img/arrow-left.png"));
  QIcon arrow_right = QIcon(QStringLiteral(":/img/arrow-right.png"));
  QIcon menu_icon = QIcon(QStringLiteral(":/img/settings.png"));

  // Header
  QHBoxLayout* headerlayout = new QHBoxLayout();
  layout->addLayout(headerlayout);

  // Year chooser
  year_back_btn_ = CreateChooserButton(arrow_left);
  connect(year_back_btn_, &QPushButton::clicked, this, &Overview::onYearBack);
  headerlayout->addWidget(year_back_btn_);

  year_lbl_ = CreateHeaderLabel(QStringLiteral("year"));
  headerlayout->addWidget(year_lbl_);

  year_forward_btn_ = CreateChooserButton(arrow_right);
  connect(year_forward_btn_, &QPushButton::clicked, this, &Overview::onYearForward);
  headerlayout->addWidget(year_forward_btn_);

  // Mouth chooser
  mouth_back_btn_ = CreateChooserButton(arrow_left);
  connect(mouth_back_btn_, &QPushButton::clicked, this, &Overview::onMouthBack);
  headerlayout->addWidget(mouth_back_btn_);

  mouth_lbl_ = CreateHeaderLabel(QStringLiteral("month"));
  headerlayout->addWidget(mouth_lbl_);

  mouth_forward_btn_ = CreateChooserButton(arrow_right);
  connect(mouth_forward_btn_, &QPushButton::clicked, this, &Overview::onMouthForward);
  headerlayout->addWidget(mouth_forward_btn_);

  // Menu
  menu_btn_ = new QPushButton(menu_icon, QString("  "));
  menu_btn_->setFlat(true);
  menu_btn_->setFixedWidth(40);
  menu_btn_->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

  createMenu();
  menu_btn_->setMenu(popup_menu_);
  headerlayout->addWidget(menu_btn_);
}

void Overview::createTable(QBoxLayout* layout)
{
  viewmodel_ = new DaysTableModel(this);

  view_ = new QTableView(this);
  layout->addWidget(view_);
  view_->setModel(viewmodel_);
  view_->setSelectionBehavior(QAbstractItemView::SelectRows);
  view_->setShowGrid(false);

  auto *vheader = view_->verticalHeader();
  vheader->hide();
  vheader->setSectionResizeMode(QHeaderView::Fixed);
  vheader->setDefaultSectionSize(22);

  view_->setItemDelegateForColumn(1, new TimeLineItemDelegate(this));
  auto* header = view_->horizontalHeader();
  header->setSectionResizeMode(0, QHeaderView::Fixed);
  header->setSectionResizeMode(1, QHeaderView::Stretch);

  connect(view_, &QTableView::activated, this, &Overview::showDetails);
}

void Overview::showDetails(const QModelIndex& index)
{
  details_screen_->setItems(viewmodel_->getItems(index));
  main_layout_->setCurrentWidget(details_screen_);
}

void Overview::showOverview()
{
  main_layout_->setCurrentWidget(main_screen_);
}

void Overview::onYearBack()
{
  date_ = date_.addYears(-1);
  update();
}

void Overview::onYearForward()
{
  date_ = date_.addYears(1);
  update();
}

void Overview::onMouthBack()
{
  date_ = date_.addMonths(-1);
  update();
}

void Overview::onMouthForward()
{
  date_ = date_.addMonths(1);
  update();
}

void Overview::update()
{
  // general
  year_lbl_->setText(QString::number(date_.year()));
  mouth_lbl_->setText(QDate::longMonthName(date_.month(), QDate::DateFormat));

  // update event view
  QDate start = date_;
  QDate end = start.addMonths(1).addDays(-1);

  if (model_)
    model_->setRange(QDateTime(start), QDateTime(end));
}

} // namespace WinUptime
