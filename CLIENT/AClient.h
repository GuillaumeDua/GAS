#ifndef __ACLIENT__
# define __ACLIENT__

# include <list>
# include <map>
# include <sstream>

# include <boost/shared_ptr.hpp>
# include <boost/make_shared.hpp>

# include "../LIB/Lockable.h"
# include "../LIB/AbstractSTDMap.hpp"
# include "../LIB/ReadOnlySingleton.hpp"
# include "../LIB/Unique.hpp"

# include "../TASKS/ACallBack.hpp"
# include "../SOCKET/ASocket.h"
# include "../EXCHANGE/Datas.h"
# include "./Buffer.h"

# include "../TASKS/TaskList.hpp"
# include "../TASKS/TaskProducer.hpp"

# define TASK_TEMPLATE_PARAM bool,std::string

namespace CLIENT
{
  enum STATUS
  {
    SHUTDOWNED = false,
    ALIVE
  };

  class Abstract : public ASOCK::Attr,
    public TaskProducer<TASK_TEMPLATE_PARAM>
    {
    public:

      typedef std::list<CLIENT::Abstract *>				PtrList;
      typedef bool(CLIENT::Abstract::* CBType)(std::string);
      typedef std::map<const std::string, CBType>			CBMapType;
      typedef std::map<const std::string, CBType>::iterator		CBMapType_iterator;
      typedef std::map<const std::string, CBType>::const_iterator	CBMapType_const_iterator;
      typedef CBMapType							Behaviour;


      Abstract(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
	       boost::shared_ptr<ASOCK::Socket> const & s,
	       const ASOCK::Attr & a);
      Abstract(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
	       boost::shared_ptr<ASOCK::Socket> const & s,
	       ASOCK::Attr * a);
      Abstract(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
	       boost::shared_ptr<ASOCK::Socket> const & s,
	       ASOCK::SOCKADDR_IN * sin);
      Abstract(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
	       boost::shared_ptr<ASOCK::Socket> const & s,
	       const std::string & h, ASOCK::PORT p);
      Abstract(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
	       boost::shared_ptr<ASOCK::Socket> const & s,
	       ASOCK::SOCKET sock);

      ~Abstract();

      virtual ATasksList<TASK_TEMPLATE_PARAM>::ACB_type *	Behave(void);

      bool							Send(const std::string &);
      bool							Send(const char *);
      bool							Send(void *, size_t);
      bool							Refresh(void);

      void							NotifyShutdown(void);
      CLIENT::STATUS						GetStatus(void)	const;

      void							SetContent(const std::string &);
      const CLIENT::Buffer &					GetContent(void) const;
      void							SetBehaviour(const CLIENT::Abstract::Behaviour *);
      const CLIENT::Abstract::Behaviour * &			GetBehaviour(void);

    protected:

      volatile CLIENT::STATUS					_status;
      boost::shared_ptr<ASOCK::Socket>				_server;
      CLIENT::Buffer						_content;
      const CLIENT::Abstract::Behaviour *			_behaviour;
    };
};

#endif // __ACLIENT__
