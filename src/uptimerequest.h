#ifndef UPTIMEREQUEST_H
#define UPTIMEREQUEST_H

#include <windows.h>

#include <deque>
#include <functional>
#include <QDateTime>

namespace WinUptime {

class SystemTime {
public:
  explicit SystemTime(uint64_t time) :
    time_(time)
  { }

  explicit SystemTime(FILETIME time) :
    time_((uint64_t(time.dwHighDateTime) << 32) + time.dwLowDateTime)
  { }

  explicit SystemTime(const QDateTime& time);

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
    BOOT_UP,
    SHUTDOWN,
    SUSPEND,
    RESUME
  };

  Type type;
  SystemTime time;

  PowerEvent(Type type, ULONGLONG time) :
    type(type), time(time)
  { }

  void print() const;

  const wchar_t* getTypeString() const;
};

class UptimeRequest
{
public:
  UptimeRequest();

  void loadAll();
  void print();

  void forEachEventBetween(SystemTime start, SystemTime end,
                           std::function<void(PowerEvent)> visitor);

private:
  std::deque<PowerEvent> database_;
};

} // namespace WinUptime

#endif // UPTIMEREQUEST_H
