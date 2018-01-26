#ifndef FG_RENDER_RESOURCE_HPP_
#define FG_RENDER_RESOURCE_HPP_

#include <fg/node.hpp>

namespace fg
{
class render_resource : public node
{
public:
  render_resource           ()                             = default;
  render_resource           (const render_resource&  that) = default;
  render_resource           (      render_resource&& temp) = default;
  virtual ~render_resource  ()                             = default;
  render_resource& operator=(const render_resource&  that) = default;
  render_resource& operator=(      render_resource&& temp) = default;

protected:

};
}

#endif