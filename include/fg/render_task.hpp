#ifndef FG_RENDER_TASK_HPP_
#define FG_RENDER_TASK_HPP_

#include <functional>
#include <string>

#include <fg/render_task_base.hpp>

namespace fg
{
class render_task_builder;

template<typename data_type_>
class render_task : public render_task_base
{
public:
  using data_type = data_type_;

  explicit render_task  (
    const std::string&                                                 name   ,
    const std::function<void(      data_type&, render_task_builder&)>& setup  ,
    const std::function<void(const data_type&)>&                       execute) : render_task_base(name), setup_(setup), execute_(execute)
  {
    
  }
  render_task           (const render_task&  that) = delete ;
  render_task           (      render_task&& temp) = default;
  virtual ~render_task  ()                         = default;
  render_task& operator=(const render_task&  that) = delete ;
  render_task& operator=(      render_task&& temp) = default;
  
  const data_type& data() const
  {
    return data_;
  }
  
protected:
  void setup  (render_task_builder& builder)       override
  {
    setup_  (data_, builder);
  }
  void execute()                             const override
  {
    execute_(data_);
  }

  data_type                                                         data_   ;
  const std::function<void(      data_type&, render_task_builder&)> setup_  ;
  const std::function<void(const data_type&)>                       execute_;
};
}

#endif