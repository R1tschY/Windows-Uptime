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

#include "rendercontext.h"

#include <QDebug>

#include "winexception.h"

namespace WinUptime {

void RenderContext::openForValues(const wchar_t* const* values, std::size_t n)
{
  handle_.reset(EvtCreateRenderContext(n, const_cast<const wchar_t**>(values), EvtRenderEventValues));
  if (!handle_)
  {
      throw WinException(L"EvtCreateRenderContext", GetLastError());
  }
}

void RenderContext::getValues(const Event& event, Variant* values, std::size_t n)
{
  EVT_HANDLE handle = handle_.get();
  DWORD used = 0;
  DWORD properties = 0;
  bool success = EvtRender(
        handle,
        event.getHandle(),
        EvtRenderEventValues,
        n * sizeof(EVT_VARIANT),
        values,
        &used,
        &properties);

  if (!success) {
    throw WinException(L"EvtRender", GetLastError());
  }

  if (n * sizeof(EVT_VARIANT) > used || n != properties) {
    throw WinException(L"RenderContext::getValues", ERROR_BAD_ARGUMENTS);
  }
}

} // namespace WinUptime
