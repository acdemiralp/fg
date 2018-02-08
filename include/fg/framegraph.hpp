#ifndef FG_FRAMEGRAPH_HPP_
#define FG_FRAMEGRAPH_HPP_

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <fg/render_task.hpp>
#include <fg/render_task_builder.hpp>
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

  template<typename data_type, typename... argument_types>
  render_task<data_type>*                  add_render_task      (argument_types&&...     arguments  )
  {
    render_tasks_.emplace_back(std::make_unique<render_task<data_type>>(arguments...));
    auto render_task = render_tasks_.back().get();

    render_task_builder builder(this, render_task);
    render_task->setup(builder);
    
    return static_cast<fg::render_task<data_type>*>(render_task);
  }
  template<typename description_type, typename actual_type>
  resource<description_type, actual_type>* add_retained_resource(const std::string& name, const description_type& description, actual_type* actual)
  {
    resources_.emplace_back(std::make_unique<resource<description_type, actual_type>>(name, description, actual));
    return static_cast<resource<description_type, actual_type>*>(resources_.back().get());
  }
  void                                     compile              ()
  {
    // TODO: Cull and fill timeline.

    // Compute creation and destruction (last read) of transient resources.
    for(auto& resource : resources_)
      if(resource->creator_)
      {
        // TODO.
      }

    // Cull resources and render tasks which are not read or written.
    // TODO.
  }
  void                                     execute              () const
  {
    for(auto& step : timeline_)
    {
      for (auto resource : step.realized_resources  ) resource->realize  ();
      for (auto resource : step.derealized_resources) resource->derealize();
      step.render_task->execute();
    }
  }
  void                                     clear                ()
  {
    render_tasks_.clear();
    resources_   .clear();
  }

protected:
  friend render_task_builder;
  
  struct step
  {
    render_task_base*           render_task         ;
    std::vector<resource_base*> realized_resources  ;
    std::vector<resource_base*> derealized_resources;
  };
  
  std::vector<std::unique_ptr<render_task_base>> render_tasks_;
  std::vector<std::unique_ptr<resource_base>>    resources_   ;
  std::vector<step>                              timeline_    ; // Computed during compilation.
};

template<typename resource_type, typename description_type>
resource_type* render_task_builder::create(const std::string& name, const description_type& description)
{
  static_assert(std::is_same<typename resource_type::description_type, description_type>::value, "Description does not match the resource.");
  framegraph_->resources_.emplace_back(std::make_unique<resource_type>(name, render_task_, description));
  const auto resource = framegraph_->resources_.back().get();
  render_task_->creates_.push_back(resource);
  return static_cast<resource_type*>(resource);
}
template<typename resource_type>
resource_type* render_task_builder::read  (resource_type* resource)
{
  resource->readers_.push_back(render_task_);
  render_task_->reads_.push_back(resource);
  return resource;
}
template<typename resource_type>
resource_type* render_task_builder::write (resource_type* resource)
{
  resource->writers_.push_back(render_task_);
  render_task_->writes_.push_back(resource);
  return resource;
}
}

#endif