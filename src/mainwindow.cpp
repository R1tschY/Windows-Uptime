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
#include "data/itemmodel.h"
#include "modules/powerstatemodule.h"
#include "mouthview/daystablemodel.h"
#include "mouthview/overview.h"
#include "detailsview/detailsview.h"
#include "mouthview/timelineitemdelegate.h"

namespace WinUptime {

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent)
{
  setWindowTitle(QStringLiteral("Windows - Uptime"));
  setWindowIcon(QIcon(QStringLiteral(":/img/app.png")));

  // design
  resize(500, 400);
  auto main_widget = new QWidget();
  main_layout_ = new QStackedLayout(main_widget);

  createBackend();

  createWelcomeScreen();
  createProgressView();
  createOverview();

  // finsh layout
  main_layout_->setCurrentWidget(welcome_screen_);
  setCentralWidget(main_widget);
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
  connect(openLocal, &QPushButton::clicked, this, &MainWindow::onLoadLocal);
  bllayout->addWidget(openLocal);

  QPushButton *openFile = new QPushButton(local_file, tr("Load from file ..."));
  openFile->setIconSize(QSize(32, 32));
  openFile->setStyleSheet(QStringLiteral("padding: 10px; font-size: 13px;"));
  connect(openFile, &QPushButton::clicked, this, &MainWindow::onLoadFile);
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
  progress_bar_->setRange(0, 0);
  progress_bar_->setFixedWidth(250);

  center_layout->addWidget(progress_label_);
  center_layout->addWidget(progress_bar_);

  main_layout_->addWidget(progress_screen_);

  connect(model_, &ItemModel::beginLoading, this, &MainWindow::onBeginDatabaseLoading);
  connect(model_, &ItemModel::endLoading, this, &MainWindow::onEndDatabaseLoading);
}

void MainWindow::createOverview()
{
  main_screen_ = new Overview();
  main_layout_->addWidget(main_screen_);
  main_screen_->setModel(model_);
}

MainWindow::~MainWindow() = default;

void MainWindow::createBackend()
{
  model_ = new ItemModel(this);
  loading_model_ = new ItemModel(this);
  power_state_.reset(new PowerStateModule());
  model_->addSubModel(power_state_->createModel(this));
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
                        "Version " PACKAGE_VERSION "<br />"
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

  model_->setFileSource(file_path);
}

void MainWindow::onBeginDatabaseLoading()
{
  loaded_ = false;
  main_layout_->setCurrentWidget(progress_screen_);
}

void MainWindow::onEndDatabaseLoading()
{
  loaded_ = true;
  main_layout_->setCurrentWidget(main_screen_);
}

} // namespace WinUptime
