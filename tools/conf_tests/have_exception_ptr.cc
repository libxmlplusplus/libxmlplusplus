// Configuration-time test program, used in Meson build.
// Test whether std::exception_ptr, std::current_exception() and
// std::rethrow_exception() are defined.
// Corresponds to the M4 macro LIBXMLXX_CXX_HAS_EXCEPTION_PTR.

#include <exception>

int main()
{
  try
  {
    throw "custom error";
  }
  catch (...)
  {
    std::exception_ptr ep = std::current_exception();
    std::rethrow_exception(ep);
  }
  return 0;
}
