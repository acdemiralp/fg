#ifndef FG_ACTUALIZE_HPP_
#define FG_ACTUALIZE_HPP_

#include <memory>
#include <type_traits>

namespace fg
{
template<typename description_type, typename actual_type>
struct missing_actualize_implementation : std::false_type { };

template<typename description_type, typename actual_type>
std::unique_ptr<actual_type> actualize(const description_type& description)
{
  static_assert(missing_actualize_implementation<description_type, actual_type>::value, "Missing actualize implementation for description - type pair.");
  return nullptr;
}
}

#endif