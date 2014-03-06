#ifndef EVENTHANDLE_H
#define EVENTHANDLE_H

#include <windows.h>
#include <winevt.h>

#include <memory>

namespace WinUptime {

struct EventHandle_finalizer {
  void operator() (EVT_HANDLE handle) const noexcept {
    EvtClose(handle);
  }
};

using EventHandle = std::unique_ptr<
  std::remove_pointer<EVT_HANDLE>::type,
  EventHandle_finalizer>;

} // namespace WinUptime

#endif // EVENTHANDLE_H
