#ifndef __MULTI_THREAD_SERVER__
# define __MULTI_THREAD_SERVER__

# include <boost/shared_ptr.hpp>
# include <boost/thread.hpp>
# include <boost/make_shared.hpp>
# include <boost/bind.hpp>

# include "./AServer.h"
# include "../CLIENT/AClient.h"
# include "../CLIENT/FakeClient.h"
# include "./ThreadRegisterStatus.h"
# include "../TASKS/ATaskSystem.hpp"

# include <bitset>

namespace SERVER
{
  class MultiThread : public SERVER::Abstract
  {
  public:
    MultiThread(const ASOCK::Socket & sock);
    MultiThread(boost::shared_ptr<ASOCK::Socket> const &);

    ~MultiThread();

    void					RunSystem(void);
    void					StopSystem(void);

  protected:
    void					CheckNewClients(void);
    void					RefreshAllClients(void);
    void					ExecClientsTasks(void);

    bool					AddNewClient(void);

    void					SyncClientLists(void);

    mutable boost::mutex			_mutex;
    boost::thread_group *			_threadGroup;
    boost::shared_ptr<CLIENT::List>		_asyncClientList;
    boost::shared_ptr<ThreadRegisterStatus>	_threadStatus;
    ATaskSystem<TASK_TEMPLATE_PARAM> *		_tasksSystem;
  };
}

#endif // __MULTI_THREAD_SERVER__
