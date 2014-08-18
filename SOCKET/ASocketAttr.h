#ifndef __ASOCKETATTR__
# define __ASOCKETATTR__

# include <iostream>
# include <cstring>
# include "../MISC/MemoryTools.h"

# ifdef WIN32
#  include <winsock2.h>
# endif

# ifdef linux
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <unistd.h>
#  include <netdb.h>
#endif

namespace ASOCK
{
# ifdef WIN32
#  define  WSAGetLastError
# endif

# ifdef linux
#  define  closesocket(s)	close(s)

  static const int		ERROR =	-1;

  typedef int			SOCKET;
  typedef int			PORT;

  typedef struct sockaddr_in	SOCKADDR_IN;
  typedef struct sockaddr	SOCKADDR;
  typedef struct in_addr	IN_ADDR;
  typedef struct hostent	HOSTENT;
#endif


  class Attr
  {
  public :
    static socklen_t		ADDR_LEN;// = sizeof(ASOCK::SOCKADDR_IN);
    /* static socklen_t *		ADDR_LEN_PTR_noconst; */

    Attr(const std::string &, ASOCK::PORT);
    Attr(const ASOCK::Attr &);
    Attr(ASOCK::Attr *);
    Attr(ASOCK::SOCKADDR_IN *);
    Attr(const ASOCK::SOCKET &);

    virtual ~Attr();

    const ASOCK::SOCKET &	GetSocket(void)			const;
    const ASOCK::SOCKADDR_IN & 	GetSockAddrIn(void)		const;
    const ASOCK::HOSTENT &	GetHostInfo(void)		const;
    ASOCK::SOCKADDR_IN & 	GetSockAddrIn_unsafe(void);
    ASOCK::HOSTENT &		GetHostInfo_unsafe(void);
    const ASOCK::PORT &		GetPort(void)			const;
    const std::string &		GetHostName(void)		const;
    bool			GetDependance(void)		const;

    void			SetSocket(const ASOCK::SOCKET &);
    void			SetSockAddrIn(const ASOCK::SOCKADDR_IN &);
    void			SetHostInfo(const ASOCK::HOSTENT &);
    void			SetPort(const ASOCK::PORT &);
    void			SetHostName(const std::string &);
    void			SetDependance(bool);

  protected:
    SOCKET			_sock;
    SOCKADDR_IN	*		_addrin;
    HOSTENT *			_hostInfo;

    std::string			_hostName;
    PORT		       	_port;

    volatile bool		_isDependant;
  };
}
#endif // __ASOCKETATTR__
