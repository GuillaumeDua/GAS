#ifndef __ASERVER__
# define __ASERVER__

# include "../SOCKET/ASocket.h"
# include "../SOCKET/TCPSocket.h"
# include "../CLIENT/AClient.h"
# include "../CLIENT/List.h"

# include "../TASKS/ATaskSystem.hpp"
# include "../TASKS/TaskList.hpp"

namespace SERVER
{
  enum	STATUS
  {
    NOT_INIT,
    RUNNING,
    PAUSED,
    STOPED
  };

  static const std::string & ERROR_RUN_FAILED		= "SERVER::Abstract::Run : Cannot start server : bad init";
  static const std::string & WARNING_CLEAN_FAILED	= "Warning : Cannot clean client's list : still in use";


  class	Abstract
  {
  public:
    static const size_t					NBR_OF_TASKS_PRODUCER = 3;

    Abstract(const ASOCK::Socket & sock);
    Abstract(boost::shared_ptr<ASOCK::Socket> const & sock);

    ~Abstract();

    bool						Run(void);
    void						Pause(void);
    void						Stop(void);

    bool						IsError(void)	const;
    bool						operator!(void)	const;

    const ASOCK::Socket &				GetSocket(void)	const;

  protected:

    virtual void					RunSystem(void)		= 0;
    virtual void					StopSystem(void)	= 0;
    virtual void					Wait(void);
    virtual void					Clean(void);

    virtual void					CheckNewClients(void)	= 0;
    virtual void					RefreshAllClients(void)	= 0;

    volatile bool					_isError;
    volatile SERVER::STATUS				_status;
    boost::shared_ptr<ASOCK::Socket>			_sock;
    boost::shared_ptr<CLIENT::List>			_clientList;

    //Tasks :
    boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> >	_tasksList;
    ATaskSystem<TASK_TEMPLATE_PARAM>			_tasksSystem;
  };
}
#endif // __ASERVER__
