#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>

#include "uptimerequest.h"

class QTableView;
class QLabel;
class QPushButton;
class EventModel;

namespace WinUptime {

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  QTableView* table_;
  EventModel* model_;

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
  UptimeRequest database_;

  void updateView();
  void loadDatabase();

private slots:
  void onYearBack();
  void onYearForward();
  void onMouthBack();
  void onMouthForward();
};

} // namespace WinUptime

#endif // MAINWINDOW_H
