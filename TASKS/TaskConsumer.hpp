#ifndef __TASK_CONSUMER__
# define __TASK_CONSUMER__

# include <unistd.h>

# include "TaskList.hpp"
# include "../LIB/Runnable.h"


template <typename R, typename P>
class ATaskSystem;

template <typename R, typename P>
class TaskConsumer;

template <typename R, typename P>
class TaskConsumer : public Runnable
{
public:

  TaskConsumer(boost::shared_ptr<ATasksList<R, P> > const & tl,
	       ATaskSystem<R,P> * ts = 0x0)
    : Runnable(),
      _manager(ts)
  {
    this->_content = tl;
  }

  virtual ~TaskConsumer()
  {
  }

  virtual void	Run_e(void)
  {
    typename ATasksList<R, P>::ACB_type * cb = this->_content->Pull();
    if ((cb != 0x0) && (!(*cb) == false))
      cb->Call();
    delete cb;
    usleep(700);
  }

  virtual void	NotifyStop(void)
  {
    if (this->_manager == 0x0) return;
    // do smthg else
  }

protected:
  boost::shared_ptr<ATasksList<R, P> >	_content;
  ATaskSystem<R,P> *			_manager;
};

#endif // __TASK_CONSUMER__
