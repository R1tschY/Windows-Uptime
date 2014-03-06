#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QListWidget;
class QLabel;
class QPushButton;

namespace WinUptime {

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void OnYearBack();
  void OnYearForward();
  void OnMouthBack();
  void OnMouthForward();

private:
  QListWidget* listview_;

  // year widgets
  QLabel* year_lbl_;
  QPushButton* year_back_btn_;
  QPushButton* year_forward_btn_;

  // mouth widgets
  QLabel* mouth_lbl_;
  QPushButton* mouth_back_btn_;
  QPushButton* mouth_forward_btn_;
};

} // namespace WinUptime

#endif // MAINWINDOW_H
