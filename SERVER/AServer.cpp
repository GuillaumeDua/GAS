#include "./AServer.h"


SERVER::Abstract::Abstract(boost::shared_ptr<ASOCK::Socket> const & sock)
  : _isError(sock->IsError()),
    _status(SERVER::STOPED),
    _tasksList(new ATasksList<TASK_TEMPLATE_PARAM>),
    _tasksSystem(this->_tasksList,
		 SERVER::Abstract::NBR_OF_TASKS_PRODUCER)
{
  this->_sock		= sock;
}

SERVER::Abstract::Abstract(const ASOCK::Socket & sock)
  : _isError(sock.IsError()),
    _status(SERVER::STOPED),
    _tasksList(new ATasksList<TASK_TEMPLATE_PARAM>),
    _tasksSystem(this->_tasksList,
		 SERVER::Abstract::NBR_OF_TASKS_PRODUCER)
{
  switch (sock.GetProtocol())
    {
    case ASOCK::Socket::TCP :
      {
	this->_sock = boost::make_shared<ASOCK::TCPSocket>(sock);
	break;
      }
    default :
      {
	this->_isError = true;
	return;
      }
    }
}

SERVER::Abstract::~Abstract()
{
  std::cout << "SERVER::Abstract::~ : called : " << this->_isError << std::endl;
  if (this->_isError) return;
  this->Clean();
  // delete this->_sock;
}

bool			SERVER::Abstract::Run(void)
{
  if (!this || !(this->_sock))
    {
      std::cerr << SERVER::ERROR_RUN_FAILED << std::endl;
      return false;
    }

  if (this->_status != SERVER::STOPED)
    return false;

  this->_status = SERVER::RUNNING;

 start:
  this->_tasksSystem.Run();
  this->RunSystem();

  if (this->_status == SERVER::PAUSED)
    {
      this->Wait();
      goto start;
    }

  // this->Clean();
  return true;
}

void			SERVER::Abstract::Wait(void)
{
  while (this->_status == SERVER::PAUSED)
    sleep(1);
}

void			SERVER::Abstract::Pause(void)
{
  this->_status = SERVER::PAUSED;
}

void			SERVER::Abstract::Stop(void)
{
  if (this->_status == SERVER::STOPED) return;
  this->_status = SERVER::STOPED;
  this->_tasksSystem.Stop();
  std::cout << "#== Stopping ..." << std::endl;
  this->StopSystem();
  std::cout << "#== Server's StopSystem done ..." << std::endl;
}

void			SERVER::Abstract::Clean(void)
{
  if (this->_status == SERVER::RUNNING)
    {
      std::cerr << SERVER::WARNING_CLEAN_FAILED << std::endl;
      return;
    }

  std::cout << "#== SERVER::Abstract::Clean : Client's list : size : "
	    << this->_clientList->Get().size() << std::endl;
  this->_clientList->Clear();
}

bool			SERVER::Abstract::IsError(void) const
{
  return this->_isError;
}

bool			SERVER::Abstract::operator!(void) const
{
  return this->_isError;
}

const ASOCK::Socket &	SERVER::Abstract::GetSocket(void) const
{
  return *(this->_sock);
}
