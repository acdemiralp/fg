#ifndef FG_FRAMEGRAPH_HPP_
#define FG_FRAMEGRAPH_HPP_

#include <memory>
#include <vector>

#include <fg/render_task.hpp>
#include <fg/resource_registry.hpp>

namespace fg
{
// TODO: remove_render_task, import_resource, compile, clear.
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
    render_tasks_.emplace_back(std::make_unique<render_task<data_type>>(arguments...));
    auto render_task = render_tasks_.back().get();

    render_task_builder builder;
    render_task->setup(builder);
    
    return static_cast<fg::render_task<data_type>*>(render_task);
  }
  void                    traverse          () const
  {
    for(auto& render_task : render_tasks_)
    {
      render_task_resources resources;
      render_task->execute(resources);
    }
  }

protected:
  resource_registry<resource_types...>           resource_registry_;
  std::vector<std::unique_ptr<render_task_base>> render_tasks_     ;
};
}

#endif