#ifndef FG_RESOURCE_REGISTRY_HPP_
#define FG_RESOURCE_REGISTRY_HPP_

#include <tuple>
#include <vector>

namespace fg
{
template<typename... resource_types>
class resource_registry final
{
public:
  template<typename resource_type>
  const std::vector<resource_type>& get() const
  {
    return std::get<resource_type>(resources_);
  }
  template<typename resource_type>
  std::vector<resource_type>&       get()
  {
    return std::get<resource_type>(resources_);
  }
  
protected:
  std::tuple<std::vector<resource_types>...> resources_;
};
}

#endif