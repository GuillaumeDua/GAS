#ifndef __UNIQUE__
# define __UNIQUE__


#include <iostream>
#include <map>
#include <cassert>

template <class Y>
class Unique
{
public:
protected:

  Unique()
  {
    assert(Unique<Y>::_useCount == 0);
    Unique<Y>::_useCount += 1;
  }

  virtual ~Unique()
  {
    Unique<Y>::_useCount -= 1;
  }

  // volatile
  static size_t		_useCount;
};

template <class Y> size_t Unique<Y>::_useCount = 0;

#endif // __UNIQUE__
