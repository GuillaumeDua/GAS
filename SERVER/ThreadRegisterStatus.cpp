#include "./ThreadRegisterStatus.h"

ThreadRegisterStatus::ThreadRegisterStatus()
{}

ThreadRegisterStatus::~ThreadRegisterStatus()
{
  // content_iterator it;
  // for (it = this->_content.begin(); it != this->_content.end(); ++it)
  //   delete *it;
  this->_content.clear();
}

void	ThreadRegisterStatus::AddThread(boost::thread * t)
{
  this->Lock();
  if (t->get_id() != boost::this_thread::get_id())
    this->_content.push_back(t);
  this->Unlock();
}

void			ThreadRegisterStatus::NotifyStop(const boost::thread::id & id)
{
  this->Lock();
  content_iterator it;
  for (it = this->_content.begin(); it != this->_content.end(); ++it)
    {
      if ((*it)->get_id() == id)
	{
	  this->_content.erase(it);
	  this->Unlock();
	  return;
	}
    }
  this->Unlock();
}

void			ThreadRegisterStatus::NotifyStop(const boost::thread * t)
{
  this->Lock();
  content_iterator it;
  for (it = this->_content.begin(); it != this->_content.end(); ++it)
    {
      if ((*it) == t)
	{
	  this->_content.erase(it);
	  this->Unlock();
	  return;
	}
    }
  this->Unlock();
}

bool			ThreadRegisterStatus::empty(void)
{
  return this->_content.empty();
}
