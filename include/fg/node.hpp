#ifndef FG_NODE_HPP_
#define FG_NODE_HPP_

#include <fg/types.hpp>

namespace fg
{
class framegraph;

class node
{
public:
  node           ()                  = default;
  node           (const node&  that) = delete ;
  node           (      node&& temp) = default;
  virtual ~node  ()                  = default;
  node& operator=(const node&  that) = delete ;
  node& operator=(      node&& temp) = default;
  
protected:
  friend framegraph;

  void set_metadata(const framegraph* owner, const vertex_descriptor descriptor)
  {
    owner_      = owner     ;
    descriptor_ = descriptor;
  }

  const framegraph* owner_      = nullptr;
  vertex_descriptor descriptor_ = nullptr;
};
}

#endif