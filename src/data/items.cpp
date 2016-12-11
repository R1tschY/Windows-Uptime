#include "items.h"

namespace WinUptime {

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

Items::Items()
{ }

void Items::clear()
{
  ranges_.clear();
  spots_.clear();
}

void Items::merge(const Items& items)
{
  ranges_.insert(ranges_.end(), items.ranges_.begin(), items.ranges_.end());
  spots_.insert(spots_.end(), items.spots_.begin(), items.spots_.end());
}

void Items::addRange(const RangeItem& item)
{
  ranges_.push_back(item);
}

void Items::addRange(RangeItem&& item)
{
  ranges_.emplace_back(std::move(item));
}

void Item::setReduceText(const QString& text)
{

}

void Item::setReduceId(const QString& text)
{

}

void RangeItem::setRange(SystemTime start, SystemTime end)
{
  if (end < start)
  {
    throw std::invalid_argument("RangeItem::setRange");
  }

  start_ = start;
  end_ = end;
}

} // namespace WinUptime
