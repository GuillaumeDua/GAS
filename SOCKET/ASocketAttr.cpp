#include "./ASocketAttr.h"

socklen_t	ASOCK::Attr::ADDR_LEN = sizeof(ASOCK::SOCKADDR_IN);
//socklen_t *	ASOCK::Attr::ADDR_LEN_PTR_noconst = const_cast<socklen_t *>(&(ASOCK::Attr::ADDR_LEN));

ASOCK::Attr::Attr(const std::string & h, ASOCK::PORT p)
  : _hostName(h),
    _port(p),
    _sock(ASOCK::ERROR),
    _hostInfo(NULL),
    _addrin(NULL),
    _isDependant(false)
{}

// no cpy
ASOCK::Attr::Attr(ASOCK::SOCKADDR_IN * addr_in)
  : _hostName(""),
    _port(ASOCK::ERROR),
    _hostInfo(NULL),
    _sock(ASOCK::ERROR),
    _addrin(addr_in),
    _isDependant(false)
{}

ASOCK::Attr::Attr(const ASOCK::SOCKET & s)
  : _hostName(""),
    _port(ASOCK::ERROR),
    _hostInfo(NULL),
    _sock(s),
    _addrin(NULL),
    _isDependant(false)
{}

// Danger in TCP : socket may be closed by attr before this.
ASOCK::Attr::Attr(const ASOCK::Attr & attr)
  : _hostName(attr.GetHostName()),
    _port(attr.GetPort()),
    _sock(attr.GetSocket()),
    _isDependant(false)
{
  SecurePtrCopy_freeArea<ASOCK::HOSTENT>(this->_hostInfo, &(attr.GetHostInfo()));
  SecurePtrCopy_freeArea<ASOCK::SOCKADDR_IN>(this->_addrin, &(attr.GetSockAddrIn()));
}

// Mirror constructor
ASOCK::Attr::Attr(ASOCK::Attr * attr)
  : _hostName(attr->GetHostName()),
    _port(attr->GetPort()),
    _sock(attr->GetSocket()),
    _addrin(&(attr->GetSockAddrIn_unsafe())),
    _hostInfo(&(attr->GetHostInfo_unsafe())),
    _isDependant(false)
{
  attr->SetDependance(true);
}


ASOCK::Attr::~Attr()
{
  if (this->_isDependant) return;

  if (this->_addrin	!= NULL) delete this->_addrin;
  if (this->_hostInfo	!= NULL) delete this->_hostInfo;
  if (this->_sock != ASOCK::ERROR)
    closesocket(this->_sock);
}


const ASOCK::SOCKET &		ASOCK::Attr::GetSocket(void) const
{
  return this->_sock;
}

const ASOCK::SOCKADDR_IN &	ASOCK::Attr::GetSockAddrIn(void) const
{
  return *(this->_addrin);
}

const ASOCK::HOSTENT &		ASOCK::Attr::GetHostInfo(void) const
{
  return *(this->_hostInfo);
}

ASOCK::SOCKADDR_IN &		ASOCK::Attr::GetSockAddrIn_unsafe(void)
{
  return *(this->_addrin);
}

ASOCK::HOSTENT &		ASOCK::Attr::GetHostInfo_unsafe(void)
{
  return *(this->_hostInfo);
}

const ASOCK::PORT &		ASOCK::Attr::GetPort(void)  const
{
  return this->_port;
}

const std::string &		ASOCK::Attr::GetHostName(void) const
{
  return this->_hostName;
}

bool				ASOCK::Attr::GetDependance(void) const
{
  return this->_isDependant;
}

void				ASOCK::Attr::SetSocket(const ASOCK::SOCKET & s)
{
  this->_sock = s;
}

void				ASOCK::Attr::SetSockAddrIn(const ASOCK::SOCKADDR_IN & addrin)
{
  SecurePtrCopy(this->_addrin, &addrin);
}

void				ASOCK::Attr::SetHostInfo(const ASOCK::HOSTENT & hostInfo)
{
  SecurePtrCopy(this->_hostInfo, &hostInfo);
}

void				ASOCK::Attr::SetPort(const ASOCK::PORT & p)
{
  this->_port = p;
}

void				ASOCK::Attr::SetHostName(const std::string & hostName)
{
  this->_hostName = hostName;
}


void				ASOCK::Attr::SetDependance(bool d)
{
  this->_isDependant = d;
}
