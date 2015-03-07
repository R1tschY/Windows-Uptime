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

#ifndef WINEXCEPTION_H
#define WINEXCEPTION_H

#include <windows.h>

#include <string>

#include "errors.h"

namespace WinUptime {

class Exception {
public:
  explicit Exception(DWORD error_code) noexcept :
    error_code_(error_code)
  { }

  virtual ~Exception() noexcept;

  virtual const wchar_t* what() const noexcept = 0;

  DWORD code() const noexcept {
    return error_code_;
  }

private:
  DWORD error_code_;
};

class WinException : Exception {
public:
  explicit WinException(DWORD error_code) noexcept;
  explicit WinException(const std::wstring& info, DWORD error_code) noexcept;
  explicit WinException(DWORD error_code, std::wstring message) noexcept;

  ~WinException() noexcept override;

  const wchar_t* what() const noexcept override;

private:
  std::wstring message_;
};

class InvalidQueryException : WinException {
public:
  explicit InvalidQueryException() noexcept;
};

} // namespace WinUptime

#endif // WINEXCEPTION_H
