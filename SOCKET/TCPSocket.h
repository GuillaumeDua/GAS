#ifndef __TCPSOCKET__
# define __TCPSOCKET__

# include "./ASocket.h"

namespace ASOCK
{
  class TCPSocket : public ASOCK::Socket
  {
  public:
    TCPSocket(const ASOCK::Socket &,
	      ASOCK::Socket::SIDE side = ASOCK::Socket::NONE);
    TCPSocket(ASOCK::SOCKET,
	      ASOCK::Socket::SIDE side = ASOCK::Socket::NONE);
    TCPSocket(const std::string &, ASOCK::PORT,
	      ASOCK::Socket::SIDE side = ASOCK::Socket::NONE);

    bool				Init(ASOCK::Socket::SIDE);
    bool				Send(const ASOCK::Attr &, const std::string &)	const;
    bool				Send(ASOCK::Attr &, void *)			const;
    bool				Send(ASOCK::Attr &, void *, size_t)		const;
    ASOCK::Socket::EXCHANGE_STATUS	Recv(const ASOCK::Attr &);
    ASOCK::Attr *			Accept(void);

  protected:
    bool				Create(void);
    bool				UnlockSock(void);
  };
}

#endif // __TCPSOCKET__
