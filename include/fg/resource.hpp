#ifndef FG_RESOURCE_HPP_
#define FG_RESOURCE_HPP_

#include <memory>
#include <string>

#include <fg/realize.hpp>
#include <fg/resource_base.hpp>

namespace fg
{
template<typename description_type_, typename actual_type_>
class resource : public resource_base
{
public:
  using description_type = description_type_;
  using actual_type      = actual_type_     ;

  explicit resource  (const std::string& name = "", const description_type& description = description_type()) : resource_base(name), description_(description)
  {

  }
  resource           (const resource&  that) = default;
  resource           (      resource&& temp) = default;
  ~resource          ()                      = default;
  resource& operator=(const resource&  that) = default;
  resource& operator=(      resource&& temp) = default;

  const description_type& description() const
  {
    return description_;
  }
  actual_type*            actual     () const
  {
    return actual_.get();
  }

protected:
  void realize  () override
  {
    actual_.reset(fg::realize(description_));
  }
  void derealize() override
  {
    actual_.reset();
  }

  description_type             description_;
  std::unique_ptr<actual_type> actual_     ;
};
}

#endif