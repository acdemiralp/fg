#ifndef FG_FRAMEGRAPH_HPP_
#define FG_FRAMEGRAPH_HPP_

#include <memory>
#include <vector>

#include <fg/render_task.hpp>
#include <fg/resource.hpp>

namespace fg
{
class framegraph
{
public:
  framegraph           ()                        = default;
  framegraph           (const framegraph&  that) = default;
  framegraph           (      framegraph&& temp) = default;
  virtual ~framegraph  ()                        = default;
  framegraph& operator=(const framegraph&  that) = default;
  framegraph& operator=(      framegraph&& temp) = default;

  template<typename type, typename... argument_types>
  type* add_render_task   (argument_types&&... arguments  )
  {
    render_tasks_.emplace_back(std::make_unique<type>(arguments...));
    return render_tasks_.back().get();
  }
  template<typename type>
  void  remove_render_task(type*               render_task)
  {
    
  }

  void  compile           () const
  {
    
  }
  void  traverse          () const
  {
    
  }

protected:
  std::vector<std::unique_ptr<render_task_base>> render_tasks_;
};
}

#endif