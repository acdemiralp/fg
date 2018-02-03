#ifndef FG_RENDER_TASK_HPP_
#define FG_RENDER_TASK_HPP_

#include <functional>

#include <fg/render_task_base.hpp>

namespace fg
{
class render_task_builder  ;
class render_task_resources;

template<typename data_type>
class render_task : public render_task_base
{
public:
  explicit render_task  (
    const std::function<void(      data_type&,       render_task_builder  &)>& setup  ,
    const std::function<void(const data_type&, const render_task_resources&)>& execute) : setup_(setup), execute_(execute)
  {
    
  }
  render_task           (const render_task&  that) = delete ;
  render_task           (      render_task&& temp) = default;
  virtual ~render_task  ()                         = default;
  render_task& operator=(const render_task&  that) = delete ;
  render_task& operator=(      render_task&& temp) = default;
  
  const data_type& data   ()                                       const
  {
    return data_;
  }
  void             setup  (      render_task_builder  & builder  )       override
  {
    setup_  (data_, builder  );
  }
  void             execute(const render_task_resources& resources) const override
  {
    execute_(data_, resources);
  }

protected:
  data_type                                                                 data_   ;
  const std::function<void(      data_type&,       render_task_builder  &)> setup_  ;
  const std::function<void(const data_type&, const render_task_resources&)> execute_;
};
}

#endif