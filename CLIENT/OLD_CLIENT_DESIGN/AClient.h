#ifndef __ACLIENT__
# define __ACLIENT__

# include <list>
# include <map>
# include <sstream>

# include <boost/shared_ptr.hpp>
# include <boost/make_shared.hpp>

# include "../LIB/Lockable.h"
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

      typedef std::list<CLIENT::Abstract *>			PtrList;
      typedef bool(CLIENT::Abstract::* CBType)(std::string);
      typedef std::map<const std::string, CBType>		CBMapType;
      typedef std::map<const std::string, CBType>::iterator	CBMapType_iterator;

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


      ATasksList<TASK_TEMPLATE_PARAM>::ACB_type *		Behave(void);

      bool							Send(const std::string &);
      bool							Send(const char *);
      bool							Send(void *, size_t);
      bool							Refresh(void);

      void							SetContent(const std::string &);
      const CLIENT::Buffer &					GetContent(void) const;
      void							NotifyShutdown(void);
      CLIENT::STATUS						GetStatus(void)	const;

      /* virtual const CBMapType				GetCBMapContent(void) = 0; */


    protected:

      virtual ATasksList<TASK_TEMPLATE_PARAM>::ACB_type *	Behave_e(const std::string &) = 0;

      volatile CLIENT::STATUS					_status;
      boost::shared_ptr<ASOCK::Socket>				_server;
      CLIENT::Buffer						_content;
    };
};

// [TODO] : remove fake quit

#define	ACLIENT_IMPLEM__H						\
  protected:								\
  static const CBMapType			GetCBMapContent(void);	\
  static CLIENT::Abstract::CBMapType		_cbMap;			\
  ATasksList<TASK_TEMPLATE_PARAM>::ACB_type *	Behave_e(const std::string & s); \


#define	ACLIENT_IMPLEM__CPP(__TYPE__)					\
  CLIENT::Abstract::CBMapType __TYPE__::_cbMap	= __TYPE__::GetCBMapContent(); \
  ATasksList<TASK_TEMPLATE_PARAM>::ACB_type *	__TYPE__::Behave_e(const std::string & s) \
  {									\
    if (s.empty() || s == "quit") return 0x0;				\
									\
    CLIENT::Abstract::CBMapType_iterator it;				\
									\
    if ((it = __TYPE__::_cbMap.find(s)) != __TYPE__::_cbMap.end())	\
      return new ATasksList<TASK_TEMPLATE_PARAM>::ACB_type(reinterpret_cast<ACallBack<TASK_TEMPLATE_PARAM>::CB_as_mthd>(it->second), \
							   s,		\
							   this->_this_shared_ptr); \
  }									\

#endif // __ACLIENT__
