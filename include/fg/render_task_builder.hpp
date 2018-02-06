#ifndef FG_RENDER_TASK_BUILDER_HPP_
#define FG_RENDER_TASK_BUILDER_HPP_

#include <string>

namespace fg
{
class framegraph;

class render_task_builder
{
public:
  explicit render_task_builder  (framegraph* framegraph) : framegraph_(framegraph)
  {

  }
  render_task_builder           (const render_task_builder&  that) = default;
  render_task_builder           (      render_task_builder&& temp) = default;
  virtual ~render_task_builder  ()                                 = default;
  render_task_builder& operator=(const render_task_builder&  that) = default;
  render_task_builder& operator=(      render_task_builder&& temp) = default;

  template<typename resource_type, typename description_type>
  resource_type* create(const std::string& name, const description_type& description);
  template<typename resource_type>
  resource_type* read  (const resource_type* resource);
  template<typename resource_type>
  resource_type* write (const resource_type* resource);

protected:
  framegraph* framegraph_;
};
}

#endif