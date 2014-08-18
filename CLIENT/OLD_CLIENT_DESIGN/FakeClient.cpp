#include "./FakeClient.h"

ACLIENT_IMPLEM__CPP(CLIENT::Fake);

CLIENT::Fake::Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
		   boost::shared_ptr<ASOCK::Socket> const & s,
		   const ASOCK::Attr & a)
  : CLIENT::Abstract(tl, s, a)
{}

CLIENT::Fake::Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
		   boost::shared_ptr<ASOCK::Socket> const & s,
		   ASOCK::Attr * a)
  : CLIENT::Abstract(tl, s, a)
{}

CLIENT::Fake::Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
		   boost::shared_ptr<ASOCK::Socket> const & s,
		   ASOCK::SOCKADDR_IN * sin)
  : CLIENT::Abstract(tl, s, sin)
{}

CLIENT::Fake::Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
		   boost::shared_ptr<ASOCK::Socket> const & s,
		   const std::string & h, ASOCK::PORT p)
  : CLIENT::Abstract(tl, s, h, p)
{}

CLIENT::Fake::Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
		   boost::shared_ptr<ASOCK::Socket> const & s,
		   ASOCK::SOCKET sock)
  : CLIENT::Abstract(tl, s, sock)
{}

const CLIENT::Abstract::CBMapType		CLIENT::Fake::GetCBMapContent(void)
{
  CLIENT::Fake::CBMapType m;

  m["toto"] = static_cast<CLIENT::Abstract::CBType>(&CLIENT::Fake::sample_1);
  m["titi"] = static_cast<CLIENT::Abstract::CBType>(&CLIENT::Fake::sample_2);
  m["tata"] = static_cast<CLIENT::Abstract::CBType>(&CLIENT::Fake::sample_3);

  return m;
}

bool						CLIENT::Fake::sample_1(std::string s)
{
  std::cout << "sample_1 : called : " << s << std::endl;
  return true;
}

bool						CLIENT::Fake::sample_2(std::string s)
{
  std::cout << "sample_2 : called : " << s << std::endl;
  return true;
}

bool						CLIENT::Fake::sample_3(std::string s)
{
  std::cout << "sample_3 : called : " << s << std::endl;
  return true;
}
