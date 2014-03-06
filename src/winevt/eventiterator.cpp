#include "eventiterator.h"

#include "winexception.h"

namespace WinUptime {

bool EventIterator::next(Event& event)
{
  DWORD size;

  bool success = EvtNext(handle_.get(), 1, (void**)&event,
                         INFINITE, 0, &size);
  if (success) {
    return size == 1;

  } else {
    DWORD error = GetLastError();
    if (error == ERROR_NO_MORE_ITEMS) {
      return false;
    }

    throw WinException(L"EvtNext: ", error);
  }
}

} // namespace WinUptime
