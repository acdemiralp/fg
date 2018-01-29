#ifndef FG_BUILDER_HPP_
#define FG_BUILDER_HPP_

#include <fg/framegraph.hpp>
#include <fg/resource.hpp>

namespace fg
{
class builder
{
public:
  builder           ()                     = default;
  builder           (const builder&  that) = default;
  builder           (      builder&& temp) = default;
  virtual ~builder  ()                     = default;
  builder& operator=(const builder&  that) = default;
  builder& operator=(      builder&& temp) = default;

  template<typename type>
  resource<type> read  (const resource<type>& resource) const
  {
    
  }
  template<typename type>
  resource<type> write (const resource<type>& resource)
  {
    
  }
  template<typename type>
  resource<type> create()
  {
    
  }

protected:
  framegraph* framegraph_;
};
}

#endif