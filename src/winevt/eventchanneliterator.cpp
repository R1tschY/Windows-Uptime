#include "eventchanneliterator.h"

namespace WinUptime {

bool EventChannelIterator::next(std::wstring& path)
{
  wchar_t buffer[1024];
  DWORD size;

  bool success = EvtNextChannelPath(handle_.get(), sizeof(buffer), buffer, &size);
  if (success) {
    path.assign(buffer, buffer + size);
    return true;

  } else {
    auto error = GetLastError();
    if (error == ERROR_INSUFFICIENT_BUFFER) {
      wchar_t bigbuffer[size];
      bool success = EvtNextChannelPath(handle_.get(), sizeof(bigbuffer), bigbuffer, &size);
      if (success) {
        path.assign(bigbuffer, bigbuffer + size);
        return true;
      }
    }

    return false;
  }
}

} // namespace WinUptime

