#include "./ASocket.h"


ASOCK::Socket::Socket(const std::string & h, ASOCK::PORT p)
  : ASOCK::Attr(h,p),
    _side(ASOCK::Socket::NONE),
    _protocol(ASOCK::Socket::NOT_SET),
    _initStatus(ASOCK::Socket::NO),
    _isError(false)
{
}

ASOCK::Socket::Socket(ASOCK::SOCKET s)
  : ASOCK::Attr(s),
    _side(ASOCK::Socket::NONE),
    _protocol(ASOCK::Socket::NOT_SET),
    _initStatus(ASOCK::Socket::NO),
    _isError((s == ASOCK::ERROR ? true : false))
{
}

ASOCK::Socket::Socket(ASOCK::SOCKADDR_IN * addr)
  : ASOCK::Attr(addr),
    _side(ASOCK::Socket::NONE),
    _protocol(ASOCK::Socket::NOT_SET),
    _initStatus(ASOCK::Socket::NO),
    _isError(false)
{
}

ASOCK::Socket::Socket(const ASOCK::Socket & s)
  : ASOCK::Attr(static_cast<ASOCK::Attr>(s)),
    _side(s.GetSide()),
    _protocol(s.GetProtocol()),
    _initStatus(s.GetInitStatus()),
    _isError(!s)
{
}

ASOCK::Socket::~Socket()
{
  if (this->_initStatus == ASOCK::Socket::NO)
    return ;

  if	(this->_protocol == ASOCK::Socket::TCP)
    closesocket(this->_sock);
  else if (this->_protocol == ASOCK::Socket::UDP)
    {
      delete this->_addrin;
      delete this->_hostInfo;
    }
}

ASOCK::Socket &		ASOCK::Socket::operator=(ASOCK::Socket & s)
{
  this->_side		= s.GetSide();
  this->_protocol	= s.GetProtocol();
  this->_initStatus	= s.GetInitStatus();
  this->_isError	= !s;

  if (this->_protocol == ASOCK::Socket::TCP ||
      this->_side == ASOCK::Socket::SERVER)
    this->_sock	= s.GetSocket();

  if (&(s.GetSockAddrIn()) != NULL)
    this->SetSockAddrIn(s.GetSockAddrIn());
  if (&(s.GetHostInfo()) != NULL)
    this->SetHostInfo(s.GetHostInfo());

  return *this;
}


const ASOCK::Socket::SIDE &    		ASOCK::Socket::GetSide(void) const
{
  return this->_side;
}

const ASOCK::Socket::PROTOCOL &    	ASOCK::Socket::GetProtocol(void) const
{
  return this->_protocol;
}

const ASOCK::Socket::INIT_STATUS &    	ASOCK::Socket::GetInitStatus(void) const
{
  return this->_initStatus;
}

ASOCK::Buffer &				ASOCK::Socket::GetBuffer(void)
{
  return this->_innerBuffer;
}

bool					ASOCK::Socket::operator!(void) const
{
  return (this->_isError ||
	  this->_sock == ASOCK::ERROR);
}

bool					ASOCK::Socket::IsError(void) const
{
  return (this->_isError ||
	  this->_sock == ASOCK::ERROR);
}

void					ASOCK::Socket::Error(const std::string & msg) const
{
  std::cerr << "[" << this << "] : " << msg
#ifdef linux
	    << " : " << strerror(errno)
#endif
	    << std::endl;
}

bool					ASOCK::Socket::IsUseable(void) const
{  
  return (!(*this) == false	&&
	  this->_initStatus > ASOCK::Socket::NO);
}



ASOCK::Socket::EXCHANGE_STATUS		ASOCK::Socket::ForceCompleteRecv(const ASOCK::Attr & from)
{
  ASOCK::Socket::EXCHANGE_STATUS ret;

  while ((ret = this->Recv(from)) == ASOCK::Socket::UNCOMPLETE);
  return ret;
}

bool					ASOCK::Socket::UnlockSock(void)
{
  USELOCK;
  return true;
}

bool					ASOCK::Socket::Bind(void)
{
  USELOCK;

  if (bind(this->_sock,
	   reinterpret_cast<SOCKADDR*>(this->_addrin),
	   ASOCK::Attr::ADDR_LEN)
      == ASOCK::ERROR)
    {
      this->Error("Bind failed");
      this->_isError = true;
      return false;
    }
  return true;
}

bool					ASOCK::Socket::Listen(void)
{
  USELOCK;

  if (listen(this->_sock, ASOCK::Socket::SOMAXECONN) == ASOCK::ERROR)
    {
      this->Error("Listen failed");
      this->_isError = true;
      return false;
    }
  return true;
}

bool					ASOCK::Socket::Connect(const std::string & h,
							       const ASOCK::PORT & p)
{
  USELOCK;

  if (h.empty() || p == ASOCK::ERROR)
    {
      this->Error("Connect error : no hostname or port specified");
      return false;
    }

  this->_hostName	= h;
  this->_port		= p;

  if (connect(this->_sock,
	      reinterpret_cast<SOCKADDR *>(this->_addrin),
	      ASOCK::Attr::ADDR_LEN) == ASOCK::ERROR)
    {
      this->Error("Connect failed");
      this->_isError = true;
      return false;
    }
  return true;
}


bool					ASOCK::Socket::Connect(void)
{
  USELOCK;

  if (this->_hostName.empty() || this->_port == ASOCK::ERROR)
    {
      this->Error("Connect error : invalid hostname or port");
      return false;
    }

  if (connect(this->_sock,
	      reinterpret_cast<SOCKADDR *>(this->_addrin),
	      ASOCK::Attr::ADDR_LEN) == ASOCK::ERROR)
    {
      this->Error("Connect failed");
      this->_isError = true;
      return false;
    }
  return true;
}

bool					ASOCK::Socket::InitAsServer_basics(void)
{
  if (!(this->Create()))
    return false;

  if (this->_addrin == NULL) 
    this->_addrin	= new ASOCK::SOCKADDR_IN;

  memset(this->_addrin, 0x0, sizeof(ASOCK::SOCKADDR_IN));

  this->_addrin->sin_addr.s_addr	= htonl(INADDR_ANY);
  this->_addrin->sin_family		= AF_INET;
  this->_addrin->sin_port		= htons(this->_port);

  this->_initStatus			= ASOCK::Socket::BASIC;
  this->_side				= ASOCK::Socket::SERVER;

  return true;
}

bool					ASOCK::Socket::InitAsClient_basics(void)
{
  if (!(this->Create()))
    return false;

  if (this->_addrin == NULL) 
    this->_addrin	= new SOCKADDR_IN;

  memset(this->_addrin, 0x0, sizeof(ASOCK::SOCKADDR_IN));

  if ((this->_hostInfo = gethostbyname(this->_hostName.c_str())) == NULL)
    {
      this->Error("Gethostbyname failed");
      return false;
    }

  this->_addrin->sin_addr		= *(reinterpret_cast<IN_ADDR *>(this->_hostInfo->h_addr));
  this->_addrin->sin_port		= htons(this->_port);
  this->_addrin->sin_family		= AF_INET;

  this->_initStatus			= ASOCK::Socket::BASIC;
  this->_side				= ASOCK::Socket::CLIENT;

  return true;
}

