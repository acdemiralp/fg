#ifndef FG_BUILDER_HPP_
#define FG_BUILDER_HPP_

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

protected:

};
}

#endif