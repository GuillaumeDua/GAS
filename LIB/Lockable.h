#ifndef __LOCKABLE__
# define __LOCKABLE__

# include <unistd.h>

class Lockable
{
 public:
  Lockable(void);

  void	Lock(void);
  void	Lock(unsigned int t_scale);
  void	Unlock(void);
  bool	TryLock(void);
  bool	TryTimeLock(unsigned int t_scale, unsigned int t_max);

  bool	IsLocked(void);

 protected:
  volatile bool	_isLocked;
};

#endif // __LOCKABLE__
