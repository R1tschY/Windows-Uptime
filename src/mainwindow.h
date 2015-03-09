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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QThread>

#include "uptimerequest.h"

class QTableView;
class QLabel;
class QPushButton;
class QAction;
class QWidget;
class QStackedLayout;
class QBoxLayout;
class QProgressBar;

namespace WinUptime {

class EventModel;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  EventModel* model_;

  // Menu widget
  QPushButton* menu_btn_;
  QMenu* popup_menu_;

  // year widgets
  QLabel* year_lbl_;
  QPushButton* year_back_btn_;
  QPushButton* year_forward_btn_;

  // mouth widgets
  QLabel* mouth_lbl_;
  QPushButton* mouth_back_btn_;
  QPushButton* mouth_forward_btn_;

  // current date
  QDate date_;

  // Database
  UptimeRequest* database_;

  // Main widgets
  QStackedLayout* main_layout_;
  QWidget* welcome_screen_;
  QWidget* main_screen_;
  QWidget* progress_screen_;

  // View widgets
  QTableView* table_view_;

  // Progress
  QLabel* progress_label_;
  QProgressBar* progress_bar_;
  QThread worker_thread_;

  void updateView();
  void loadDatabase();

  void createMenu();
  void createWelcomeScreen();
  void createTableView(QBoxLayout* layout);
  void createProgressView();
  void createHeader(QBoxLayout* layout);

private slots:
  void onYearBack();
  void onYearForward();
  void onMouthBack();
  void onMouthForward();

  void onSaveEventLog();
  void onSaveUptimeLog();

  void onAbout();

  void onLoadLocal();
  void onLoadFile();

  void onDatabaseLoaded();
};

} // namespace WinUptime

#endif // MAINWINDOW_H
