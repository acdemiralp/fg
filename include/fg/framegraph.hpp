#ifndef FG_FRAMEGRAPH_HPP_
#define FG_FRAMEGRAPH_HPP_

#include <memory>
#include <vector>

#include <fg/render_task.hpp>
#include <fg/resource_registry.hpp>

namespace fg
{
template<typename... resource_types>
class framegraph
{
public:
  framegraph           ()                        = default;
  framegraph           (const framegraph&  that) = default;
  framegraph           (      framegraph&& temp) = default;
  virtual ~framegraph  ()                        = default;
  framegraph& operator=(const framegraph&  that) = default;
  framegraph& operator=(      framegraph&& temp) = default;

  template<typename data_type, typename... argument_types>
  render_task<data_type>* add_render_task   (argument_types&&...     arguments  )
  {
    return nullptr;
  }
  template<typename data_type>
  void                    remove_render_task(render_task<data_type>* render_task)
  {
    
  }

  void                    compile           ()
  {
    
  }
  void                    traverse          () const
  {
    
  }

protected:
  resource_registry<resource_types...>           resource_registry_;
  std::vector<std::unique_ptr<render_task_base>> render_tasks_     ;
};
}

#endif