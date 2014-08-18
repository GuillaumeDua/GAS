# include "./Lockable.h"

Lockable::Lockable(void)
  : _isLocked(false)
{}

bool	Lockable::IsLocked(void)
{
  return this->_isLocked;
}

void	Lockable::Lock(void)
{
  while (this->TryLock() == false)
    usleep(700);
}

void	Lockable::Lock(unsigned int t_scale)
{
  while (this->TryLock() == false)
    usleep(t_scale);
}

void	Lockable::Unlock(void)
{
  if (this->_isLocked)
    this->_isLocked = false;    
}

bool	Lockable::TryLock(void)
{
  if (this->_isLocked) return false;
  this->_isLocked = true;
  return true;
}

bool	Lockable::TryTimeLock(unsigned int t_scale, unsigned int t_max)
{
  unsigned int count;
  for  (count = 0;
	(this->TryLock() == false) && (count < t_max);
	count += t_scale)
    usleep(t_scale);
  return this->_isLocked;
}
