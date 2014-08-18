#ifndef __SOCK_BUFFER__
# define __SOCK_BUFFER__

# include <iostream>
# include <cstring>
# include <cassert>

namespace ASOCK
{
  class	Buffer
  {
  public:
    static const size_t	CAPACITY	= 128;

    Buffer(void);

    void		Reset(void);
    const char *	GetContent(void) const;
    char *		GetPtr(void);
    const size_t &	GetSize(void) const;
    void		Insert(const char * str, size_t size);
    void		Insert(const std::string & str);
    void		SetSize(const size_t &);

  protected:
    char		_content[ASOCK::Buffer::CAPACITY + 1];
    size_t		_size;
  };
}
#endif // __SOCK_BUFFER__

