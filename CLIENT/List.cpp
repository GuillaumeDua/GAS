#include "./List.h"

CLIENT::List::List()
  : _upToDate(CLIENT::UPTODATE)
{}

CLIENT::List::~List()
{
  this->Clear();
}

CLIENT::List::ContentType &	CLIENT::List::Get(void)
{
  return this->_content;
}

const CLIENT::UPDATE_STATUS	CLIENT::List::GetUpdateStatus(void) const
{
  return this->_upToDate;
}

void				CLIENT::List::SetUpdateStatus(CLIENT::UPDATE_STATUS s)
{
  this->_upToDate = s;
}

void				CLIENT::List::Clear(void)
{
  CLIENT::List::ContentType_iterator	it;

  for (it = this->Get().begin(); it != this->Get().end(); ++it)
    {
      std::cout << "use count : " << (*it).use_count() << std::endl;
      (*it)->CleanSelfPtr();
      (*it).reset();
    }
  this->Get().clear();
}
