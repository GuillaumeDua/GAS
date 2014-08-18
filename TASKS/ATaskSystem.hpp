#ifndef __ATASK_SYSTEM__
# define __ATASK_SYSTEM__

# include <list>
# include <boost/thread.hpp>

# include "/usr/include/boost/make_shared.hpp"
# include "/usr/include/boost/shared_ptr.hpp"

# include "./TaskList.hpp"

# include "./TaskProducer.hpp"
# include "./TaskConsumer.hpp"

template <typename R, typename P>
class ATaskSystem;

template <typename R, typename P>
class ATaskSystem
{
public:
  typedef TaskProducer<R, P> Producer;
  typedef TaskConsumer<R, P> Consumer;

  // Recommanded
  ATaskSystem(boost::shared_ptr<ATasksList<R,P> > const & tl,
	      size_t consumer_nbr,
	      const std::list<Producer *> &  p_l = std::list<Producer *>())
    : _isRunning(false)
  {
    this->_isError		= (consumer_nbr < 1);
    if (this->_isError) return;

    this->_producer_list	= p_l;
    this->_taskList		= tl;


    typename std::list<Producer *>::const_iterator it;

    for (it = p_l.begin(); it != p_l.end(); ++it)
      this->_isError = (*it == 0x0 ? true : this->_isError);

    size_t i;
    for (i = 0; i < consumer_nbr; ++i)
      this->_consumer_list.push_back(new TaskConsumer<R,P>(this->_taskList, this));
  };


  ATaskSystem(boost::shared_ptr<ATasksList<R,P> > const & tl,
	      std::list<Consumer *> & p_c,
	      const std::list<Producer *> & p_l = std::list<Producer *>())
    : _isRunning(false)
  {
    this->_isError		= (p_c.empty());
    if (this->_isError) return;

    this->_producer_list	= p_l;
    this->_consumer_list	= p_c;
    this->_taskList		= tl;


    typename std::list<Producer *>::const_iterator it;

    for (it = p_l.begin(); it != p_l.end(); ++it)
      this->_isError = (*it == 0x0 ? true : this->_isError);

    typename std::list<Consumer *>::iterator it_c;

    for (it_c = p_c.begin(); it_c != p_c.end(); ++it)
      this->_isError = (*it_c == 0x0 ? true : this->_isError);
  };

  //deprecated
  ATaskSystem(boost::shared_ptr<ATasksList<R,P> > const & tl,
	      std::list<Producer *> & p_l,
	      Consumer * c)
    : _isRunning(false)
  {
    this->_isError		= (c == 0x0 || p_l.empty());
    if (this->_isError) return;

    this->_taskList		= tl;
    this->_producer_list	= p_l;
    this->_consumer_list.push_back(c);

    typename std::list<Producer *>::const_iterator it;

    for (it = p_l.begin(); it != p_l.end(); ++it)
      this->_isError = (*it == 0x0 ? true : this->_isError);
  };

  //deprecated
  ATaskSystem(boost::shared_ptr<ATasksList<R,P> > const & tl,
	      Producer * p,
	      Consumer * c)
    : _isRunning(false)
  {
    this->_isError		= (p == 0x0 || c == 0x0);
    if (this->_isError) return;

    this->_producer_list.push_back(p);
    this->_consumer_list.push_back(c);
    this->_taskList		= tl;
  };

  ~ATaskSystem()
  {
    if (this->_isError)
      {
	std::cerr << "ATaskSystem : destructor : Warning : inner error, undefined behaviours or undead thread"
		  << std::endl;
	return;
      }
    this->Stop();
  }

  bool	operator!(void)
  {
    return this->_isError;
  }

  void	Stop(void)
  {
    if (this->_isError || (!(this->_isRunning))) return;
    std::cout << "stopping ..." << std::endl;

    typename std::list<Consumer *>::iterator it_c;
    typename std::list<Producer *>::iterator it_p;

    for (it_c = this->_consumer_list.begin(); it_c != this->_consumer_list.end(); ++it_c)
      (*it_c)->Stop();
    for (it_p = this->_producer_list.begin(); it_p != this->_producer_list.end(); ++it_p)
      (*it_p)->NotifyStopProduction();

    this->_t_group.join_all();

    while (!(this->_consumer_list.empty()))
      {
	for (it_c = this->_consumer_list.begin(); it_c != this->_consumer_list.end(); ++it_c)
	  {
	    if ((*it_c)->GetStatus() == Runnable::STOPPED)
	      {
		delete *it_c;
		it_c = this->_consumer_list.erase(it_c);
	      }
	  }
      }
    
    // delete task list

    this->_isRunning = false;
  }

  void	AddProducer(Producer * p)
  {
    this->_producer_list.push_back(p);
  }

  void	RemoveProducer(Producer * p)
  {
    this->_producer_list.erase(p);
  }

  void	Run(void)
  {
    if (this->_isError)
      {
	std::cerr << "ATaskSystem::Run : Cannot run because of inner error" << std::endl;
	return;
      }
    if (this->_isRunning) return;

    typename std::list<Consumer *>::iterator it;

    for (it = this->_consumer_list.begin(); it != this->_consumer_list.end(); ++it)
      this->_t_group.add_thread(new boost::thread(boost::bind(&Consumer::Run, *it)));
    this->_isRunning = true;
  }
  
private:
  volatile bool				_isError;
  boost::thread_group			_t_group;
  std::list<Producer *>			_producer_list;
  std::list<Consumer *>			_consumer_list;	// One thread requiered per consumer
  boost::shared_ptr<ATasksList<R,P> >	_taskList;
  volatile bool				_isRunning;
};


#endif // __ATASK_SYSTEM__
