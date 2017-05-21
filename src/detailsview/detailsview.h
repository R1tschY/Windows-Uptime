#ifndef DETAILSVIEW_H
#define DETAILSVIEW_H

#include <QWidget>

class QTableView;
class QHBoxLayout;

namespace WinUptime {

class Items;
class DetailsModel;

class DetailsView : public QWidget
{
  Q_OBJECT
public:
  explicit DetailsView(QWidget *parent = 0);

  void setItems(const Items& items);

signals:
  void exit();

public slots:

private:
  QTableView* table_;
  DetailsModel* model_;

  void createHeader(QHBoxLayout* layout);
  void createContainer(QHBoxLayout* layout);
};

} // namespace WinUptime

#endif // DETAILSVIEW_H
