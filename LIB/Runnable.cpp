#include "./Runnable.h"

Runnable::Runnable()
  : _isRunning(false),
    _status(Runnable::STOPPED)
{}

void			Runnable::Run(void)
{
  if (this->_isRunning)
    {
      std::cerr << RUNNABLE_ERROR_ALREADY_RUNNING << std::endl;
      return;
    }

  this->_isRunning	= true;
  this->_status		= Runnable::RUNNING;

  while (this->_isRunning)
    {
      this->Run_e();
    }
  this->_status		= Runnable::STOPPED;
  this->NotifyStop();
}

void			Runnable::Stop(void)
{
  this->_isRunning = false;
}


bool			Runnable::isRunning(void) const
{
  return this->_isRunning;
}

Runnable::STATUS	Runnable::GetStatus(void) const
{
  return this->_status;
}
