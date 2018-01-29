#ifndef FG_RESOURCE_HPP_
#define FG_RESOURCE_HPP_

#include <optional>
#include <type_traits>

namespace fg
{
template <typename resource_type>
class resource
{
  virtual ~resource() = default;
  
  resource_type resource_;
};
}

#endif