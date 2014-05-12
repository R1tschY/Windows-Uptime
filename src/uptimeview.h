#ifndef LOG_H
#define LOG_H

#include <QTabWidget>
#include <QAbstractTableModel>
#include <QTime>

#include <tuple>
#include <string>
#include <vector>

class QTableView;

class EventModel : public QAbstractTableModel {
  Q_OBJECT

public:
  enum class Column {
    Day,
    Uptime,
    Ontime,

    Max
  };

  enum DataColumn {
    DataColumn_Day,
    DataColumn_Uptime,
    DataColumn_Ontime,
  };

  typedef std::tuple<unsigned, QTime, QTime> Row;
  typedef std::vector<Row> Rows;

  EventModel(QObject* parent = 0);

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;

  QVariant data(const QModelIndex& index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex& index) const;
  bool setData(const QModelIndex&, const QVariant&, int = Qt::EditRole);

  void addRow(unsigned day, QTime uptime, QTime ontime);
  void clear();
  bool update();

private:
  Rows rows_;
};

#endif // LOG_H
