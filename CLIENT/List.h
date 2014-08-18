#ifndef __CLIENT_LIST__
# define __CLIENT_LIST__

# include "./AClient.h"
# include "../LIB/Lockable.h"
# include <list>

namespace CLIENT
{
  enum	UPDATE_STATUS
  {
    UPDATE_NEEDED,
    UPTODATE
  };

  class List : public Lockable
  {
  public:

    typedef	boost::shared_ptr<CLIENT::Abstract>				ClientType;
    typedef	std::list<boost::shared_ptr<CLIENT::Abstract> >			ContentType;
    typedef	std::list<boost::shared_ptr<CLIENT::Abstract> >::iterator	ContentType_iterator;

    List();
    ~List();

    CLIENT::List::ContentType &				Get(void);
    const UPDATE_STATUS					GetUpdateStatus(void) const;
    void						SetUpdateStatus(CLIENT::UPDATE_STATUS);

    void						Clear(void);

  protected:
    CLIENT::List::ContentType				_content;
    volatile UPDATE_STATUS				_upToDate;
  };
}

#endif // __CLIENT_LIST__
