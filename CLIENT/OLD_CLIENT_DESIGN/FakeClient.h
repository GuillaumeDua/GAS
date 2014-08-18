#ifndef __FAKE_CLIENT__
# define __FAKE_CLIENT__

# include "./AClient.h"

namespace CLIENT
{
  class Fake : public CLIENT::Abstract
  {
    ACLIENT_IMPLEM__H;

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



    /* ATasksList<TASK_TEMPLATE_PARAM>::ACB_type *		Behave_e(const std::string & s); */
    /* ATasksList<TASK_TEMPLATE_PARAM>::ACB_type *		getCB(const std::string & s = ""); */

    bool						sample_1(std::string s);
    bool						sample_2(std::string s);
    bool						sample_3(std::string s);

  protected:
  };
}

#endif // __FAKE_CLIENT__
