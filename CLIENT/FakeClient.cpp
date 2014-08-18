#include "./FakeClient.h"

CLIENT::Abstract::Behaviour *		CLIENT::Fake::_static_behaviour = 0x0;
boost::mutex				CLIENT::Fake::_mutex;

void					CLIENT::CleanFakeBehaviour(void)
{
  std::cout << "Cleaning CLIENT::Fake 's behaviour" << std::endl;
  if (CLIENT::Fake::_static_behaviour != 0x0)
    delete CLIENT::Fake::_static_behaviour;
}

CLIENT::Fake::Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
		   boost::shared_ptr<ASOCK::Socket> const & s,
		   const ASOCK::Attr & a)
  : CLIENT::Abstract(tl, s, a)
{
  this->SetBehaviour(CLIENT::Fake::CreateBehaviour());
}

CLIENT::Fake::Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
		   boost::shared_ptr<ASOCK::Socket> const & s,
		   ASOCK::Attr * a)
  : CLIENT::Abstract(tl, s, a)
{
  this->SetBehaviour(CLIENT::Fake::CreateBehaviour());
}

CLIENT::Fake::Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
		   boost::shared_ptr<ASOCK::Socket> const & s,
		   ASOCK::SOCKADDR_IN * sin)
  : CLIENT::Abstract(tl, s, sin)
{
  this->SetBehaviour(CLIENT::Fake::CreateBehaviour());
}

CLIENT::Fake::Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
		   boost::shared_ptr<ASOCK::Socket> const & s,
		   const std::string & h, ASOCK::PORT p)
  : CLIENT::Abstract(tl, s, h, p)
{
  this->SetBehaviour(CLIENT::Fake::CreateBehaviour());
}

CLIENT::Fake::Fake(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
		   boost::shared_ptr<ASOCK::Socket> const & s,
		   ASOCK::SOCKET sock)
  : CLIENT::Abstract(tl, s, sock)
{
  this->SetBehaviour(CLIENT::Fake::CreateBehaviour());
}

CLIENT::Fake::~Fake()
{}

const CLIENT::Abstract::Behaviour *		CLIENT::Fake::CreateBehaviour(void)
{
  boost::mutex::scoped_lock lock(CLIENT::Fake::_mutex);
  if (CLIENT::Fake::_static_behaviour == 0x0)
    {
      CLIENT::Fake::_static_behaviour = new CLIENT::Abstract::Behaviour();

      (*CLIENT::Fake::_static_behaviour)["toto"] = static_cast<CLIENT::Abstract::CBType>(&CLIENT::Fake::sample_1);
      (*CLIENT::Fake::_static_behaviour)["titi"] = static_cast<CLIENT::Abstract::CBType>(&CLIENT::Fake::sample_2);
      (*CLIENT::Fake::_static_behaviour)["tata"] = static_cast<CLIENT::Abstract::CBType>(&CLIENT::Fake::sample_3);
    }

  return CLIENT::Fake::_static_behaviour;
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
