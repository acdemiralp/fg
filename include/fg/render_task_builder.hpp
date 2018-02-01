#ifndef FG_RENDER_TASK_BUILDER_HPP_
#define FG_RENDER_TASK_BUILDER_HPP_

namespace fg
{
class render_task_builder
{
public:
  explicit render_task_builder  ()
  {

  }
  render_task_builder           (const render_task_builder&  that) = default;
  render_task_builder           (      render_task_builder&& temp) = default;
  virtual ~render_task_builder  ()                                 = default;
  render_task_builder& operator=(const render_task_builder&  that) = default;
  render_task_builder& operator=(      render_task_builder&& temp) = default;
};
}

#endif