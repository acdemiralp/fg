#ifndef FG_FRAMEGRAPH_HPP_
#define FG_FRAMEGRAPH_HPP_

#include <memory>

#include <boost/graph/directed_graph.hpp>

#include <fg/render_resource.hpp>
#include <fg/render_task_base.hpp>

namespace fg
{
class node;

class framegraph : public boost::directed_graph<std::shared_ptr<node>>
{
public:
  using graph_type        = boost::directed_graph<std::shared_ptr<node>>;
  using vertex_descriptor = vertex_descriptor;
  using edge_descriptor   = edge_descriptor  ;

  framegraph           ()                        = default;
  framegraph           (const framegraph&  that) = default;
  framegraph           (      framegraph&& temp) = default;
  virtual ~framegraph  ()                        = default;
  framegraph& operator=(const framegraph&  that) = default;
  framegraph& operator=(      framegraph&& temp) = default;

protected:

};
}

#endif