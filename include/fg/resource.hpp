#ifndef FG_RESOURCE_HPP_
#define FG_RESOURCE_HPP_

#include <memory>
#include <string>
#include <variant>

#include <fg/realize.hpp>
#include <fg/resource_base.hpp>

namespace fg
{
class render_task_base;

template<typename description_type_, typename actual_type_>
class resource : public resource_base
{
public:
  using description_type = description_type_;
  using actual_type      = actual_type_     ;

  explicit resource  (const std::string& name, const render_task_base* creator, const description_type& description) 
  : resource_base(name, creator), description_(description), actual_(std::unique_ptr<actual_type>())
  {
    // Transient (normal) constructor.
  }
  explicit resource  (const std::string& name,                                  const description_type& description, actual_type* actual = nullptr) 
  : resource_base(name, nullptr), description_(description), actual_(actual)
  {
    // Retained (import) constructor.
    if(!actual) actual_ = fg::realize<description_type, actual_type>(description_);
  }
  resource           (const resource&  that) = delete ;
  resource           (      resource&& temp) = default;
  ~resource          ()                      = default;
  resource& operator=(const resource&  that) = delete ;
  resource& operator=(      resource&& temp) = default;

  const description_type& description () const
  {
    return description_;
  }
  actual_type*            actual      () const // If transient, only valid through the realized interval of the resource.
  {
    return std::holds_alternative<std::unique_ptr<actual_type>>(actual_) ? std::get<std::unique_ptr<actual_type>>(actual_).get() : std::get<actual_type*>(actual_);
  }

protected:
  void realize  () override
  {
    if (transient()) std::get<std::unique_ptr<actual_type>>(actual_) = fg::realize<description_type, actual_type>(description_);
  }
  void derealize() override
  {
    if (transient()) std::get<std::unique_ptr<actual_type>>(actual_).reset();
  }

  description_type                                         description_;
  std::variant<std::unique_ptr<actual_type>, actual_type*> actual_     ;
};
}

#endif