#ifndef __TASK_LIST__
# define  __TASK_LIST__

# include <list>
# include "../LIB/Lockable.h"
# include "./ACallBack.hpp"

//
// MISC
//

# define MAX_MEM_SECURITY_ON

static const unsigned int octet		= 1;
static const unsigned int Koctet	= octet * 1024;
static const unsigned int Moctet	= Koctet * 1024;
static const unsigned int Goctet	= Moctet * 1024;

//
// Incomplete type :
//

template <typename R, typename P>
class ATasksList;

//
// Default :
//

template <typename R, typename P>
class ATasksList : public Lockable
{
public:
  typedef ACallBack<R,P>				ACB_type;
  typedef std::list<ACB_type *>				content_type;
  typedef typename std::list<ACB_type *>::iterator	content_type_iterator;

  ATasksList()
  {
#ifdef MAX_MEM_SECURITY_ON
    this->_max_elem = (100 * Moctet) / sizeof(ACB_type);
#endif
  }

  bool		Push(ACB_type * task)
  {
    if (!(*task)
#ifdef MAX_MEM_SECURITY_ON
	|| (this->_content.size() >= this->_max_elem)
#endif
	)
      return false;
    this->Lock();
    this->_content.push_back(task);
    this->Unlock();
    return true;
  }

  bool		Push(content_type task_list)
  {
    if (
#ifdef MAX_MEM_SECURITY_ON
	(this->_content.size() + task_list.size() >= this->_max_elem)
#endif
	)
      return false;
    
    content_type_iterator it;

    for (it = task_list.begin(); it != task_list.end(); ++it)
      {
	if (*it)
	  {
	    this->Lock();
	    this->_content.push_back(*it);
	    this->Unlock();
	  }
      }

    return true;
  }

  ACB_type *	Pull(void)
  {
    if (this->_content.size() < 1) return 0x0;

    this->Lock();    
    ACB_type * r = this->_content.back();
    this->_content.pop_back();
    this->Unlock();
    return r;
  }

  bool		IsNextTaskValid()
  {
    return (this->_content.size() > 1 && this->_content.last() != 0x0);
  }

  // not optimized
  R		Do(void)
  {
    this->Lock();
    ACB_type *	task = this->PullUnsafe();
    R		r;
    if (task == 0x0)
      r = 0x0;
    else
      {
	r = task->Call();
	delete task;
      }
    this->Unlock();
    return r;
  }

  // not optimized
  void		DoAll(std::list<R> & res_list)
  {
    while (this->_content.size() > 0)
      {
	std::cout << "Nbr of tasks remaining : " << this->_content.size() << std::endl;
	res_list.push_back(this->Do());
      }
  }

  // not optimized
  void		DoAll(void)
  {
    while (this->_content.size() > 0)
      {
	std::cout << "Nbr of tasks remaining : " << this->_content.size() << std::endl;
	this->Do();
      }
  }

  size_t	Size(void)
  {
    return this->_content.size();
  }

  // not optimized
  void		RemoveInvalidTasks(void)
  {
    content_type_iterator it;

    this->Lock();
    for (it = this->_content.begin(); it != this->_content.end(); ++it)
      {
	if (*it == 0x0)
	    this->_content.erase(it);
	else if (!(**it))
	  {
	    delete *it;
	    this->_content.erase(it);
	  }
      }
    this->Unlock();
  }

  // Idea : PullNTasks

protected:

  ACB_type *	PullUnsafe(void)
  {
    if (this->_content.size() < 1) return 0x0;
    
    ACB_type * r = this->_content.back();
    this->_content.pop_back();
    return r;
  }

private:
  content_type	_content;
#ifdef MAX_MEM_SECURITY_ON
  size_t	_max_elem;	// Maximum elem recommanded
#endif
};

//
// Specialization for void return :
//

template <typename P>
class ATasksList<void, P> : public Lockable
{
public:
  typedef ACallBack<void, P>				ACB_type;
  typedef std::list<ACB_type *>				content_type;
  typedef typename std::list<ACB_type *>::iterator	content_type_iterator;

  ATasksList()
  {
#ifdef MAX_MEM_SECURITY_ON
    this->_max_elem = (100 * Moctet) / sizeof(ACB_type);
#endif
  }

  bool		Push(ACB_type * task)
  {
    if (!(*task)
#ifdef MAX_MEM_SECURITY_ON
	|| (this->_content.size() >= this->_max_elem)
#endif
	)
      return false;
    this->Lock();
    this->_content.push_back(task);
    this->Unlock();
    return true;
  }

  bool		Push(content_type task_list)
  {
    if (
#ifdef MAX_MEM_SECURITY_ON
	(this->_content.size() + task_list.size() >= this->_max_elem)
#endif
	)
      return false;
    
    content_type_iterator it;

    for (it = task_list.begin(); it != task_list.end(); ++it)
      {
	if (*it)
	  {
	    this->Lock();
	    this->_content.push_back(*it);
	    this->Unlock();
	  }
      }

    return true;
  }

  ACB_type *	Pull(void)
  {
    if (this->_content.size() < 1) return 0x0;

    this->Lock();    
    ACB_type * r = this->_content.back();
    this->_content.pop_back();
    this->Unlock();
    return r;
  }

  bool		IsNextTaskValid()
  {
    return (this->_content.size() > 1 && this->_content.last() != 0x0);
  }

  // not optimized
  void		Do(void)
  {
    this->Lock();
    ACB_type *	task = this->PullUnsafe();
    this->Unlock();

    if (task == 0x0)
      return;
    else
      {
	task->Call();
	delete task;
      }
  }

  // not optimized
  void	DoAll(void)
  {
    while (this->_content.size() > 0)
      {
	std::cout << "Nbr of tasks remaining : " << this->_content.size() << std::endl;
	this->Do();
      }
  }

  size_t	Size(void)
  {
    return this->_content.size();
  }

  // Idea : PullNTasks

  // not optimized
  void		RemoveInvalidTasks(void)
  {
    content_type_iterator it;

    this->Lock();
    for (it = this->_content.begin(); it != this->_content.end(); ++it)
      {
	if (*it == 0x0)
	    this->_content.erase(it);
	else if (!(**it))
	  {
	    delete *it;
	    this->_content.erase(it);
	  }
      }
    this->Unlock();
  }

protected:

  ACB_type *	PullUnsafe(void)
  {
    if (this->_content.size() < 1) return 0x0;
    
    ACB_type * r = this->_content.back();
    this->_content.pop_back();
    return r;
  }

private:
  content_type	_content;
#ifdef MAX_MEM_SECURITY_ON
  size_t	_max_elem;	// Maximum elem recommanded
#endif
};


//
// Specialization for void parameter
//

template <typename R>
class ATasksList<R, void> : public Lockable
{
public:
  typedef ACallBack<R,void>				ACB_type;
  typedef std::list<ACB_type *>				content_type;
  typedef typename std::list<ACB_type *>::iterator	content_type_iterator;

  ATasksList()
  {
#ifdef MAX_MEM_SECURITY_ON
    this->_max_elem = (100 * Moctet) / sizeof(ACB_type);
#endif
  }

  bool		Push(ACB_type * task)
  {
    if (!(*task)
#ifdef MAX_MEM_SECURITY_ON
	|| (this->_content.size() >= this->_max_elem)
#endif
	)
      return false;
    this->Lock();
    this->_content.push_back(task);
    this->Unlock();
    return true;
  }

  bool		Push(content_type task_list)
  {
    if (
#ifdef MAX_MEM_SECURITY_ON
	(this->_content.size() + task_list.size() >= this->_max_elem)
#endif
	)
      return false;
    
    content_type_iterator it;

    for (it = task_list.begin(); it != task_list.end(); ++it)
      {
	if (*it)
	  {
	    this->Lock();
	    this->_content.push_back(*it);
	    this->Unlock();
	  }
      }

    return true;
  }

  ACB_type *	Pull(void)
  {
    if (this->_content.size() < 1) return 0x0;

    this->Lock();
    ACB_type * r = this->_content.back();
    this->_content.pop_back();
    this->Unlock();
    return r;
  }

  bool		IsNextTaskValid()
  {
    return (this->_content.size() > 1 && this->_content.last() != 0x0);
  }

  // not optimized
  R		Do(void)
  {
    this->Lock();
    ACB_type *	task = this->PullUnsafe();
    this->Unlock();

    R		r;
    if (task == 0x0)
      r = 0x0;
    else
      {
	r = task->Call();
	delete task;
      }
    return r;
  }

  // not optimized
  void	DoAll(std::list<R> & res_list)
  {
    while (this->_content.size() > 0)
      {
	std::cout << "Nbr of tasks remaining : " << this->_content.size() << std::endl;
	res_list.push_back(this->Do());
      }
  }

  // not optimized
  void	DoAll(void)
  {
    while (this->_content.size() > 0)
      {
	std::cout << "Nbr of tasks remaining : " << this->_content.size() << std::endl;
	this->Do();
      }
  }

  size_t	Size(void)
  {
    return this->_content.size();
  }

  // not optimized
  void		RemoveInvalidTasks(void)
  {
    content_type_iterator it;

    this->Lock();
    for (it = this->_content.begin(); it != this->_content.end(); ++it)
      {
	if (*it == 0x0)
	    this->_content.erase(it);
	else if (!(**it))
	  {
	    delete *it;
	    this->_content.erase(it);
	  }
      }
    this->Unlock();
  }

  // Idea : PullNTasks

protected:

  ACB_type *	PullUnsafe(void)
  {
    if (this->_content.size() < 1) return 0x0;
    
    ACB_type * r = this->_content.back();
    this->_content.pop_back();
    return r;
  }

private:
  content_type	_content;
#ifdef MAX_MEM_SECURITY_ON
  size_t	_max_elem;	// Maximum elem recommanded
#endif
};

//
// Specialization for both parameter and return void
//


template <>
class ATasksList<void, void> : public Lockable
{
public:
  typedef ACallBack<void, void>				ACB_type;
  typedef std::list<ACB_type *>				content_type;
  typedef typename std::list<ACB_type *>::iterator	content_type_iterator;

  ATasksList()
  {
#ifdef MAX_MEM_SECURITY_ON
    this->_max_elem = (100 * Moctet) / sizeof(ACB_type);
#endif
  }

  bool		Push(ACB_type * task)
  {
    if (!(*task)
#ifdef MAX_MEM_SECURITY_ON
	|| (this->_content.size() >= this->_max_elem)
#endif
	)
      return false;
    this->Lock();
    this->_content.push_back(task);
    this->Unlock();
    return true;
  }

  bool		Push(content_type task_list)
  {
    if (
#ifdef MAX_MEM_SECURITY_ON
	(this->_content.size() + task_list.size() >= this->_max_elem)
#endif
	)
      return false;
    
    content_type_iterator it;

    for (it = task_list.begin(); it != task_list.end(); ++it)
      {
	if (*it)
	  {
	    this->Lock();
	    this->_content.push_back(*it);
	    this->Unlock();
	  }
      }

    return true;
  }

  ACB_type *	Pull(void)
  {
    if (this->_content.size() < 1) return 0x0;
    
    this->Lock();
    ACB_type * r = this->_content.back();
    this->_content.pop_back();
    this->Unlock();
    return r;
  }

  bool		IsNextTaskValid()
  {
    return (this->_content.size() > 1 && this->_content.back() != 0x0);
  }

  // not optimized
  void		Do(void)
  {
    this->Lock();
    ACB_type *	task = this->PullUnsafe();
    this->Unlock();

    if (task == 0x0)
      return;
    else
      {
	task->Call();
	delete task;
      }
  }

  // not optimized
  void	DoAll(void)
  {
    while (this->_content.size() > 0)
      {
	std::cout << "Nbr of tasks remaining : " << this->_content.size() << std::endl;
	this->Do();
      }
  }

  size_t	Size(void)
  {
    return this->_content.size();
  }

  // not optimized
  void		RemoveInvalidTasks(void)
  {
    content_type_iterator it;

    this->Lock();
    for (it = this->_content.begin(); it != this->_content.end(); ++it)
      {
	if (*it == 0x0)
	    this->_content.erase(it);
	else if (!(**it))
	  {
	    delete *it;
	    this->_content.erase(it);
	  }
      }
    this->Unlock();
  }

  // Idea : PullNTasks/PullTaskTimed

protected:

  ACB_type *	PullUnsafe(void)
  {
    if (this->_content.size() < 1) return 0x0;
    
    ACB_type * r = this->_content.back();
    this->_content.pop_back();
    return r;
  }

private:
  content_type	_content;
#ifdef MAX_MEM_SECURITY_ON
  size_t	_max_elem;	// Maximum elem recommanded
#endif
};


#endif //  __TASK_LIST__
