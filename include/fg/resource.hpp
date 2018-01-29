#ifndef FG_RESOURCE_HPP_
#define FG_RESOURCE_HPP_

#include <cstddef>
#include <optional>

namespace fg
{
template <typename type>
class resource
{
public:
  resource           ()
  {
    static std::size_t id = 0;
    id_ = id++;
  }
  resource           (const resource&  that) = delete ;
  resource           (      resource&& temp) = default;
  virtual ~resource  ()                      = default;
  resource& operator=(const resource&  that) = delete ;
  resource& operator=(      resource&& temp) = default;

  virtual void realize() = 0;

protected:
  std::size_t         id_         ;
  std::optional<type> realization_;
};
}

#endif