/*
 *  This file is part of Windows-Uptime.
 *
 *  Copyright (C) 2014-2015 R1tschY <r1tschy@yahoo.de>
 *
 *  Windows-Uptime is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TrafficIndicator is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "winexception.h"

#include <winevt.h>

#include <memory>
#include <QDebug>

namespace WinUptime {

Exception::~Exception() noexcept
{
}

namespace {

namespace Detail {
struct LocalDeleter {
  void operator()(void* ptr) {
    LocalFree(ptr);
  }
};
} // namespace Detail

template<typename T>
using LocalPtr = std::unique_ptr<T, Detail::LocalDeleter>;

LocalPtr<wchar_t> GetSystemErrorMessage(DWORD error_code)
{
  wchar_t* buffer;
  FormatMessageW(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
      FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      error_code,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (wchar_t*)&buffer,
      0, nullptr);
  return LocalPtr<wchar_t>(buffer);
}

std::wstring GetErrorCodeMessage(DWORD error_code) {
  std::wstring result;

  LocalPtr<wchar_t> buffer = GetSystemErrorMessage(error_code);
  if (buffer == nullptr) {
    return result;
  }

  result.assign(buffer.get());
  return result;
}

std::wstring GetQueryErrorMessage() {
  std::wstring result = GetErrorCodeMessage(ERROR_EVT_INVALID_QUERY);

  DWORD buffer_size;
  EvtGetExtendedStatus(0, nullptr, &buffer_size);
  if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
    wchar_t buffer[buffer_size];
    DWORD size;
    bool success = EvtGetExtendedStatus(sizeof(buffer), buffer, &size);
    if (success) {
      result.append(L": ");
      result.append(buffer, buffer + size);
    }
  }

  return result;
}

}

WinException::WinException(DWORD error_code) noexcept :
  Exception(error_code), message_(GetErrorCodeMessage(error_code))
{
}

WinException::WinException(const std::wstring& info, DWORD error_code) noexcept :
  Exception(error_code), message_(std::wstring(info) + L": " + GetErrorCodeMessage(error_code))
{
}

WinException::WinException(DWORD error_code, std::wstring message) noexcept :
  Exception(error_code), message_(std::move(message))
{
}

WinException::~WinException() noexcept
{
}

const wchar_t* WinException::what() const noexcept
{
  return message_.c_str();
}

InvalidQueryException::InvalidQueryException() noexcept :
  WinException(ERROR_EVT_INVALID_QUERY, GetQueryErrorMessage())
{
}

} // namespace WinUptime

