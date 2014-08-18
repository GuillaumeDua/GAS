#include "./TCPSocket.h"

ASOCK::TCPSocket::TCPSocket(const ASOCK::Socket & s,
			    ASOCK::Socket::SIDE side)
  : ASOCK::Socket(s)
{
  this->_protocol	= ASOCK::Socket::TCP;
  this->Init(side);
}

ASOCK::TCPSocket::TCPSocket(ASOCK::SOCKET s,
			    ASOCK::Socket::SIDE side)
  : ASOCK::Socket(s)
{
  this->_protocol	= ASOCK::Socket::TCP;
  this->Init(side);
}

ASOCK::TCPSocket::TCPSocket(const std::string & h, ASOCK::PORT p,
			    ASOCK::Socket::SIDE side)
  : ASOCK::Socket(h,p)
{
  this->_protocol	= ASOCK::Socket::TCP;
  this->Init(side);
}

bool				ASOCK::TCPSocket::Create(void)
{
  //USELOCK;

  if ((this->_sock = socket(AF_INET,
			    SOCK_STREAM,
			    0))
      == ASOCK::ERROR)
    {
      this->Error("Socket failed");
      this->_isError = true;
      return false;
    }
  return true;
}

ASOCK::Attr *			ASOCK::TCPSocket::Accept(void)
{
  USELOCK;
  
  ASOCK::SOCKET		client_sock;
  ASOCK::SOCKADDR_IN	csin;

  memset(&csin, 0x0, sizeof(SOCKADDR_IN));

  ASOCK::Attr::ADDR_LEN = sizeof(SOCKADDR_IN);

  if ((client_sock = accept(this->_sock,
			    reinterpret_cast<SOCKADDR *>(&csin),
			    &(ASOCK::Attr::ADDR_LEN)))
      == ASOCK::ERROR)
    {
      if (errno != EAGAIN && errno != EWOULDBLOCK)
	{
	  this->Error("Accept failed");
	  this->_isError = true;
	}
      return NULL;
    }
  return new ASOCK::Attr(client_sock);
}

// null-terminated memarea
bool				ASOCK::TCPSocket::Send(ASOCK::Attr & to, void * data) const
{
  USELOCK;

  if (data == 0x0	||
      sendto(this->_sock,
	     data,
	     strlen(static_cast<char *>(data)),
	     0,
	     reinterpret_cast<ASOCK::SOCKADDR *>(&(to.GetSockAddrIn_unsafe())),
	     sizeof(to.GetSockAddrIn())) 
      == ASOCK::ERROR)
    {
      this->Error("Send failed");
      return false;
    }
  return true;
}

bool				ASOCK::TCPSocket::Send(ASOCK::Attr & to, void * data, size_t data_len) const
{
  USELOCK;

  if (data == NULL	||
      data_len == 0	||
      sendto(this->_sock,
	     data,
	     data_len,
	     0,
	     reinterpret_cast<SOCKADDR *>(&(to.GetSockAddrIn_unsafe())),
	     sizeof(to.GetSockAddrIn())) 
      == ASOCK::ERROR)
    {
      this->Error("Send failed");
      return false;
    }
  return true;
}


bool				ASOCK::TCPSocket::Send(const ASOCK::Attr & to, const std::string & data) const
{
  USELOCK;

  if (data.empty()	||
      send(to.GetSocket(),
	   data.c_str(),
	   data.length(),
	   0)
      == ASOCK::ERROR)
    {
      this->Error("Send failed");
      return false;
    }
  return true;
}

ASOCK::Socket::EXCHANGE_STATUS	ASOCK::TCPSocket::Recv(const ASOCK::Attr & from)
{
  USELOCK;

  int	ret = 0;

  if ((ret = recv(from.GetSocket(),
		  this->_innerBuffer.GetPtr(),
		  ASOCK::Buffer::CAPACITY,
		  MSG_DONTWAIT))
      < 0)
    {

      if (errno ==  EAGAIN || errno == EWOULDBLOCK)
	return ASOCK::Socket::TO_SKIP;

      this->Error("Recv failed");
      this->_isError = true;
      return FAILED;
    }



  if (ret == 0) return ASOCK::Socket::CLIENT_SHUTDOWNED;

  if (ret >=  2						&&
      this->_innerBuffer.GetContent()[ret - 2] == '\r'	&&
      this->_innerBuffer.GetContent()[ret - 1] == '\n')
    {
      this->_innerBuffer.GetPtr()[ret - 2] = '\0';
      this->_innerBuffer.SetSize(ret - 2);
    }
  else
    {
      this->_innerBuffer.GetPtr()[ret] = '\0';
      this->_innerBuffer.SetSize(ret);
    }

  return ((static_cast<size_t>(ret) == ASOCK::Buffer::CAPACITY
	   ? ASOCK::Socket::UNCOMPLETE
	   : ASOCK::Socket::COMPLETE));
}

bool				ASOCK::TCPSocket::Init(ASOCK::Socket::SIDE s)
{
  assert(this->_initStatus == ASOCK::Socket::NO);

  if (s == ASOCK::Socket::SERVER)
    {
      this->InitAsServer_basics();
      if (this->Bind() == false)	return false;
      if (this->Listen() == false)	return false;
    }
  else if (s == ASOCK::Socket::CLIENT)
    {
      this->InitAsClient_basics();
      if (this->Connect() == false)	return false;
    }
  else
    return false;

  this->_initStatus = FULL;
  return true;
}

bool				ASOCK::TCPSocket::UnlockSock(void)
{
  USELOCK;

  if (fcntl(this->_sock, F_GETFL) & O_NONBLOCK)
    return true;

  if (fcntl(this->_sock, F_SETFL, fcntl(this->_sock, F_GETFL) | O_NONBLOCK) < 0)
    {
      this->Error("Fcntl failed : forcing socket to unlock");
      this->_isError = true;
      return false;
    }
  return true;
}

