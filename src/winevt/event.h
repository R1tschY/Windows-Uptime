#ifndef EVENT_H
#define EVENT_H

#include "eventhandle.h"

namespace WinUptime {

class Event {
public:
  explicit Event() noexcept :
    handle_(nullptr)
  {
  }

  explicit Event(EventHandle&& handle) noexcept :
    handle_(std::move(handle))
  { }

  explicit Event(Event&& other) noexcept :
    handle_(std::move(other.handle_))
  { }

  EVT_HANDLE getHandle() const {
    return handle_.get();
  }

private:
  EventHandle handle_;
};

} // namespace WinUptime

#endif // EVENT_H
