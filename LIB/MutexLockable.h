#ifndef __MUTEX_LOCKABLE__
# define __MUTEX_LOCKABLE__

# include <unistd.h>
# include <boost/shared_ptr.hpp>
# include <boost/thread.hpp>
# include <boost/make_shared.hpp>

class	MutexLockable
{
 public:

  static const size_t	U_TIME_MARGIN = 400;

  MutexLockable();

  bool			IsAbortRequired(void);
  bool			IsLocked(void);
  void			Abort(void);
  bool			Lock(void);
  bool			Unlock(void);

 private:
  volatile bool		_isLocked;
  volatile bool		_abort;
  mutable boost::mutex	_mutex;
};

#endif // __MUTEX_LOCKABLE__
