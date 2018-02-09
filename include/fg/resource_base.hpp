#ifndef FG_RESOURCE_BASE_HPP_
#define FG_RESOURCE_BASE_HPP_

#include <cstddef>
#include <string>
#include <vector>

namespace fg
{
class framegraph;
class render_task_base;
class render_task_builder;

class resource_base
{
public:
  explicit resource_base  (const std::string& name, const render_task_base* creator) : name_(name), creator_(creator), ref_count_(0)
  {
    static std::size_t id = 0;
    id_ = id++;
  }
  resource_base           (const resource_base&  that) = delete ;
  resource_base           (      resource_base&& temp) = default;
  virtual ~resource_base  ()                           = default;
  resource_base& operator=(const resource_base&  that) = delete ;
  resource_base& operator=(      resource_base&& temp) = default;
                             
  std::size_t        id       () const
  {
    return id_;
  }                                                      
                              
  const std::string& name     () const
  {
    return name_;
  }
  void               set_name (const std::string& name)
  {
    name_ = name;
  }

  bool               transient() const
  {
    return creator_ != nullptr;
  }

protected:
  friend framegraph;
  friend render_task_builder;

  virtual void realize  () = 0;
  virtual void derealize() = 0;

  std::size_t                          id_       ;
  std::string                          name_     ;
  const render_task_base*              creator_  ;
  std::vector<const render_task_base*> readers_  ;
  std::vector<const render_task_base*> writers_  ;
  std::size_t                          ref_count_; // Computed through framegraph compilation.
};
}

#endif