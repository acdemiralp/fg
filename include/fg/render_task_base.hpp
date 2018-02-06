#ifndef FG_RENDER_TASK_BASE_HPP_
#define FG_RENDER_TASK_BASE_HPP_

#include <string>

namespace fg
{
class framegraph;
class render_task_builder;

class render_task_base
{
public:
  explicit render_task_base  (const std::string& name = "") : name_(name)
  {
    
  }
  render_task_base           (const render_task_base&  that) = default;
  render_task_base           (      render_task_base&& temp) = default;
  virtual ~render_task_base  ()                              = default;
  render_task_base& operator=(const render_task_base&  that) = default;
  render_task_base& operator=(      render_task_base&& temp) = default;

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
  friend render_task_builder;

  virtual void setup  (render_task_builder& builder)       = 0;
  virtual void execute()                             const = 0;

  std::string name_;
};
}

#endif