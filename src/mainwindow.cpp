#include "winversion.h"
#include "mainwindow.h"

#include <QLayout>
#include <QListWidget>
#include <QMessageBox>

#include "eventlog.h"

namespace WinUptime {

static bool NextChannelPath(EVT_HANDLE channel_enum, std::wstring& path)
{

}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent)
{
  // design
  //QVBoxLayout* layout = new QVBoxLayout();

  listview_ = new QListWidget();
  setCentralWidget(listview_);
  //layout->addWidget(listview_);

  //setLayout(layout);

  // testing ...

  listview_->addItem("Channels:");

  auto iter = EventLog::getChannelPaths();
  std::wstring channel_name;
  while (iter.next(channel_name)) {
    listview_->addItem(QString::fromStdWString(channel_name));
  }
}

MainWindow::~MainWindow()
{

}

} // namespace WinUptime
