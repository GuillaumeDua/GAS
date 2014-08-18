#include "./AClient.h"


CLIENT::Abstract::Abstract(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
			   boost::shared_ptr<ASOCK::Socket> const & s,
			   const ASOCK::Attr & a)
  : TaskProducer(tl),
    ASOCK::Attr(a),
    _server(s),
    _status(CLIENT::ALIVE)
{}

CLIENT::Abstract::Abstract(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
			   boost::shared_ptr<ASOCK::Socket> const & s,
			   ASOCK::Attr * a)
  : TaskProducer(tl),
    ASOCK::Attr(a),
    _server(s),
    _status(CLIENT::ALIVE)

{}

CLIENT::Abstract::Abstract(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
			   boost::shared_ptr<ASOCK::Socket> const & s,
			   ASOCK::SOCKADDR_IN * sin)
  : TaskProducer(tl),
    ASOCK::Attr(sin),
    _server(s),
    _status(CLIENT::ALIVE)
{}

CLIENT::Abstract::Abstract(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
			   boost::shared_ptr<ASOCK::Socket> const & s,
			   const std::string & h, ASOCK::PORT p)
  : TaskProducer(tl),
    ASOCK::Attr(h,p),
    _server(s),
    _status(CLIENT::ALIVE)
{}

CLIENT::Abstract::Abstract(boost::shared_ptr<ATasksList<TASK_TEMPLATE_PARAM> > const & tl,
			   boost::shared_ptr<ASOCK::Socket> const & s,
			   ASOCK::SOCKET sock)
  : TaskProducer(tl),
    ASOCK::Attr(sock),
    _server(s),
    _status(CLIENT::ALIVE)
{}


ATasksList<TASK_TEMPLATE_PARAM>::ACB_type *	CLIENT::Abstract::Behave(void)
{
  this->Lock();
  ATasksList<TASK_TEMPLATE_PARAM>::ACB_type * ret = this->Behave_e(this->_content.ExtractOneCompleteExchange());
  this->Unlock();
  return ret;
}



void			CLIENT::Abstract::NotifyShutdown(void)
{
  this->_status = CLIENT::SHUTDOWNED;
}

bool			CLIENT::Abstract::Send(const std::string & s)
{
  return (this->_server->Send(*this, s));
}

bool			CLIENT::Abstract::Send(const char * s)
{
  return (this->_server->Send(*this, s));
}

bool			CLIENT::Abstract::Send(void * s, size_t len)
{
  return (this->_server->Send(*this, s, len));
}

bool			CLIENT::Abstract::Refresh(void)
{
  if (this->_status == CLIENT::SHUTDOWNED)	return false;
  if (this->IsLocked())				return true;

  this->Lock();

  this->_content.SetStatus(this->_server->Recv(*this));

  switch (this->_content.GetStatus())
    {
    case ASOCK::Socket::FAILED :
      {
	this->NotifyShutdown();
	this->Unlock();
	return false;
      }
    case ASOCK::Socket::CLIENT_SHUTDOWNED :
      {
	this->NotifyShutdown();
	this->Unlock();
	return false;
      }
    default :
      {
	if (this->_content.GetStatus() != ASOCK::Socket::TO_SKIP)
	  {
	    this->_content.write(this->_server->GetBuffer().GetContent(),
				 this->_server->GetBuffer().GetSize());
	    std::cout << this << " : content : " << this->_content.Get().str() << std::endl;
	    this->Unlock();
	    return true;
	  }
      }
    }
  this->Unlock();
  return true;
}

void			CLIENT::Abstract::SetContent(const std::string & s)
{
  this->_content.Clean();
  this->_content.write(s);
}

const CLIENT::Buffer &	CLIENT::Abstract::GetContent(void) const
{
  return this->_content;
}

CLIENT::STATUS		CLIENT::Abstract::GetStatus(void) const
{
  return this->_status;
}
