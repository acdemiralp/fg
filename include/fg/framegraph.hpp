#ifndef FG_FRAMEGRAPH_HPP_
#define FG_FRAMEGRAPH_HPP_

#include <memory>
#include <vector>

#include <fg/render_task.hpp>
#include <fg/render_task_builder.hpp>
#include <fg/resource.hpp>

namespace fg
{
// TODO: Finish compile, traverse. Implement remove_render_task, add_resource, remove_resource.
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

    render_task_builder builder(this);
    render_task->setup(builder);
    
    return static_cast<fg::render_task<data_type>*>(render_task);
  }
  void                    compile           () const
  {
    for(auto& render_task : render_tasks_)
    {
      // Increment reference counts of resources, compute creation and destruction (last read) of resources.
    }
  }
  void                    traverse          () const
  {
    for(auto& render_task : render_tasks_)
    {
      // Realize resources based on their computed lifetimes.
      render_task->execute();
    }
  }
  void                    clear             ()
  {
    render_tasks_.clear();
    resources_   .clear();
  }

protected:
  friend render_task_builder;
  
  std::vector<std::unique_ptr<render_task_base>> render_tasks_;
  std::vector<std::unique_ptr<resource_base>>    resources_   ;
};

template<typename resource_type, typename description_type>
resource_type*                 render_task_builder::create(const std::string& name, const description_type& description)
{
  framegraph_->resources_.emplace_back(std::make_unique<resource_type>(name, description));
  return static_cast<resource_type*>(framegraph_->resources_.back().get());
}
template<typename resource_type>
resource_type*                 render_task_builder::read  (const resource_type*    resource   )
{
  return const_cast<resource_type*>(resource);
}
template<typename resource_type>
resource_type*                 render_task_builder::write (const resource_type*    resource   )
{
  return const_cast<resource_type*>(resource);
}
}

#endif