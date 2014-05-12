#include "uptimerequest.h"

#include "winevt/eventlog.h"
#include "winevt/rendercontext.h"

#include <QDebug>
#include <QDateTime>

namespace WinUptime {

UptimeRequest::UptimeRequest()
{
}

void WinUptime::UptimeRequest::loadAll()
{
  EventLog log = EventLog::getLocalLog();
  EventIterator iter = log.queryChannel(
        L"System",
        L"*[System/Provider[@Name=\"Microsoft-Windows-Kernel-General\"]]");

  RenderContext id_context(L"Event/System/EventID");
  RenderContext resume_context({
                                 L"Event/EventData/Data[@Name=\"OldTime\"]",
                                 L"Event/EventData/Data[@Name=\"NewTime\"]"
                               });
  RenderContext boot_up_context(L"Event/EventData/Data[@Name=\"StartTime\"]");
  RenderContext shutdown_context(L"Event/EventData/Data[@Name=\"StopTime\"]");

  Event event;
  while (iter.next(event)) {
    Variant result = id_context.getValue(event);

    const uint16_t* eventid = result.getUInt16();
    if (eventid) {
      switch (*eventid) {
      case 1: {
        Variant results[2];
        resume_context.getValues(event, results);
        uint64_t old_time = *results[0].getFileTime();
        uint64_t new_time = *results[1].getFileTime();

        // mindestens eine Sekunde Abstand
        if (new_time < old_time ||  new_time - old_time < 10*1000*1000)
          break;

        database_.emplace_back(
              PowerEvent::Type::SUSPEND,
              old_time);
        database_.emplace_back(
              PowerEvent::Type::RESUME,
              new_time);
        break;
      }

      case 12:
        database_.emplace_back(
              PowerEvent::Type::BOOT_UP,
              *boot_up_context.getValue(event).getFileTime());
        break;

      case 13:
        database_.emplace_back(
              PowerEvent::Type::SHUTDOWN,
              *shutdown_context.getValue(event).getFileTime());
        break;
      }
    }
  }
}

void UptimeRequest::print()
{
  for (const auto& entry : database_) {
    entry.print();
  }
}

void UptimeRequest::forEachEventBetween(
    SystemTime start, SystemTime end,
    std::function<void (PowerEvent)> visitor)
{
  // do better with std::sort, std::lower_bound and std::upper_bound
  for (auto i = database_.crbegin(); i != database_.crend(); i++) {
    const auto& event = *i;
    if (event.time > start && event.time < end) {
      visitor(event);
    }
  }
}

void PowerEvent::print() const
{
  qDebug() << QString::fromWCharArray(getTypeString())
           << " @" << time.toDateTime().toString(Qt::RFC2822Date);
}

const wchar_t* PowerEvent::getTypeString() const
{
  switch (type) {
  case Type::BOOT_UP:
    return L"Boot up";

  case Type::SHUTDOWN:
    return L"Shutdown";

  case Type::SUSPEND:
    return L"Suspend";

  case Type::RESUME:
    return L"Resume";

  default:
    return L"";
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
