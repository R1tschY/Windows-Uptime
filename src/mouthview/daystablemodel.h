#ifndef MOUTHTABLEMODEL_H
#define MOUTHTABLEMODEL_H

#include <vector>
#include <QAbstractTableModel>

namespace WinUptime {

class ItemModel;
class Items;
struct DaysTableModelEntry;

class DaysTableModel : public QAbstractTableModel
{
  Q_OBJECT

public:  
  explicit DaysTableModel(QObject *parent = 0);
  ~DaysTableModel();

  // Header:
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  // Basic functionality:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  const Items& getItems(const QModelIndex& index) const;

  ItemModel* getModel() const;
  void setModel(ItemModel* model);

private:
  enum Column {
    ColumnDay,
    ColumnTimeline,
    ColumnUptime,

    NumberOfColumns
  };

  ItemModel* model_ = nullptr;
  std::vector<DaysTableModelEntry> days_;

  void onDataChanged();

};

} // namespace WinUptime

#endif // MOUTHTABLEMODEL_H
