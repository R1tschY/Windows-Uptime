#ifndef DETAILSMODEL_H
#define DETAILSMODEL_H

#include <QAbstractTableModel>

namespace WinUptime {

class SpotItem;

class DetailsModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit DetailsModel(QObject *parent = 0);
  ~DetailsModel();

  // Header:
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  void setSpots(const std::vector<SpotItem>& spots);

private:
  enum Column {
    ColumnTime,
    ColumnIcon,
    ColumnName,

    Columns
  };

  std::vector<SpotItem> spots_;
};

} // namespace WinUptime

#endif // DETAILSMODEL_H
