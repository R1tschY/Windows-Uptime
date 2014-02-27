#ifndef EVENTLOG_H
#define EVENTLOG_H

#include "winversion.h"
#include <windows.h>
#include <winevt.h>

#include <string>
#include <vector>
#include <chrono>
#include <exception>

namespace WinUptime {

class WinException : std::exception {
public:
  explicit WinException(DWORD error_code);

  ~WinException() override;

  const char* what() const noexcept override;

  DWORD code() const noexcept;

private:
  DWORD error_code_;
  std::string message_;
};

class EventHandle final {
public:
  constexpr EventHandle() :
    handle_(nullptr)
  { }

  constexpr explicit EventHandle(EVT_HANDLE handle) noexcept :
    handle_(handle)
  { }

  EventHandle& operator=(const EventHandle&) = delete;
  EventHandle(const EventHandle&) = delete;

  EventHandle(EventHandle&& other) noexcept :
    handle_(other.handle_)
  {
    other.handle_ = nullptr;
  }

  EventHandle& operator=(EventHandle&& other) noexcept
  {
    handle_ = other.handle_;
    other.handle_ = nullptr;
  }

  ~EventHandle() noexcept {
    if (handle_ != nullptr) {
      EvtClose(handle_);
    }
  }

  bool operator==(decltype(nullptr)) {
    return handle_ == nullptr;
  }

  bool operator!=(decltype(nullptr)) {
    return handle_ != nullptr;
  }

  EVT_HANDLE get() const noexcept {
    return handle_;
  }

  explicit operator bool() const noexcept {
    return handle_ != nullptr;
  }

private:
  EVT_HANDLE handle_;
};

class ChannelPathIterator {
public:
  explicit ChannelPathIterator(EventHandle&& handle) :
    handle_(std::move(handle))
  { }

  bool next(std::wstring& path);

private:
  EventHandle handle_;
};

class EventIterator {
public:
  explicit EventIterator(EventHandle&& handle) :
    handle_(std::move(handle))
  { }

  bool next(std::vector<EventHandle>& events, int n,
            std::chrono::milliseconds timeout);

private:
  EventHandle handle_;
};

class Event {
public:

  ~Event();


private:
  EventHandle handle_;
};

class EventLog
{
public:
  EventLog();

  static ChannelPathIterator getChannelPaths();
  static EventIterator query(const std::wstring& path, const std::wstring& query,
                             DWORD flags);
  static EventIterator queryAll(const std::wstring& path, DWORD flags);

private:

};

} // namespace WinUptime

#endif // EVENTLOG_H
