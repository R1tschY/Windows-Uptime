#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "eventhandle.h"
#include "variant.h"
#include "event.h"

#include <boost/utility/string_ref.hpp>

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
