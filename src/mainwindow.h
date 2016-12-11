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

#include <memory>
#include <QMainWindow>
#include <QDate>
#include <QThread>

class QTableView;
class QLabel;
class QPushButton;
class QAction;
class QWidget;
class QStackedLayout;
class QBoxLayout;
class QProgressBar;

namespace WinUptime {

class ItemModel;
class UptimeView;
class PowerStateModule;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  // Menu widget
  QPushButton* menu_btn_ = nullptr;
  QMenu* popup_menu_ = nullptr;

  // year widgets
  QLabel* year_lbl_ = nullptr;
  QPushButton* year_back_btn_ = nullptr;
  QPushButton* year_forward_btn_ = nullptr;

  // mouth widgets
  QLabel* mouth_lbl_ = nullptr;
  QPushButton* mouth_back_btn_ = nullptr;
  QPushButton* mouth_forward_btn_ = nullptr;

  // current date
  QDate date_;

  // Main widgets
  QStackedLayout* main_layout_ = nullptr;
  QWidget* welcome_screen_ = nullptr;
  QWidget* main_screen_ = nullptr;
  QWidget* progress_screen_ = nullptr;

  // Progress
  QLabel* progress_label_ = nullptr;
  QProgressBar* progress_bar_ = nullptr;

  // Backend
  ItemModel* model_ = nullptr;
  std::unique_ptr<PowerStateModule> power_state_;

  // View
  UptimeView* view_ = nullptr;

  // internal state
  bool loaded_ = false;

  void update();
  void loadDatabase();

  void createMenu();
  void createWelcomeScreen();
  void createProgressView();
  void createHeader(QBoxLayout* layout);
  void createBackend(QBoxLayout* layout);

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
