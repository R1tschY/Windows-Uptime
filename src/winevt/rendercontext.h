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

  template<std::size_t N>
  void openForValues(const wchar_t* (&values)[N]) {
    openForValues(values, N);
  }

  template<std::size_t N>
  void getValues(const Event& event, Variant (&values)[N]) {
    getValues(event, values, N);
  }

private:
  EventHandle handle_;

  void openForValues(const wchar_t** values, std::size_t n);
  void getValues(const Event& event, Variant* values, std::size_t n);
};

} // namespace WinUptime

#endif // RENDERCONTEXT_H
