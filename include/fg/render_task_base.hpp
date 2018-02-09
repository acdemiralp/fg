#ifndef FG_RENDER_TASK_BASE_HPP_
#define FG_RENDER_TASK_BASE_HPP_

#include <cstddef>
#include <string>
#include <vector>

namespace fg
{
class framegraph;
class render_task_builder;
class resource_base;

class render_task_base
{
public:
  explicit render_task_base  (const std::string& name) : name_(name), cull_immune_(false), ref_count_(0)
  {
    
  }
  render_task_base           (const render_task_base&  that) = delete ;
  render_task_base           (      render_task_base&& temp) = default;
  virtual ~render_task_base  ()                              = default;
  render_task_base& operator=(const render_task_base&  that) = delete ;
  render_task_base& operator=(      render_task_base&& temp) = default;

  const std::string& name             () const
  {
    return name_;
  }
  void               set_name         (const std::string& name)
  {
    name_ = name;
  }
  
  bool               cull_immune      () const
  {
    return cull_immune_;
  }
  void               set_cull_immune  (const bool cull_immune)
  {
    cull_immune_ = cull_immune;
  }

protected:
  friend framegraph;
  friend render_task_builder;

  virtual void setup  (render_task_builder& builder)       = 0;
  virtual void execute()                             const = 0;

  std::string                       name_       ;
  bool                              cull_immune_;
  std::vector<const resource_base*> creates_    ;
  std::vector<const resource_base*> reads_      ;
  std::vector<const resource_base*> writes_     ;
  std::size_t                       ref_count_  ; // Computed through framegraph compilation.
};
}

#endif