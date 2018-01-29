#ifndef FG_FRAMEGRAPH_HPP_
#define FG_FRAMEGRAPH_HPP_

#include <fstream>
#include <memory>
#include <type_traits>

#include <boost/graph/directed_graph.hpp>
#include <boost/graph/graphviz.hpp>

#include <fg/node.hpp>
#include <fg/render_resource.hpp>
#include <fg/render_task.hpp>
#include <fg/types.hpp>

namespace fg
{
// Notes:
// - The framegraph starts and ends its traversal from internal predefined vertices marking the beginning and the end of the frame.
// - Any vertex that is not directly or indirectly connected to these vertices will not be traversed.
// - A vertex may be a render resource or a render task.
// - Dependencies of a vertex is declared during insertion to the framegraph.
// - If a vertex does not have dependencies, then it has an incoming edge from the start vertex and an outgoing edge to the end vertex.
// - If a vertex has dependencies, then it does not have an incoming edge from the start vertex but from each of its dependencies instead. It still has an outgoing edge to the end vertex.
// - If any of the dependencies are connected to the end vertex, this connection is removed when a dependent is added, and is re-added when all dependents are removed.
class framegraph
{
public:
  framegraph           () 
  : start_(graph_.add_vertex(std::make_shared<node>()))
  , end_  (graph_.add_vertex(std::make_shared<node>()))
  {
    graph_.add_edge(start_, end_);
  }
  framegraph           (const framegraph&  that) = default;
  framegraph           (      framegraph&& temp) = default;
  virtual ~framegraph  ()                        = default;
  framegraph& operator=(const framegraph&  that) = default;
  framegraph& operator=(      framegraph&& temp) = default;

  template<typename type, typename... arguments_type>
  render_task<type>* add_render_task   (arguments_type&&... arguments  )
  {
    auto descriptor  = graph_.add_vertex(std::make_shared<render_task<type>>(arguments...));
    auto render_task = static_cast<render_task<type>*>(graph_[descriptor].get());
    render_task->set_metadata(this, descriptor);
    graph_.add_edge(start_    , descriptor);
    graph_.add_edge(descriptor, end_      );
    return render_task;
  }
  template<typename type>
  void               remove_render_task(render_task<type>*  render_task)
  {
    graph_.remove_vertex(render_task->descriptor_);
  }

  template<typename type, typename... arguments_type>
  type*              import_resource   (arguments_type&&... arguments  )
  {
    static_assert(std::is_base_of<render_resource, type>::value, "Type does not inherit from render_resource.");
    auto descriptor      = graph_.add_vertex(std::make_shared<type>(arguments...));
    auto render_resource = static_cast<type*>(graph_[descriptor].get());
    render_resource->set_metadata(this, descriptor);
    graph_.add_edge(start_    , descriptor);
    graph_.add_edge(descriptor, end_      );
    return render_resource;
  }

  void               execute           ()                                const
  {
    
  }

  void               to_graphviz       (const std::string& filepath)     const
  {
    boost::write_graphviz(std::ofstream(filepath.c_str()), graph_, boost::make_label_writer(make_transform_value_property_map([ ] (const vertex_type& iteratee)
    {
      return typeid(*iteratee.get()).name();
    }, get(boost::vertex_bundle, graph_))));
  }

protected:
  void declare_dependency(const node& lhs, const node& rhs)
  {
    graph_.add_edge(lhs.descriptor_, rhs.descriptor_);
    // But how does rhs access the outputs of lhs?
  }

  graph_type        graph_;
  vertex_descriptor start_;
  vertex_descriptor end_  ;
};
}

#endif