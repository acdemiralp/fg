#ifndef FG_RENDER_TASK_BASE_HPP_
#define FG_RENDER_TASK_BASE_HPP_

namespace fg
{
class render_task_builder;
class render_task_resources;
class resource_base;

class render_task_base
{
public:
  virtual ~render_task_base() = default;

  virtual void                       setup    (      render_task_builder  & builder  )       = 0;
  virtual void                       execute  (const render_task_resources& resources) const = 0;

  const std::vector<resource_base*>& resources()                                       const
  {
    return resources_;
  }

protected:
  friend render_task_builder;
  friend render_task_resources;

  std::vector<resource_base*> resources_;
};
}

#endif