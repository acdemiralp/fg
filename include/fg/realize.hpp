#ifndef FG_REALIZE_HPP_
#define FG_REALIZE_HPP_

#include <memory>
#include <type_traits>

namespace fg
{
template <typename real_type, typename virtual_type>
struct missing_realize_implementation : std::false_type { };

template <typename real_type, typename virtual_type>
std::unique_ptr<real_type> realize(const virtual_type& resource)
{
  static_assert(missing_realize_implementation<real_type, virtual_type>::value, "Missing realize implementation for resource.");
  return nullptr;
}
}

#endif