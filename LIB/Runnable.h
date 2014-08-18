#ifndef __RUNNABLE__
# define __RUNNABLE__

# include <iostream>
# include <unistd.h>

static const std::string & RUNNABLE_ERROR_ALREADY_RUNNING = "Runnable::Run : Error : already running";

class Runnable
{
 public:

  enum	STATUS
  {
    STOPPED,
    RUNNING
  };

  Runnable();

  void			Run(void);
  void			Stop(void);
  bool			isRunning(void) const;
  STATUS		GetStatus(void) const;

 protected:

  virtual void		Run_e()		= 0;
  virtual void		NotifyStop()	= 0;

  volatile bool		_isRunning;	// /!\ false != is_not_running : can be stopping but no stopped
  STATUS		_status;
};

#endif // __RUNNABLE__
