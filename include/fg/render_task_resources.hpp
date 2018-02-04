#ifndef FG_RENDER_TASK_RESOURCES_HPP_
#define FG_RENDER_TASK_RESOURCES_HPP_

namespace fg
{
class framegraph;

class render_task_resources
{
public:
  explicit render_task_resources  (const framegraph& framegraph) : framegraph_(framegraph)
  {

  }
  render_task_resources           (const render_task_resources&  that) = default;
  render_task_resources           (      render_task_resources&& temp) = default;
  virtual ~render_task_resources  ()                                   = default;
  render_task_resources& operator=(const render_task_resources&  that) = default;
  render_task_resources& operator=(      render_task_resources&& temp) = default;

  template<typename resource_type>
  typename resource_type::actual_type* get(const resource_type& resource);

protected:
  const framegraph& framegraph_;
};
}

#endif