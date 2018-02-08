#ifndef FG_FRAMEGRAPH_HPP_
#define FG_FRAMEGRAPH_HPP_

#include <fstream>
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
  framegraph           (const framegraph&  that) = delete ;
  framegraph           (      framegraph&& temp) = default;
  virtual ~framegraph  ()                        = default;
  framegraph& operator=(const framegraph&  that) = delete ;
  framegraph& operator=(      framegraph&& temp) = default;

  template<typename data_type, typename... argument_types>
  render_task<data_type>*                  add_render_task      (argument_types&&... arguments)
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
    timeline_.clear();

    // Reference counting.
    std::vector<std::size_t> render_task_references(render_tasks_.size());
    std::vector<std::size_t> resource_references   (resources_   .size());
    for (auto i = 0; i < render_tasks_.size(); ++i)
      render_task_references[i] = render_tasks_[i]->creates_.size() + render_tasks_[i]->writes_.size();
    for (auto i = 0; i < resources_   .size(); ++i)
      resource_references   [i] = resources_   [i]->readers_.size();

    // TODO: Cull and prepare the timeline with unculled render tasks and resources.
    // - Vertex Culling via Flood Fill:
    //   - For each resource with 0 references:
    //     - Push it to a stack.
    //   - For each element of the stack:
    //     - Remove it from the framegraph.
    //     - Decrement the references of creator/writer render tasks.
    //     - For each creator/writer render task with 0 references and is not cull-immune:
    //       - Decrement reference counts of its read resources.
    //       - For each read resource with 0 references:
    //         - Push it to the stack.
    // - Timeline setup:
    //   - The unculled render tasks are sequentially added to the timeline.
    //   - The realization-derealization interval of transient resources (as part of the timeline) are computed via:
    //     - A create marks the realization of a transient resource.
    //     - The last read or write marks the derealization of a transient resource.
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
  void                                     export_graphviz      (const std::string& filepath)
  {
    std::ofstream stream(filepath);
    stream << "digraph framegraph \n{\n";

    stream << "rankdir = LR\n";
    stream << "bgcolor = black\n\n";
    stream << "node [shape=rectangle, fontname=\"helvetica\", fontsize=12]\n\n";

    for (auto& render_task : render_tasks_)
      stream << "\"" << render_task->name() << "\" [label=\"" << render_task->name() << "\", style=filled, fillcolor=darkorange]\n";
    stream << "\n";

    for (auto& resource    : resources_   )
      stream << "\"" << resource   ->name() << "\" [label=\"" << resource   ->name() + "\\nID: " << resource->id() << "\", style=filled, fillcolor= " << (resource->is_transient() ? "skyblue" : "steelblue") << "]\n";
    stream << "\n";
    
    for (auto& render_task : render_tasks_)
    {
      stream << "\"" << render_task->name() << "\" -> { ";
      for (auto& resource : render_task->creates_)
        stream << "\"" << resource->name() << "\" ";
      stream << "} [color=seagreen]\n";

      stream << "\"" << render_task->name() << "\" -> { ";
      for (auto& resource : render_task->writes_)
        stream << "\"" << resource->name() << "\" ";
      stream << "} [color=gold]\n";
    }
    stream << "\n";

    for (auto& resource : resources_)
    {
      stream << "\"" << resource->name() << "\" -> { ";
      for (auto& render_task : resource->readers_)
        stream << "\"" << render_task->name() << "\" ";
      stream << "} [color=firebrick]\n";
    }
    stream << "}";
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