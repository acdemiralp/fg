#ifndef FG_RESOURCE_BASE_HPP_
#define FG_RESOURCE_BASE_HPP_

#include <cstddef>

namespace fg
{
class resource_base
{
public:
  explicit resource_base  ()
  {
    static std::size_t id = 0;
    id_ = id++;
  }
  resource_base           (const resource_base&  that) = default;
  resource_base           (      resource_base&& temp) = default;
  ~resource_base          ()                           = default;
  resource_base& operator=(const resource_base&  that) = default;
  resource_base& operator=(      resource_base&& temp) = default;

  std::size_t id() const
  {
    return id_;
  }

protected:
  std::size_t id_;
};
}

#endif