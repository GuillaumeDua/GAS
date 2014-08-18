# include "./SockBuffer.h"

ASOCK::Buffer::Buffer(void)
  : _size(0)
{
  this->_content[0] = '\0';
}

// cost
void		ASOCK::Buffer::Reset(void)
{
  memset(this->_content, 0x0, this->_size); // useless security
  this->_size = 0;
}

const char *	ASOCK::Buffer::GetContent(void) const
{
  return this->_content;
}

char *		ASOCK::Buffer::GetPtr(void)
{
  return this->_content;
}

const size_t &	ASOCK::Buffer::GetSize(void) const
{
  return this->_size;
}

void		ASOCK::Buffer::Insert(const char * str, size_t size)
{
  assert(size <= ASOCK::Buffer::CAPACITY);
  memcpy(this->_content, str, size);
  this->_size = size;
}

void		ASOCK::Buffer::Insert(const std::string & str)
{
  assert(str.length() <= ASOCK::Buffer::CAPACITY);
  memcpy(this->_content, str.c_str(), str.length());
  this->_size = str.length();
}

void		ASOCK::Buffer::SetSize(const size_t & s)
{
  this->_size = s;
}
