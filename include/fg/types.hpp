#ifndef FG_TYPES_HPP_
#define FG_TYPES_HPP_

#include <memory>

#include <boost/graph/directed_graph.hpp>

namespace fg
{
class node;

using vertex_type       = std::shared_ptr<node>             ;
using graph_type        = boost::directed_graph<vertex_type>;
using vertex_descriptor = graph_type::vertex_descriptor     ;
}

#endif