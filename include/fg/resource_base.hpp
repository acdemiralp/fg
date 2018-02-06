#ifndef FG_RESOURCE_BASE_HPP_
#define FG_RESOURCE_BASE_HPP_

#include <cstddef>
#include <string>

namespace fg
{
class framegraph;

class resource_base
{
public:
  explicit resource_base  (const std::string& name = "") : name_(name)
  {
    static std::size_t id = 0;
    id_ = id++;
  }
  resource_base           (const resource_base&  that) = default;
  resource_base           (      resource_base&& temp) = default;
  virtual ~resource_base  ()                           = default;
  resource_base& operator=(const resource_base&  that) = default;
  resource_base& operator=(      resource_base&& temp) = default;
  
  std::size_t        id      () const
  {
    return id_;
  }
                                                        
  const std::string& name    () const
  {
    return name_;
  }
  void               set_name(const std::string& name)
  {
    name_ = name;
  }
  
protected:
  friend framegraph;
  
  virtual void realize  () = 0;
  virtual void derealize() = 0;

  std::size_t id_  ;
  std::string name_;
};
}

#endif