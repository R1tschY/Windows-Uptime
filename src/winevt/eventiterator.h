#ifndef EVENTITERATOR_H
#define EVENTITERATOR_H

#include <vector>
#include <chrono>

#include "event.h"

namespace WinUptime {

class EventIterator {
public:
  explicit EventIterator(EventHandle&& handle) :
    handle_(std::move(handle))
  { }

  bool next(Event& event);

private:
  EventHandle handle_;
};

} // namespace WinUptime

#endif // EVENTITERATOR_H
