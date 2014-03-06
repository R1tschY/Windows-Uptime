#include "mainwindow.h"

#include <QLayout>
#include <QListWidget>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QPushButton>
#include <QLabel>
#include <QSizePolicy>

#include <array>

#include "uptimerequest.h"
#include "winevt/winexception.h"

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
  connect(year_back_btn_, SIGNAL(clicked()), this, SLOT(OnYearBack()));
  ylayout->addWidget(year_back_btn_);

  year_lbl_ = new QLabel("Year");
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
  connect(year_forward_btn_, SIGNAL(clicked()), this, SLOT(OnYearForward()));
  ylayout->addWidget(year_forward_btn_);

  // Mouth chooser
  QHBoxLayout* mlayout = new QHBoxLayout();
  layout->addLayout(mlayout);

  mouth_back_btn_ = new QPushButton();
  mouth_back_btn_->setIcon(arrow_left);
  mouth_back_btn_->setFlat(true);
  mouth_back_btn_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  mouth_back_btn_->resize(32, 32);
  connect(mouth_back_btn_, SIGNAL(clicked()), this, SLOT(OnMouthBack()));
  mlayout->addWidget(mouth_back_btn_);

  mouth_lbl_ = new QLabel("Mouth");
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
  connect(mouth_forward_btn_, SIGNAL(clicked()), this, SLOT(OnMouthForward()));
  mlayout->addWidget(mouth_forward_btn_);

  //layout->addWidget(listview_);

  //setLayout(layout);

  // testing ...

  listview_ = new QListWidget();
  layout->addWidget(listview_);
  listview_->addItem("Channels:");

  try {
    UptimeRequest request;
    request.loadAll();

  } catch (WinException exp) {
    qDebug() << QString::fromWCharArray(exp.what());
  }
}

MainWindow::~MainWindow()
{

}

void MainWindow::OnYearBack()
{

}

void MainWindow::OnYearForward()
{

}

void MainWindow::OnMouthBack()
{

}

void MainWindow::OnMouthForward()
{

}

} // namespace WinUptime
