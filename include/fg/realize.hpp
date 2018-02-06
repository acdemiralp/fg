#ifndef FG_REALIZE_HPP_
#define FG_REALIZE_HPP_

#include <memory>
#include <type_traits>

namespace fg
{
template<typename description_type, typename actual_type>
struct missing_realize_implementation : std::false_type {};

template<typename description_type, typename actual_type>
std::unique_ptr<actual_type> realize(const description_type& description)
{
  static_assert(missing_realize_implementation<description_type, actual_type>::value, "Missing realize implementation for description - type pair.");
  return nullptr;
}
}

#endif