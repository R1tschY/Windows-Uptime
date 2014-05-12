#include "mainwindow.h"

#include <QLayout>
#include <QTableView>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QPushButton>
#include <QLabel>
#include <QSizePolicy>
#include <QHeaderView>

#include <array>

#include "winevt/winexception.h"
#include "uptimeview.h"

namespace WinUptime {

static QDateTime FromFileTime(FILETIME filetime) {
  SYSTEMTIME system_time;
  QDateTime result;
  bool success = FileTimeToSystemTime(&filetime, &system_time);
  if (success) {
    result.setDate(QDate(
                     system_time.wYear,
                     system_time.wMonth,
                     system_time.wDay));
    result.setTime(QTime(
                     system_time.wHour,
                     system_time.wMinute,
                     system_time.wSecond,
                     system_time.wMilliseconds));
  }
  return result;
}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent)
{
  // current date
  date_ = QDate::currentDate();

  // design
  QVBoxLayout* layout = new QVBoxLayout();
  QWidget* main_widget = new QWidget();
  main_widget->setLayout(layout);
  setCentralWidget(main_widget);

  QIcon arrow_left = QIcon(":/img/arrow-left.png");
  QIcon arrow_right = QIcon(":/img/arrow-right.png");

  // Year chooser
  QHBoxLayout* ylayout = new QHBoxLayout();
  layout->addLayout(ylayout);

  year_back_btn_ = new QPushButton();
  year_back_btn_->setIcon(arrow_left);
  year_back_btn_->setFlat(true);
  year_back_btn_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  year_back_btn_->resize(32, 32);
  connect(year_back_btn_, SIGNAL(clicked()), this, SLOT(onYearBack()));
  ylayout->addWidget(year_back_btn_);

  year_lbl_ = new QLabel("year");
  year_lbl_->setAlignment(Qt::AlignCenter);
  QFont year_font = year_lbl_->font();
  year_font.setBold(true);
  year_font.setPointSize(10);
  year_lbl_->setFont(year_font);
  ylayout->addWidget(year_lbl_);

  year_forward_btn_ = new QPushButton();
  year_forward_btn_->setIcon(arrow_right);
  year_forward_btn_->setFlat(true);
  year_forward_btn_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  year_forward_btn_->resize(32, 32);
  connect(year_forward_btn_, SIGNAL(clicked()), this, SLOT(onYearForward()));
  ylayout->addWidget(year_forward_btn_);

  // Mouth chooser
  QHBoxLayout* mlayout = new QHBoxLayout();
  layout->addLayout(mlayout);

  mouth_back_btn_ = new QPushButton();
  mouth_back_btn_->setIcon(arrow_left);
  mouth_back_btn_->setFlat(true);
  mouth_back_btn_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  mouth_back_btn_->resize(32, 32);
  connect(mouth_back_btn_, SIGNAL(clicked()), this, SLOT(onMouthBack()));
  mlayout->addWidget(mouth_back_btn_);

  mouth_lbl_ = new QLabel("month");
  mouth_lbl_->setAlignment(Qt::AlignCenter);
  QFont mouth_font = mouth_lbl_->font();
  mouth_font.setBold(true);
  mouth_font.setPointSize(10);
  mouth_lbl_->setFont(mouth_font);
  mlayout->addWidget(mouth_lbl_);

  mouth_forward_btn_ = new QPushButton();
  mouth_forward_btn_->setIcon(arrow_right);
  mouth_forward_btn_->setFlat(true);
  mouth_forward_btn_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  mouth_forward_btn_->resize(32, 32);
  connect(mouth_forward_btn_, SIGNAL(clicked()), this, SLOT(onMouthForward()));
  mlayout->addWidget(mouth_forward_btn_);

  // testing ...

  table_ = new QTableView();
  model_ = new EventModel();

  table_->setModel(model_);
  table_->setSelectionBehavior(QAbstractItemView::SelectRows);
  table_->setSelectionMode(QAbstractItemView::SingleSelection);
  table_->setWordWrap(true);
  table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table_->setShowGrid(false);

  QHeaderView* vheader = table_->verticalHeader();
  vheader->setHidden(true);

  QHeaderView* hheader = table_->horizontalHeader();
  hheader->setFrameShape(QFrame::NoFrame);
  hheader->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  hheader->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  hheader->setSectionResizeMode(2, QHeaderView::ResizeToContents);

  layout->addWidget(table_);

  //

  loadDatabase();

  updateView();
}

MainWindow::~MainWindow()
{

}

void MainWindow::onYearBack()
{
  date_ = date_.addYears(-1);
  updateView();
}

void MainWindow::onYearForward()
{
  date_ = date_.addYears(1);
  updateView();
}

void MainWindow::onMouthBack()
{
  date_ = date_.addMonths(-1);
  updateView();
}

void MainWindow::onMouthForward()
{
  date_ = date_.addMonths(1);
  updateView();
}

void MainWindow::loadDatabase()
{
  try {
    database_.loadAll();

  } catch (WinException exp) {
    qDebug() << QString::fromWCharArray(exp.what());
  }
}

enum class PowerState {
  On,
  Off,
  Suspended,
  Unknown
};

class DayUptimeCalculator {
public:
  DayUptimeCalculator(EventModel* model):
    model_(model), last_day_(1), uptime_(0, 0), ontime_(0, 0)
  { }

  void operator()(PowerEvent event) {
    QDateTime date = event.time.toDateTime();
    unsigned day = date.date().day();

    if (day != last_day_) {
      int remainingMs = 24*3600*1000 - last_time_.msecsSinceStartOfDay();

      switch (event.type) {
      case PowerEvent::Type::BOOT_UP:
        model_->addRow(last_day_, uptime_, ontime_);
        break;

      case PowerEvent::Type::SUSPEND:
      case PowerEvent::Type::SHUTDOWN:
        uptime_ = uptime_.addMSecs(remainingMs);
        ontime_ = ontime_.addMSecs(remainingMs);
        model_->addRow(last_day_, uptime_, ontime_);
        break;

      case PowerEvent::Type::RESUME:
        ontime_ = ontime_.addMSecs(remainingMs);
        model_->addRow(last_day_, uptime_, ontime_);
        break;
      }

      for (unsigned i = last_day_ + 1; i < day; i++) {
        switch (event.type) {
        case PowerEvent::Type::BOOT_UP:
          model_->addRow(i, QTime(0, 0, 0), QTime(0, 0, 0));
          break;

        case PowerEvent::Type::SUSPEND:
        case PowerEvent::Type::SHUTDOWN:
          model_->addRow(i, QTime(24, 0, 0), QTime(24, 0, 0));
          break;

        case PowerEvent::Type::RESUME:
          model_->addRow(i, QTime(0, 0, 0), QTime(24, 0, 0));
          break;
        }
      }

      uptime_ = QTime(0,0);
      ontime_ = QTime(0,0);
      last_day_ = day;
    }

    int newMs = last_time_.msecsTo(date.time());
    switch (event.type) {
    case PowerEvent::Type::SUSPEND:
    case PowerEvent::Type::SHUTDOWN:
      uptime_ = uptime_.addMSecs(newMs);
      ontime_ = ontime_.addMSecs(newMs);
      break;

    case PowerEvent::Type::RESUME:
      ontime_ = ontime_.addMSecs(newMs);
      break;
    }
    last_time_ = date.time();

    switch (event.type) {
    case PowerEvent::Type::SUSPEND:
      state_ = PowerState::Suspended;
      break;

    case PowerEvent::Type::SHUTDOWN:
      state_ = PowerState::Off;
      break;

    case PowerEvent::Type::BOOT_UP:
    case PowerEvent::Type::RESUME:
      state_ = PowerState::On;
      break;
    }
  }

  void finish(unsigned last_day_of_mouth) {
    int remainingMs = 24*3600*1000 - last_time_.msecsSinceStartOfDay();

    switch (state_) {
    case PowerState::Off:
      model_->addRow(last_day_, uptime_, ontime_);
      break;

    case PowerState::On:
      uptime_ = uptime_.addMSecs(remainingMs);
      ontime_ = ontime_.addMSecs(remainingMs);
      model_->addRow(last_day_, uptime_, ontime_);
      break;

    case PowerState::Suspended:
      ontime_ = ontime_.addMSecs(remainingMs);
      model_->addRow(last_day_, uptime_, ontime_);
      break;
    }

    for (unsigned i = last_day_ + 1; i < last_day_of_mouth; i++) {
      switch (state_) {
      case PowerState::Off:
        model_->addRow(i, QTime(0, 0, 0), QTime(0, 0, 0));
        break;

      case PowerState::On:
        model_->addRow(i, QTime(24, 0, 0), QTime(24, 0, 0));
        break;

      case PowerState::Suspended:
        model_->addRow(i, QTime(0, 0, 0), QTime(24, 0, 0));
        break;
      }
    }
  }

private:
  PowerState state_ = PowerState::Unknown;
  EventModel* model_;
  unsigned last_day_;
  QTime last_time_;
  QTime uptime_;
  QTime ontime_;
};

void MainWindow::updateView()
{
  // general
  year_lbl_->setText(QString::number(date_.year()));
  mouth_lbl_->setText(QDate::longMonthName(date_.month(), QDate::DateFormat));
  model_->clear();

  // calc
  DayUptimeCalculator c(model_);

  QDate begin_date(date_.year(), date_.month(), 1);
  QDate end_date = begin_date.addMonths(1);  
  database_.forEachEventBetween(
        SystemTime(QDateTime(begin_date, QTime(0, 0))),
        SystemTime(QDateTime(end_date, QTime(0, 0))),
        [&] (PowerEvent event) {
    event.print();
    c(event);
  });
  c.finish(end_date.addDays(-1).day());

  // the end
  model_->update();
}

} // namespace WinUptime
