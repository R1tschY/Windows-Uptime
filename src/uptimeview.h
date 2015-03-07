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

#ifndef LOG_H
#define LOG_H

#include <QTabWidget>
#include <QAbstractTableModel>
#include <QTime>

#include <tuple>
#include <string>
#include <vector>

class QTableView;

namespace WinUptime {

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

} // namespace WinUptime

#endif // LOG_H
