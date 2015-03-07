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

#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "eventhandle.h"
#include "variant.h"
#include "event.h"

namespace WinUptime {

class RenderContext
{
public:
  RenderContext() noexcept:
    handle_()
  { }

  RenderContext(const wchar_t* query) :
    handle_()
  {
    openForValues(&query, 1);
  }

  RenderContext(std::initializer_list<const wchar_t*> queries) :
    handle_()
  {
    openForValues(queries.begin(), queries.size());
  }

  void openForValues(std::initializer_list<const wchar_t*> queries) {
    openForValues(queries.begin(), queries.size());
  }

  template<std::size_t N>
  void openForValues(const wchar_t* (&values)[N]) {
    openForValues(values, N);
  }

  template<std::size_t N>
  void getValues(const Event& event, Variant (&values)[N]) {
    getValues(event, values, N);
  }

  Variant getValue(const Event& event) {
    Variant value;
    getValues(event, &value, 1);
    return value;
  }

private:
  EventHandle handle_;

  void openForValues(const wchar_t* const* values, std::size_t n);
  void getValues(const Event& event, Variant* values, std::size_t n);
};

} // namespace WinUptime

#endif // RENDERCONTEXT_H
