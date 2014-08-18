#include "./MutexLockable.h"

MutexLockable::MutexLockable()
  : _isLocked(false),
    _abort(false)
{}


bool			MutexLockable::IsAbortRequired(void)
{
  return this->_abort;
}

bool			MutexLockable::IsLocked(void)
{
  return this->_isLocked;
}

void			MutexLockable::Abort(void)
{
  this->_abort = true;
}

bool			MutexLockable::Lock(void)
{
  while (this->_isLocked)
    {
      if (this->_abort) return false;
      usleep(MutexLockable::U_TIME_MARGIN);
    }

  while (!(this->_mutex.try_lock()))
    {
      if (this->_abort) return false;
      usleep(MutexLockable::U_TIME_MARGIN);
    }
  this->_isLocked = true;
  return true;
}

bool			MutexLockable::Unlock(void)
{
  if (this->_isLocked == false) return false;

  this->_mutex.unlock();
  this->_isLocked = false;
  usleep(MutexLockable::U_TIME_MARGIN);
  return true;    
}

