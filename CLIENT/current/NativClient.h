#ifndef __NATIVCLIENT__
# define __NATIVCLIENT__

# include "./AClient.h"

namespace CLIENT
{
  class Nativ : protected CLIENT::Abstract
  {
  public:
    Nativ(boost::shared_ptr<ASOCK::Socket> const & s)
      : CLIENT::Abstract(s, *(boost::static_pointer_cast<ASOCK::Attr>(s).get()))
      {}

    void	Run(void)
    {
      std::string	to_send;
      while (this->_status == CLIENT::ALIVE)
	{
	  std::cin >> to_send;
	  this->Send(to_send);
	  this->Refresh();
	  this->Behave();
	}
    }

  protected:

    bool	Behave_e(const std::string & data)
    {
      std::cout << "recved data : " << data << std::endl;
      if (data == "quit") return false;
      return true;
    }

  private:
  };

};

#endif // __NATIVCLIENT__
