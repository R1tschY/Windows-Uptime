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

#ifndef UPTIMEREQUEST_H
#define UPTIMEREQUEST_H

#include <windows.h>

#include <deque>
#include <functional>
#include <QDateTime>

class QThread;

namespace WinUptime {

class SystemTime {
public:
  explicit SystemTime(uint64_t time) :
    time_(time)
  { }

  SystemTime(FILETIME time) :
    time_((uint64_t(time.dwHighDateTime) << 32) + time.dwLowDateTime)
  { }

  SystemTime(const QDateTime& time);

  uint64_t toUInt64() const {
    return time_;
  }

  FILETIME toFileTime() const {
    return FILETIME{uint32_t(time_), uint32_t(time_ >> 32)};
  }

  QDateTime toDateTime() const;

  bool operator < (const SystemTime& other) const { return time_ < other.time_;  }
  bool operator > (const SystemTime& other) const { return time_ > other.time_;  }

private:
  uint64_t time_;
};

struct PowerEvent {
public:
  enum class Type {
    BootUp,
    Shutdown,
    Suspend,
    Resume,
    SystemTimeTurnForth,
    SystemTimeTurnedBack
  };

  PowerEvent(Type type, ULONGLONG time, ULONGLONG time_needed = 0) :
    time_(time), time_needed_(time_needed), type_(type)
  { }

  void print() const;

  Type getType() const { return type_; }
  SystemTime getTime() const { return time_; }
  ULONGLONG getTimeNeeded() const { return time_needed_; }
  QString getTypeString() const;

private:
  SystemTime time_;
  ULONGLONG time_needed_;
  Type type_;
};

class UptimeRequest : public QObject {
  Q_OBJECT
public:
  UptimeRequest(QThread* thread);

  void loadLocal();
  void loadFromFile(const QString& path);
  void print();

  void forEachEventBetween(SystemTime start, SystemTime end,
                           std::function<void(const PowerEvent&)> visitor) const;

  const std::deque<PowerEvent>& getEvents() const { return database_; }

signals:
  void ready();
  void load(QString server);

private:
  std::deque<PowerEvent> database_;
  QThread* thread_;

private slots:
  void run(QString path);
};

} // namespace WinUptime

#endif // UPTIMEREQUEST_H
