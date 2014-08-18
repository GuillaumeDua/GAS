#ifndef __FAKE_CLIENT__
# define __FAKE_CLIENT__

# include <boost/thread.hpp>
# include "./AClient.h"

namespace CLIENT
{
  class Fake : public CLIENT::Abstract
  {

  public:

    Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
	 boost::shared_ptr<ASOCK::Socket> const & s,
	 const ASOCK::Attr & a);
    Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
	 boost::shared_ptr<ASOCK::Socket> const & s,
	 ASOCK::Attr * a);
    Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
	 boost::shared_ptr<ASOCK::Socket> const & s,
	 ASOCK::SOCKADDR_IN * sin);
    Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
	 boost::shared_ptr<ASOCK::Socket> const & s,
	 const std::string & h, ASOCK::PORT p);
    Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
	 boost::shared_ptr<ASOCK::Socket> const & s,
	 ASOCK::SOCKET sock);

    ~Fake();

    static const CLIENT::Abstract::Behaviour *		CreateBehaviour(void);

    bool						sample_1(std::string s);
    bool						sample_2(std::string s);
    bool						sample_3(std::string s);

    static CLIENT::Abstract::Behaviour *		_static_behaviour;

  protected:

    static boost::mutex					_mutex;
  };

  void							CleanFakeBehaviour(void) __attribute__((destructor));
}

#endif // __FAKE_CLIENT__
