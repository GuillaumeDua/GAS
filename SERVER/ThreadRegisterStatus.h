#ifndef __THREAD_REGISTER_SATTUS__
# define __THREAD_REGISTER_SATTUS__

# include <vector>

# include <boost/shared_ptr.hpp>
# include <boost/thread.hpp>
# include <boost/make_shared.hpp>

# include "../LIB/MutexLockable.h"

// to transform by a singleton factory
class ThreadRegisterStatus : public MutexLockable
{
 public:
  typedef std::vector<boost::thread *>			content_type;
  typedef std::vector<boost::thread *>::iterator	content_iterator;

  ThreadRegisterStatus();
  ~ThreadRegisterStatus();

  void			AddThread(boost::thread * t);
  void			NotifyStop(const boost::thread::id & id);
  void			NotifyStop(const boost::thread * t);
  bool			empty(void);

 protected:
  content_type		_content;
};


#endif // __THREAD_REGISTER_SATTUS__
