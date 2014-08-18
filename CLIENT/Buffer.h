#ifndef __CLIENT_BUFFER__
# define __CLIENT_BUFFER__

# include <iostream>
# include <sstream>

# include "../SOCKET/ASocket.h"
# include "../EXCHANGE/Datas.h"

namespace CLIENT
{
  class Buffer
  {
  public:
    Buffer();


    const std::stringstream &			Get(void) const;

    void					write(const std::string &);
    void					write(const char *, size_t);

    const std::string				ExtractOneCompleteExchange(void);

    void					Clean(void);

    void					SetStatus(const ASOCK::Socket::EXCHANGE_STATUS &);
    const ASOCK::Socket::EXCHANGE_STATUS &	GetStatus(void) const;

  protected:
    std::stringstream				_content;
    ASOCK::Socket::EXCHANGE_STATUS		_lastExchangeStatus;
  };
}

#endif // __CLIENT_BUFFER__
