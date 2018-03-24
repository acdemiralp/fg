#ifndef FG_FRAMEGRAPH_HPP_
#define FG_FRAMEGRAPH_HPP_

#include <algorithm>
#include <fstream>
#include <iterator>
#include <memory>
#include <stack>
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
  resource<description_type, actual_type>* add_retained_resource(const std::string& name, const description_type& description, actual_type* actual = nullptr)
  {
    resources_.emplace_back(std::make_unique<resource<description_type, actual_type>>(name, description, actual));
    return static_cast<resource<description_type, actual_type>*>(resources_.back().get());
  }
  void                                     compile              ()
  {
    // Reference counting.
    for (auto& render_task : render_tasks_)
      render_task->ref_count_ = render_task->creates_.size() + render_task->writes_.size();
    for (auto& resource    : resources_   )
      resource   ->ref_count_ = resource   ->readers_.size();

    // Culling via flood fill from unreferenced resources.
    std::stack<resource_base*> unreferenced_resources;
    for (auto& resource : resources_)
      if (resource->ref_count_ == 0 && resource->transient())
        unreferenced_resources.push(resource.get());
    while (!unreferenced_resources.empty())
    {
      auto unreferenced_resource = unreferenced_resources.top();
      unreferenced_resources.pop();

      auto creator = const_cast<render_task_base*>(unreferenced_resource->creator_);
      if  (creator->ref_count_ > 0) 
           creator->ref_count_--;
      if  (creator->ref_count_ == 0 && !creator->cull_immune())
      {
        for (auto iteratee : creator->reads_)
        {
          auto read_resource = const_cast<resource_base*>(iteratee);
          if  (read_resource->ref_count_ > 0)
               read_resource->ref_count_--;
          if  (read_resource->ref_count_ == 0 && read_resource->transient())
            unreferenced_resources.push(read_resource);
        }
      }
      
      for(auto c_writer : unreferenced_resource->writers_)
      {
        auto writer = const_cast<render_task_base*>(c_writer);
        if  (writer->ref_count_ > 0)
             writer->ref_count_--;
        if  (writer->ref_count_ == 0 && !writer->cull_immune())
        {
          for (auto iteratee : writer->reads_)
          {
            auto read_resource = const_cast<resource_base*>(iteratee);
            if  (read_resource->ref_count_ > 0)
                 read_resource->ref_count_--;
            if  (read_resource->ref_count_ == 0 && read_resource->transient())
              unreferenced_resources.push(read_resource);
          }
        }
      }
    }

    // Timeline computation.
    timeline_.clear();
    for (auto& render_task : render_tasks_)
    {
      if (render_task->ref_count_ == 0 && !render_task->cull_immune()) 
        continue;
      
      std::vector<resource_base*> realized_resources, derealized_resources;

      for (auto resource : render_task->creates_)
      {
        realized_resources.push_back(const_cast<resource_base*>(resource));
        if (resource->readers_.empty() && resource->writers_.empty())
          derealized_resources.push_back(const_cast<resource_base*>(resource));
      }

      auto reads_writes = render_task->reads_;
      reads_writes.insert(reads_writes.end(), render_task->writes_.begin(), render_task->writes_.end());
      for (auto resource : reads_writes)
      {
        if (!resource->transient()) 
          continue;

        auto        valid     = false;
        std::size_t last_index;
        if (!resource->readers_.empty())
        {
          auto last_reader = std::find_if(
            render_tasks_.begin(), 
            render_tasks_.end  (),
            [&resource] (const std::unique_ptr<render_task_base>& iteratee)
            {
              return iteratee.get() == resource->readers_.back();
            });
          if(last_reader != render_tasks_.end())
          {
            valid      = true;
            last_index = std::distance(render_tasks_.begin(), last_reader);
          } 
        }
        if (!resource->writers_.empty())
        {
          auto last_writer = std::find_if(
            render_tasks_.begin(), 
            render_tasks_.end  (), 
            [&resource] (const std::unique_ptr<render_task_base>& iteratee)
            {
              return iteratee.get() == resource->writers_.back();
            });
          if (last_writer != render_tasks_.end())
          {
            valid      = true;
            last_index = std::max(last_index, std::size_t(std::distance(render_tasks_.begin(), last_writer)));
          }
        }

        if (valid && render_tasks_[last_index] == render_task)
          derealized_resources.push_back(const_cast<resource_base*>(resource));
      }
      
      timeline_.push_back(step{render_task.get(), realized_resources, derealized_resources});
    }
  }
  void                                     execute              () const
  {
    for(auto& step : timeline_)
    {
      for (auto resource : step.realized_resources  ) resource->realize  ();
      step.render_task->execute();
      for (auto resource : step.derealized_resources) resource->derealize();
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
      stream << "\"" << render_task->name() << "\" [label=\"" << render_task->name() << "\\nRefs: " << render_task->ref_count_ << "\", style=filled, fillcolor=darkorange]\n";
    stream << "\n";

    for (auto& resource    : resources_   )
      stream << "\"" << resource   ->name() << "\" [label=\"" << resource   ->name() << "\\nRefs: " << resource   ->ref_count_ << "\\nID: " << resource->id() << "\", style=filled, fillcolor= " << (resource->transient() ? "skyblue" : "steelblue") << "]\n";
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
  std::vector<step>                              timeline_    ; // Computed through framegraph compilation.
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