#ifndef FG_RESOURCE_REGISTRY_HPP_
#define FG_RESOURCE_REGISTRY_HPP_

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <vector>

namespace fg
{
template<typename... resource_types>
class resource_registry
{
public:
  virtual ~resource_registry() = default;

  template<typename type>
  const std::vector<type>& get() const
  {
    return find<0, type, std::tuple<std::vector<resource_types>...>, is_same<0, type>::value>::get(resources_);
  }
  template<typename type>
  std::vector<type>&       get()
  {
    return find<0, type, std::tuple<std::vector<resource_types>...>, is_same<0, type>::value>::get(resources_);
  }
  
protected:
  template<std::size_t index, typename type>
  struct is_same : std::is_same<type, typename std::tuple_element<index, std::tuple<std::vector<resource_types>...>>::type::value_type>
  {
    
  };
  template<std::size_t index, typename type, typename tuple, bool match = false>
  struct find
  {
    static std::vector<type>& get(tuple& value)
    {
      return find<index + 1, type, tuple, is_same<index + 1, type>::value>::get(value);
    }
  };
  template<std::size_t index, typename type, typename tuple>
  struct find<index, type, tuple, true>
  {
    static std::vector<type>& get(tuple& value)
    {
      return std::get<index>(value);
    }
  };
  
  std::tuple<std::vector<resource_types>...> resources_;
};
}

#endif