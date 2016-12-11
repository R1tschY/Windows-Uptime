/*
 *  This file is part of Windows-Uptime.
 *
 *  Copyright (C) 2014-2015 R1tschY <r1tschy@yahoo.de>
 *
 *  Windows-Uptime is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TrafficIndicator is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#include <QMenu>
#include <QFileDialog>
#include <QStackedLayout>
#include <QProgressBar>
#include <QSplitter>
#include <QApplication>

#include <array>

#include "winevt/winexception.h"
#include "utils.h"
#include "views/rowsview.h"
#include "data/itemmodel.h"
#include "modules/powerstatemodule.h"

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

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent)
{
  setWindowTitle(QStringLiteral("Windows - Uptime"));
  setWindowIcon(QIcon(QStringLiteral(":/img/app.png")));

  // current date
  date_ = QDate::currentDate();
  date_ = date_.addDays(1 - date_.day());

  // design
  resize(500, 400);
  auto main_widget = new QWidget();
  main_layout_ = new QStackedLayout(main_widget);

  createWelcomeScreen();
  createProgressView();

  // Main screen
  main_screen_ = new QWidget();
  main_layout_->addWidget(main_screen_);
  auto layout = new QVBoxLayout(main_screen_);

  createHeader(layout);

  // backend
  createBackend(layout);

  // finsh layout
  main_layout_->setCurrentWidget(welcome_screen_);
  setCentralWidget(main_widget);


  update();
}

void MainWindow::createMenu()
{
  popup_menu_ = new QMenu();

  auto eventLog = new QAction(tr("&Save event log ..."), this);
  connect(eventLog, SIGNAL(triggered()), this, SLOT(onSaveEventLog()));
  popup_menu_->addAction(eventLog);

  auto uptimeLog = new QAction(tr("&Save uptime log ..."), this);
  uptimeLog->setShortcuts(QKeySequence::Save);
  connect(uptimeLog, SIGNAL(triggered()), this, SLOT(onSaveUptimeLog()));
  popup_menu_->addAction(uptimeLog);

  popup_menu_->addSeparator();

  auto about = new QAction(tr("&About ..."), this);
  connect(about, SIGNAL(triggered()), this, SLOT(onAbout()));
  popup_menu_->addAction(about);

  auto aboutQt = new QAction(tr("&About Qt ..."), this);
  connect(aboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
  popup_menu_->addAction(aboutQt);
}

void MainWindow::createWelcomeScreen()
{
  QIcon local_icon = QIcon(QStringLiteral(":/img/local.png"));
  QIcon local_file = QIcon(QStringLiteral(":/img/file.png"));

  welcome_screen_ = new QWidget();
  QVBoxLayout* wslayout = new QVBoxLayout(welcome_screen_);
  main_layout_->addWidget(welcome_screen_);

  auto button_list = new QWidget();
  QVBoxLayout* bllayout = new QVBoxLayout();
  button_list->setLayout(bllayout);
  wslayout->addWidget(button_list, 0, Qt::AlignCenter);

  QPushButton *openLocal = new QPushButton(local_icon, tr("Load from local computer"));
  openLocal->setIconSize(QSize(32, 32));
  openLocal->setStyleSheet(QStringLiteral("padding: 10px; font-size: 13px;"));
  connect(openLocal, SIGNAL(clicked()), this, SLOT(onLoadLocal()));
  bllayout->addWidget(openLocal);

  QPushButton *openFile = new QPushButton(local_file, tr("Load from file ..."));
  openFile->setIconSize(QSize(32, 32));
  openFile->setStyleSheet(QStringLiteral("padding: 10px; font-size: 13px;"));
  connect(openFile, SIGNAL(clicked()), this, SLOT(onLoadFile()));
  bllayout->addWidget(openFile);
}

void MainWindow::createProgressView()
{
  progress_screen_ = new QWidget();
  auto progress_view_layout = new QVBoxLayout(progress_screen_);

  auto center_widget = new QWidget();
  auto center_layout = new QVBoxLayout(center_widget);
  progress_view_layout->addWidget(center_widget, 0, Qt::AlignCenter);

  progress_label_ = new QLabel(tr("Loading ..."));
  progress_label_->setStyleSheet(QStringLiteral("font-size: 13px;"));

  progress_bar_ = new QProgressBar();
  progress_bar_->setRange(0,0);
  progress_bar_->setFixedWidth(250);

  center_layout->addWidget(progress_label_);
  center_layout->addWidget(progress_bar_);

  main_layout_->addWidget(progress_screen_);
}

void MainWindow::createHeader(QBoxLayout* layout)
{
  QIcon arrow_left = QIcon(QStringLiteral(":/img/arrow-left.png"));
  QIcon arrow_right = QIcon(QStringLiteral(":/img/arrow-right.png"));
  QIcon menu_icon = QIcon(QStringLiteral(":/img/settings.png"));

  // Header
  QHBoxLayout* headerlayout = new QHBoxLayout();
  layout->addLayout(headerlayout);

  // Year chooser
  year_back_btn_ = CreateChooserButton(arrow_left);
  connect(year_back_btn_, SIGNAL(clicked()), this, SLOT(onYearBack()));
  headerlayout->addWidget(year_back_btn_);

  year_lbl_ = CreateHeaderLabel(QStringLiteral("year"));
  headerlayout->addWidget(year_lbl_);

  year_forward_btn_ = CreateChooserButton(arrow_right);
  connect(year_forward_btn_, SIGNAL(clicked()), this, SLOT(onYearForward()));
  headerlayout->addWidget(year_forward_btn_);

  // Mouth chooser
  mouth_back_btn_ = CreateChooserButton(arrow_left);
  connect(mouth_back_btn_, SIGNAL(clicked()), this, SLOT(onMouthBack()));
  headerlayout->addWidget(mouth_back_btn_);

  mouth_lbl_ = CreateHeaderLabel(QStringLiteral("month"));
  headerlayout->addWidget(mouth_lbl_);

  mouth_forward_btn_ = CreateChooserButton(arrow_right);
  connect(mouth_forward_btn_, SIGNAL(clicked()), this, SLOT(onMouthForward()));
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

void MainWindow::createBackend(QBoxLayout* layout)
{
  model_ = new ItemModel(this);
  power_state_.reset(new PowerStateModule());
  model_->addSubModel(power_state_->createModel(this));

  view_ = new RowsView(this);
  view_->setModel(model_);

  connect(model_, SIGNAL(endLoading()), this, SLOT(onDatabaseLoaded()));

  layout->addWidget(view_);
}

MainWindow::~MainWindow()
{

}

void MainWindow::onYearBack()
{
  date_ = date_.addYears(-1);
  update();
}

void MainWindow::onYearForward()
{
  date_ = date_.addYears(1);
  update();
}

void MainWindow::onMouthBack()
{
  date_ = date_.addMonths(-1);
  update();
}

void MainWindow::onMouthForward()
{
  date_ = date_.addMonths(1);
  update();
}

void MainWindow::onSaveEventLog()
{
//  QString save_path = QFileDialog::getSaveFileName(
//            this,
//            tr("Save event log"));
//  if (save_path.length() == 0) return;

//  QFile file(save_path);
//  if (!file.open(QIODevice::WriteOnly)) {
//      QMessageBox::information(this, tr("Unable to open file"),
//          file.errorString());
//      return;
//  }

//  QTextStream out(&file);
//  out << "datetime;event;argument\n";
//  for (auto&& event : database_->getEvents()) {
//    out << event.getTime().toDateTime().toString(Qt::RFC2822Date)
//        << ";" << event.getTypeString()
//        << ";" << event.getTimeNeeded() / (10*1000) << "ms"
//        << "\n";
//  }
}

void MainWindow::onSaveUptimeLog()
{
//  QString save_path = QFileDialog::getSaveFileName(
//            this,
//            tr("Save uptime log"));
//  if (save_path.length() == 0) return;

//  QFile file(save_path);
//  if (!file.open(QIODevice::WriteOnly)) {
//      QMessageBox::information(this, tr("Unable to open file"),
//          file.errorString());
//      return;
//  }

//  QTextStream out(&file);
//  out << "day of mouth;uptime;ontime\n";
//  for (auto&& row : model_->getRows()) {
//    out << std::get<0>(row) << ";" << std::get<1>(row).toString() << ";" << std::get<2>(row).toString() << "\n";
//  }
}

void MainWindow::onAbout()
{
  QMessageBox::about(this,
                     tr("About Windows - Uptime"),
                     tr("<b>Windows - Uptime</b> is a application to get information when a windows computer was on.<br /><br />"
                        "Version " STRINGIFY(PACKAGE_VERSION) "<br />"
                        "Copyright &copy; 2014 - 2015  R1tschY &lt;r1tschy@yahoo.de&gt;<br /><br />"
                        "This program is free software: you can redistribute it and/or modify "
                        "it under the terms of the GNU General Public License as published by "
                        "the Free Software Foundation, either version 3 of the License, or "
                        "(at your option) any later version.<br />"
                        "<br />"
                        "All icons from <a href=\"http://www.iconsdb.com\">ICONSDB.COM</a>"));
}

void MainWindow::onLoadLocal()
{
  main_layout_->setCurrentWidget(progress_screen_);
  model_->setLocalhostAsSource();
}

void MainWindow::onLoadFile()
{
  QString file_path = QFileDialog::getOpenFileName(
        this,
        tr("Open log"),
        QString(), tr("Event log (*.evt *.evtx *.etl);; All files (*.*)"));
  if (file_path.isEmpty())
    return;

  main_layout_->setCurrentWidget(progress_screen_);
  model_->setFileSource(file_path);
}

void MainWindow::onDatabaseLoaded()
{
  loaded_ = true;
  main_layout_->setCurrentWidget(main_screen_);
}

void MainWindow::update()
{
  // general
  year_lbl_->setText(QString::number(date_.year()));
  mouth_lbl_->setText(QDate::longMonthName(date_.month(), QDate::DateFormat));

  if (!loaded_)
    return;

  // update event view
  QDate start = date_;
  QDate end = start.addMonths(1).addDays(-1);

  qDebug() << "updateView" << start << " -> " << end;
  model_->setRange(QDateTime(start), QDateTime(end));
}

} // namespace WinUptime
