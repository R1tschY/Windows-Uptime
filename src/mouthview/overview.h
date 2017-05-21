#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <memory>
#include <QWidget>
#include <QDate>

class QPushButton;
class QMenu;
class QLabel;
class QTableView;
class QBoxLayout;
class QStackedLayout;

namespace WinUptime {

class ItemModel;
class DaysTableModel;
class PowerStateModule;
class DetailsView;

class Overview : public QWidget
{
  Q_OBJECT
public:
  Overview(QWidget* parent = nullptr);


  ItemModel* model() const;
  void setModel(ItemModel* model);

private:
  QWidget* main_screen_ = nullptr;
  QStackedLayout* main_layout_ = nullptr;

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

  // View
  QTableView* view_ = nullptr;
  DaysTableModel* viewmodel_ = nullptr;
  DetailsView* details_screen_ = nullptr;

  // current date
  QDate date_;

  // Model
  ItemModel* model_ = nullptr;

  void createHeader(QBoxLayout* layout);
  void createTable(QBoxLayout* layout);
  void createTable();

  void showDetails(const QModelIndex& index);
  void showOverview();
  void onYearBack();
  void onYearForward();
  void onMouthBack();
  void onMouthForward();
  void update();
  void createMenu();
};

} // namespace WinUptime

#endif // OVERVIEW_H
