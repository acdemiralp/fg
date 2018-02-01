#ifndef FG_RESOURCE_HPP_
#define FG_RESOURCE_HPP_

#include <optional>

#include <fg/resource_base.hpp>

namespace fg
{
template <typename actual_type>
class resource : public resource_base
{
public:
  resource           ()                      = default;
  resource           (const resource&  that) = delete ;
  resource           (      resource&& temp) = default;
  virtual ~resource  ()                      = default;
  resource& operator=(const resource&  that) = delete ;
  resource& operator=(      resource&& temp) = default;
  
  actual_type* actual()
  {
    return actual_ ? &actual_.value() : nullptr;
  }

protected:
  std::optional<actual_type> actual_;
};
}

#endif
