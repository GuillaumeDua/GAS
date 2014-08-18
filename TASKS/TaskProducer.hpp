#ifndef __TASK_PRODUCER__
# define __TASK_PRODUCER__

# include <boost/thread.hpp>

# include "/usr/include/boost/make_shared.hpp"
# include "/usr/include/boost/shared_ptr.hpp"


# include "./ACallBack.hpp"
# include "./TaskList.hpp"
# include "../LIB/Lockable.h"

template <typename R, typename P>
class TaskProducer;

template <typename R, typename P>
class TaskProducer : public Lockable,
		     public boost::enable_shared_from_this<TaskProducer<R, P> >
{
public:

  typedef R(TaskProducer<R, P>::* CB_as_mthd)(const P &);
  typedef R(*CB_as_fct)(boost::shared_ptr<TaskProducer<R, P> >, const P &);

  TaskProducer(boost::shared_ptr<ATasksList<R, P> > const & tl)
    : _productionNeedStop(false)
  {
    this->_content = tl;
  }

  void	InitSelfPtr(void)
  {
    std::cout << "TaskProducer::~ : use_count : " << this->_this_shared_ptr.use_count() << std::endl;
    this->_this_shared_ptr = boost::static_pointer_cast<Lockable>(this->shared_from_this());
    std::cout << "TaskProducer::~ : use_count : " << this->_this_shared_ptr.use_count() << std::endl;
  }

  void	CleanSelfPtr(void)
  {
    this->_this_shared_ptr.reset();
  }

  bool	AddTask(typename ATasksList<R, P>::ACB_type * task)
  {
    if (this->_productionNeedStop) return false;
    return (this->_content->Push(task));
  }

  bool	AddTask(CB_as_mthd m_ptr, const P & p)
  {
    if (this->_productionNeedStop) return false;
    return (this->_content->Push(new typename ATasksList<R, P>::ACB_type(reinterpret_cast<typename ACallBack<R,P>::CB_as_mthd>(m_ptr),
    									 p,
    									 _this_shared_ptr)));
  }

  bool	AddTask(CB_as_fct f_ptr, const P & p)
  {
    if (this->_productionNeedStop) return false;
    return (this->_content->Push(new typename ATasksList<R, P>::ACB_type(reinterpret_cast<typename ACallBack<R,P>::CB_as_fct>(f_ptr),
									 p,
									 _this_shared_ptr)));
  }

  void	NotifyStopProduction(void)
  {
    this->_productionNeedStop = true;
  }

  boost::shared_ptr<Lockable> const &	GetThisSharedPtr(void)
  {
    return this->_this_shared_ptr;
  }

  // virtual typename ATasksList<R, P>::ACB_type *	GenerateACB() = 0;

protected:
  boost::shared_ptr<ATasksList<R, P> >		_content;
  volatile bool					_productionNeedStop;
  boost::shared_ptr<Lockable>			_this_shared_ptr;
};

#endif // __TASK_PRODUCER__
