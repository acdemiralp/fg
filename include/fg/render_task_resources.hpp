#ifndef FG_RENDER_TASK_RESOURCES_HPP_
#define FG_RENDER_TASK_RESOURCES_HPP_

namespace fg
{
class render_task_resources
{
public:
  explicit render_task_resources  ()
  {

  }
  render_task_resources           (const render_task_resources&  that) = default;
  render_task_resources           (      render_task_resources&& temp) = default;
  virtual ~render_task_resources  ()                                   = default;
  render_task_resources& operator=(const render_task_resources&  that) = default;
  render_task_resources& operator=(      render_task_resources&& temp) = default;
};
}

#endif