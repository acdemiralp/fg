#ifndef FG_RENDER_TASK_HPP_
#define FG_RENDER_TASK_HPP_

#include <functional>

namespace fg
{
template<typename type>
class render_task
{
public:
  render_task           (const std::function<void(type&)>& setup, const std::function<void(const type&)>& execute) : setup_(setup), execute_(execute)
  {
    
  }
  render_task           (const render_task&  that) = delete ;
  render_task           (      render_task&& temp) = default;
  virtual ~render_task  ()                         = default;
  render_task& operator=(const render_task&  that) = delete ;
  render_task& operator=(      render_task&& temp) = default;

  void setup  ()
  {
    setup_  (data_);
  }
  void execute() const
  {
    execute_(data_);
  }

protected:
  type                             data_   ;
  std::function<void(      type&)> setup_  ;
  std::function<void(const type&)> execute_;
};
}

#endif