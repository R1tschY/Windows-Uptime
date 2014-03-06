#ifndef UPTIMEREQUEST_H
#define UPTIMEREQUEST_H

#include <windows.h>

#include <deque>

namespace WinUptime {

struct PowerEvent {
  enum class Type {
    BOOT_UP,
    SHUTDOWN,
    SUSPEND,
    RESUME
  };

  Type type;
  FILETIME time;

  PowerEvent(Type type, FILETIME time) :
    type(type), time(time)
  { }
};

class UptimeRequest
{
public:
  UptimeRequest();

  void loadAll();

private:
  std::deque<PowerEvent> database_;
};

} // namespace WinUptime

#endif // UPTIMEREQUEST_H
