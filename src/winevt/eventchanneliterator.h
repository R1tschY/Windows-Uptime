#ifndef CHANNELPATHITERATOR_H
#define CHANNELPATHITERATOR_H

#include "eventhandle.h"

namespace WinUptime {

class EventChannelIterator {
public:
  explicit EventChannelIterator(EventHandle&& handle) :
    handle_(std::move(handle))
  { }

  bool next(std::wstring& path);

private:
  EventHandle handle_;
};

} // namespace WinUptime

#endif // CHANNELPATHITERATOR_H
