#ifndef FG_RESOURCE_HPP_
#define FG_RESOURCE_HPP_

#include <memory>

#include <fg/actualize.hpp>
#include <fg/resource_base.hpp>

namespace fg
{
template<typename description_type_, typename actual_type_>
class resource : public resource_base
{
public:
  using description_type = description_type_;
  using actual_type      = actual_type_     ;

  explicit resource  (const description_type& description = description_type()) : resource_base(), description_(description)
  {

  }
  resource           (const resource&  that) = default;
  resource           (      resource&& temp) = default;
  ~resource          ()                      = default;
  resource& operator=(const resource&  that) = default;
  resource& operator=(      resource&& temp) = default;

  const description_type&      description() const
  {
    return description_;
  }
  std::unique_ptr<actual_type> actualize  ()
  {
    return fg::actualize(description_);
  }

protected:
  description_type description_;
};
}

#endif