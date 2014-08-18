#ifndef __ASOCKET__
# define __ASOCKET__


# include <cerrno>
# include <cassert>
# include <unistd.h>
# include <cstdio>
# include <cstdlib>
# include <fcntl.h>


# include "./ASocketAttr.h"
# include "./SockBuffer.h"


namespace ASOCK
{
  class Socket : public Attr
  {
  public:

    enum	SIDE
    {
      NONE	= 0,
      SERVER,
      CLIENT
    };

    enum	PROTOCOL
    {
      NOT_SET	= 0,
      TCP,
      UDP
    };

    enum	EXCHANGE_STATUS
    {
      FAILED	= 0,
      COMPLETE,
      UNCOMPLETE,
      TO_SKIP,
      CLIENT_SHUTDOWNED
    };

    enum	INIT_STATUS
    {
      NO	= 0,
      BASIC,
      FULL
    };


    static const size_t				SOMAXECONN = 128;

#define						USELOCK	assert(this->IsUseable());

    bool					IsError(void)					const;
    bool					operator!(void)					const;
    void					Error(const std::string & msg)			const;
    bool					IsUseable(void)					const;

    const ASOCK::Socket::SIDE &			GetSide(void)					const;
    const ASOCK::Socket::PROTOCOL &		GetProtocol(void)				const;
    const ASOCK::Socket::INIT_STATUS &		GetInitStatus(void)				const;
    ASOCK::Buffer &				GetBuffer(void);

    virtual bool				UnlockSock(void);

    virtual bool				Init(ASOCK::Socket::SIDE)				= 0;
    virtual bool				Send(const ASOCK::Attr &, const std::string &)	const	= 0;
    virtual bool				Send(ASOCK::Attr &, void *)			const	= 0;
    virtual bool				Send(ASOCK::Attr & s, void *, size_t)		const	= 0;
    virtual ASOCK::Socket::EXCHANGE_STATUS	Recv(const ASOCK::Attr &)				= 0;
    virtual ASOCK::Socket::EXCHANGE_STATUS	ForceCompleteRecv(const ASOCK::Attr &);
    virtual ASOCK::Attr *			Accept(void)						= 0;

    virtual ~Socket();

  protected:

    Socket(const std::string &, ASOCK::PORT);
    Socket(ASOCK::SOCKADDR_IN *);
    Socket(ASOCK::SOCKET);
    Socket(const ASOCK::Socket &);

    ASOCK::Socket &				operator=(ASOCK::Socket &);

    virtual bool				Create(void)						= 0;
    virtual bool				Connect(void);
    virtual bool				Connect(const std::string & hostname,
							const ASOCK::PORT & p);
    virtual bool				Listen(void);
    virtual bool				Bind(void);

    bool					InitAsServer_basics(void);
    bool					InitAsClient_basics(void);


    ASOCK::Buffer				_innerBuffer;
    
    // Infos attributs
    bool					_isError;
    ASOCK::Socket::SIDE				_side;
    ASOCK::Socket::PROTOCOL			_protocol;
    ASOCK::Socket::INIT_STATUS			_initStatus;
  };
}

#endif // __ASOCKET__
