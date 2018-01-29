#ifndef FG_FRAMEGRAPH_HPP_
#define FG_FRAMEGRAPH_HPP_

namespace fg
{
class framegraph
{
public:
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