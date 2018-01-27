#ifndef FG_RENDER_TASK_HPP_
#define FG_RENDER_TASK_HPP_

#include <functional>
#include <type_traits>

#include <fg/node.hpp>

namespace fg
{
class render_task_base : public node
{
public:
  render_task_base           ()                              = default;
  render_task_base           (const render_task_base&  that) = delete ;
  render_task_base           (      render_task_base&& temp) = default;
  virtual ~render_task_base  ()                              = default;
  render_task_base& operator=(const render_task_base&  that) = delete ;
  render_task_base& operator=(      render_task_base&& temp) = default;

  virtual void setup  ()       = 0;
  virtual void execute() const = 0;
};

template<typename type = void, typename enable = void>
class render_task;
template<typename type>
class render_task<type, typename std::enable_if< std::is_void<type>::value>::type> : public render_task_base
{
public:
  render_task           (const std::function<void()>& setup, const std::function<void()>& execute) : setup_(setup), execute_(execute)
  {
    
  }
  render_task           (const render_task&  that) = delete ;
  render_task           (      render_task&& temp) = default;
  virtual ~render_task  ()                         = default;
  render_task& operator=(const render_task&  that) = delete ;
  render_task& operator=(      render_task&& temp) = default;

  void setup  ()       override
  {
    setup_  ();
  }
  void execute() const override
  {
    execute_();
  }

protected:
  std::function<void()> setup_  ;
  std::function<void()> execute_;
};
template<typename type>
class render_task<type, typename std::enable_if<!std::is_void<type>::value>::type> : public render_task_base
{
public:
  render_task           (const std::function<void(type&)>& setup, const std::function<void(const type&)>& execute) : setup_(setup), execute_(execute)
  {
    
  }
  render_task           (const render_task&  that) = delete ;
  render_task           (      render_task&& temp) = default;
  virtual ~render_task  ()                         = default;
  render_task& operator=(const render_task&  that) = delete ;
  render_task& operator=(      render_task&& temp) = default;

  void setup  ()       override
  {
    setup_  (data_);
  }
  void execute() const override
  {
    execute_(data_);
  }

protected:
  type                             data_   ;
  std::function<void(      type&)> setup_  ;
  std::function<void(const type&)> execute_;
};
}

#endif