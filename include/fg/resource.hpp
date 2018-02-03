#ifndef FG_RESOURCE_HPP_
#define FG_RESOURCE_HPP_

#include <cstddef>
#include <memory>

#include <fg/actualize.hpp>

namespace fg
{
template<typename description_type, typename actual_type>
class resource final
{
public:
  explicit resource  (const description_type& description = description_type()) : description_(description)
  {
    static std::size_t id = 0;
    id_ = id++;
  }
  resource           (const resource&  that) = default;
  resource           (      resource&& temp) = default;
  ~resource          ()                      = default;
  resource& operator=(const resource&  that) = default;
  resource& operator=(      resource&& temp) = default;

  std::size_t                  id         () const
  {
    return id_;
  }
  const description_type&      description() const
  {
    return description_;
  }

  std::unique_ptr<actual_type> actualize  ()
  {
    return fg::actualize(description_);
  }

protected:
  std::size_t      id_         ;
  description_type description_;
};
}

#endif