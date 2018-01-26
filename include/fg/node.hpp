#ifndef FG_NODE_HPP_
#define FG_NODE_HPP_

namespace fg
{
class framegraph;

class node
{
public:
  node           ()                  = default;
  node           (const node&  that) = default;
  node           (      node&& temp) = default;
  virtual ~node  ()                  = default;
  node& operator=(const node&  that) = default;
  node& operator=(      node&& temp) = default;

protected:
  friend framegraph;
  const  framegraph* framegraph_ = nullptr;
};
}

#endif