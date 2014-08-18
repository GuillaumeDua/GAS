#include "./Buffer.h"

CLIENT::Buffer::Buffer()
  : _lastExchangeStatus(ASOCK::Socket::COMPLETE)
{}

// unsecured : do not use
const std::stringstream &		CLIENT::Buffer::Get(void) const
{
  return this->_content;
}

void					CLIENT::Buffer::write(const std::string & s)
{
  this->_content.write(s.c_str(), s.length());
}

void					CLIENT::Buffer::write(const char * s, size_t len)
{
  this->_content.write(s, len);
}

// s1 : haystack
// s2 : needle
bool					StrIsPartOf(const std::string & s1,
						    const std::string & s2)
{
  int i = (s2.length() > s1.length() ? s1.length() : s2.length());

  for (i = (i > 0 ? i - 1 : i);
       ((i >= 0) &&
	(s1[i] == s2[i]));
       --i);

  if ((i + 1 == 0) && (s1[0] == s2[0])) return true;
  return false;
}

const std::string			CLIENT::Buffer::ExtractOneCompleteExchange(void)
{
  size_t	begin;
  size_t	end;

  if ((begin = this->_content.str().find(EXCHANGE::Datas::BEGIN_TAG)) == std::string::npos)
    {
      if (!StrIsPartOf(this->_content.str(), EXCHANGE::Datas::BEGIN_TAG))
	this->Clean();
      return "";
    }

  begin += EXCHANGE::Datas::BEGIN_TAG.length();
  if ((end = this->_content.str().find(EXCHANGE::Datas::END_TAG, begin)) == std::string::npos)
    return "";

  std::string		new_content	= this->_content.str().substr(end + EXCHANGE::Datas::END_TAG.length());
  const std::string	ret		= this->_content.str().substr(begin, end - begin);

  this->_content.str(std::string(""));
  this->_content.write(new_content.c_str(), new_content.length());

  return ret;
}

void					CLIENT::Buffer::Clean(void)
{
  this->_content.str(std::string(""));
  this->_content << "";
}

void					CLIENT::Buffer::SetStatus(const ASOCK::Socket::EXCHANGE_STATUS & s)
{
  this->_lastExchangeStatus = s;
}

const ASOCK::Socket::EXCHANGE_STATUS &	CLIENT::Buffer::GetStatus(void) const
{
  return (this->_lastExchangeStatus);
}
