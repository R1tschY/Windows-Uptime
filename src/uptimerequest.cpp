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

#include "uptimerequest.h"

#include <cstdint>
#include <algorithm>
#include <QDebug>
#include <QDateTime>
#include <QTime>
#include <QThread>
#include <QDir>

#include "winevt/eventlog.h"
#include "winevt/rendercontext.h"
#include "winevt/winexception.h"

namespace WinUptime {

UptimeRequest::UptimeRequest(QThread* thread) :
  thread_(thread)
{
  moveToThread(thread);
  connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
  connect(this, SIGNAL(load(QString)), this, SLOT(run(QString)));
}

void UptimeRequest::loadLocal()
{
  thread_->start();
  emit load(QString());
}

void UptimeRequest::loadFromFile(const QString& path)
{
  thread_->start();
  emit load(path);
}

void UptimeRequest::print()
{
  for (const auto& entry : database_) {
    entry.print();
  }
}

void UptimeRequest::forEachEventBetween(SystemTime start, SystemTime end,
    std::function<void(const PowerEvent&)> visitor) const
{
  // do better with std::sort, std::lower_bound and std::upper_bound
  for (auto i = database_.crbegin(); i != database_.crend(); i++) {
    const auto& event = *i;
    if (event.getTime() > start && event.getTime() < end) {
      visitor(event);
    }
  }
}

void UptimeRequest::run(QString path)
{
  try {
    EventLog log = (path.isEmpty()) ?
          EventLog::getLocalChannel(L"System") :
          EventLog::getFileLog(QDir::toNativeSeparators(path).toStdWString());

    RenderContext id_context(L"Event/System/EventID");
    Event event;

    EventIterator kernel_general = log.query(L"*[System/Provider[@Name=\"Microsoft-Windows-Kernel-General\"]]");
    RenderContext time_changed_context({
                                   L"Event/EventData/Data[@Name=\"OldTime\"]",
                                   L"Event/EventData/Data[@Name=\"NewTime\"]"
                                 });
    RenderContext boot_up_context(L"Event/EventData/Data[@Name=\"StartTime\"]");
    RenderContext shutdown_context(L"Event/EventData/Data[@Name=\"StopTime\"]");
    while (kernel_general.next(event)) {
      Variant result = id_context.getValue(event);

      const uint16_t* eventid = result.getUInt16();
      if (eventid) {
        switch (*eventid) {
        case 1: {
          Variant results[2];
          time_changed_context.getValues(event, results);
          uint64_t old_time = *results[0].getFileTimeChecked();
          uint64_t new_time = *results[1].getFileTimeChecked();

          if (old_time < new_time) {
            database_.emplace_back(
                  PowerEvent::Type::SystemTimeTurnForth,
                  old_time,
                  new_time - old_time);
          } else {
            database_.emplace_back(
                  PowerEvent::Type::SystemTimeTurnedBack,
                  old_time,
                  old_time - new_time);
          }
          break;
        }

        case 12:
          database_.emplace_back(
                PowerEvent::Type::BootUp,
                *boot_up_context.getValue(event).getFileTimeChecked());
          break;

        case 13:
          database_.emplace_back(
                PowerEvent::Type::Shutdown,
                *shutdown_context.getValue(event).getFileTimeChecked());
          break;
        }
      }
    }

    EventIterator kernel_power = log.query(L"*[System/Provider[@Name=\"Microsoft-Windows-Kernel-Power\"]]");
    RenderContext time_created_context(L"Event/System/TimeCreated/@SystemTime");
    while (kernel_power.next(event)) {
      Variant result = id_context.getValue(event);

      const uint16_t* eventid = result.getUInt16();
      if (eventid && *eventid == 42) {
        database_.emplace_back(
              PowerEvent::Type::Suspend,
              *time_created_context.getValue(event).getFileTimeChecked());
      }
    }

    EventIterator power_troubleshooting = log.query(L"*[System/Provider[@Name=\"Microsoft-Windows-Power-Troubleshooter\"]]");
    RenderContext wake_time(L"Event/EventData/Data[@Name=\"WakeTime\"]");
    while (power_troubleshooting.next(event)) {
      Variant result = id_context.getValue(event);

      const uint16_t* eventid = result.getUInt16();
      if (eventid && *eventid == 1) {
        database_.emplace_back(
              PowerEvent::Type::Resume,
              *wake_time.getValue(event).getFileTimeChecked());
      }
    }

    std::sort(database_.begin(), database_.end(),
              [](const PowerEvent& a, const PowerEvent& b) -> bool {
      return a.getTime() > b.getTime();
    });

    qDebug() << "Database size: " << database_.size() * sizeof(PowerEvent)
             << "Database memory usage: " << (((database_.size()-1)/512)+1) * 512 * sizeof(PowerEvent);

    emit ready();

  }
  catch (WinException exp)
  {
    qDebug() << QString::fromWCharArray(exp.what());
    emit ready();
  }
  catch (std::exception exp)
  {
    qDebug() << QString(exp.what());
    emit ready();
  }
}

void PowerEvent::print() const
{
  qDebug() << getTypeString()
           << " @" << time_.toDateTime().toString(Qt::RFC2822Date);
}

QString PowerEvent::getTypeString() const
{
  switch (type_) {
  case Type::BootUp:
    return QStringLiteral("Boot up");

  case Type::Shutdown:
    return QStringLiteral("Shutdown");

  case Type::Suspend:
    return QStringLiteral("Suspend");

  case Type::Resume:
    return QStringLiteral("Resume");

  case Type::SystemTimeTurnForth:
    return QStringLiteral("SystemTimeTurnForth");

  case Type::SystemTimeTurnedBack:
    return QStringLiteral("SystemTimeTurnedBack");

  default:
    return QString();
  }
}

SystemTime::SystemTime(const QDateTime& t)
{
  SYSTEMTIME system_time;
  QDate date = t.date();
  system_time.wDay = date.day();
  system_time.wMonth = date.month();
  system_time.wYear = date.year();
  QTime time = t.time();
  system_time.wHour = time.hour();
  system_time.wMinute = time.minute();
  system_time.wSecond = time.second();
  system_time.wMilliseconds = time.msec();

  FILETIME file_time;
  bool success = SystemTimeToFileTime(&system_time, &file_time);
  if (success) {
    time_ = (uint64_t(file_time.dwHighDateTime) << 32) + file_time.dwLowDateTime;
  }
  else {
    time_ = 0;
  }
}

QDateTime SystemTime::toDateTime() const
{
  SYSTEMTIME system_time;
  QDateTime result;
  FILETIME file_time = toFileTime();
  bool success = FileTimeToSystemTime(&file_time, &system_time);
  if (success) {
    result.setTimeSpec(Qt::UTC);
    result.setDate(QDate(
                     system_time.wYear,
                     system_time.wMonth,
                     system_time.wDay));
    result.setTime(QTime(
                     system_time.wHour,
                     system_time.wMinute,
                     system_time.wSecond,
                     system_time.wMilliseconds));
  }
  return result.toLocalTime();
}

} // namespace WinUptime
