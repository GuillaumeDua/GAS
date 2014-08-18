#ifndef __ACALLBACK__
# define __ACALLBACK__

# include "/usr/include/boost/make_shared.hpp"
# include "/usr/include/boost/shared_ptr.hpp"

# include "../LIB/Lockable.h"

template <typename R, typename P>
class ACallBack;

template <typename R, typename P>
class ACallBack
{
public:
  typedef R(*CB_as_fct)(boost::shared_ptr<Lockable>, const P &);
  typedef R(Lockable::* CB_as_mthd)(const P &);
  typedef boost::shared_ptr<Lockable>  Owner;

  ACallBack(ACallBack * _acb)
  {
    this->_fct_CB	= _acb->getFctAsCB();
    this->_mthd_CB	= _acb->getFctAsMthd();
    this->_owner	= _acb->getOwner();
    this->_param	= _acb->getParam();

    this->_isError = (this->_fct_CB == 0x0 && this->_mthd_CB == 0x0);
  }

  ACallBack(ACallBack & _acb)
  {
    this->_fct_CB	= _acb.getFctAsCB();
    this->_mthd_CB	= _acb.getFctAsMthd();
    this->_owner	= _acb.getOwner();
    this->_param	= _acb.getParam();

    this->_isError = (this->_fct_CB == 0x0 && this->_mthd_CB == 0x0);
  }

  ACallBack(CB_as_mthd cb, const P & p, Owner & o)
  {
    this->_fct_CB	= 0x0;
    this->_mthd_CB	= cb;
    this->_owner	= o;
    this->_param	= p;

    this->_isError = (this->_mthd_CB == 0x0);
  }

  ACallBack(CB_as_fct cb, const P & p, Owner & o)
  {
    this->_mthd_CB	= 0x0;
    this->_fct_CB	= cb;
    this->_owner	= o;
    this->_param	= p;

    this->_isError = (this->_fct_CB == 0x0);
  }

  inline bool	operator!(void)
  {
    return this->_isError;
  }

  bool	getError(void) const
  {
    if (this->_isError == true)
      {
	if (this->_fct_CB == 0x0 && this->_mthd_CB == 0x0)
	  std::cerr << "There's no valid callback available" << std::endl;
	if (this->_owner.get() == 0x0)
	  std::cerr << "Owner is a null ptr !" << std::endl;
      }
    return this->_isError;
  }

  R	Call(void)
  {
    this->_owner->Lock();
    R ret;

    if (this->_fct_CB == 0x0)
      ret = (*(this->_owner).*(this->_mthd_CB))(this->_param);
    else
      ret = _fct_CB(this->_owner, this->_param);

    this->_owner->Unlock();
    return ret;
  }

  R	Call(P p)
  {
    this->_owner.Lock();
    R ret;

    if (this->_fct_CB == 0x0)
      ret = (*(this->_owner))->*_mthd_CB(p);
    else if (this->_mthd_CB == 0x0)
      ret = _fct_CB(this->_owner, p);
    else std::cerr << "no valid callback found" << std::endl;

    this->Unlock();
    return ret;
  }

  R	operator()(void)
  {
    return this->Call();
  }

  R	operator()(P p)
  {
    return this->Call(p);
  }

  inline CB_as_fct	getFctAsCB(void) const
  {
    return this->_fct_CB;
  }

  inline CB_as_mthd	getFctAsMthd(void) const
  {
    return this->_mthd_CB;
  }

  inline Owner &	getOwner(void)
  {
    return this->_owner;
  }

  inline const P &	getParam(void) const
  {
    return this->_param;
  }

protected:
  Owner			_owner;
  CB_as_fct		_fct_CB;
  CB_as_mthd		_mthd_CB;
  P			_param;
  volatile bool		_isError;
};


// Void parameter specialization

template <typename R>
class ACallBack<R, void>
{
public:
  typedef R(*CB_as_fct)(boost::shared_ptr<Lockable>);
  typedef R(Lockable::* CB_as_mthd)(void);
  typedef boost::shared_ptr<Lockable>  Owner;
  typedef Lockable *		       basicOwner_ptr;

  ACallBack(ACallBack * _acb)
  {
    this->_fct_CB	= _acb->getFctAsCB();
    this->_mthd_CB	= _acb->getFctAsMthd();
    this->_owner	= _acb->getOwner();

    this->_isError = (this->_fct_CB == 0x0 && this->_mthd_CB == 0x0);
  }

  ACallBack(ACallBack & _acb)
  {
    this->_fct_CB	= _acb.getFctAsCB();
    this->_mthd_CB	= _acb.getFctAsMthd();
    this->_owner	= _acb.getOwner();

    this->_isError = (this->_fct_CB == 0x0 && this->_mthd_CB == 0x0);
  }

  ACallBack(CB_as_mthd cb, Owner & o)
  {
    this->_fct_CB	= 0x0;
    this->_mthd_CB	= cb;
    this->_owner	= o;

    this->_isError = (this->_mthd_CB == 0x0);
  }

  ACallBack(CB_as_fct cb, Owner & o)
  {
    this->_mthd_CB	= 0x0;
    this->_fct_CB	= cb;
    this->_owner	= o;

    this->_isError = (this->_fct_CB == 0x0);
  }

  inline bool	operator!(void)
  {
    return this->_isError;
  }

  bool	getError(void) const
  {
    if (this->_isError == true)
      {
	if (this->_fct_CB == 0x0 && this->_mthd_CB == 0x0)
	  std::cerr << "There's no valid callback available" << std::endl;
	if (this->_owner.get() == 0x0)
	  std::cerr << "Owner is a null ptr !" << std::endl;
      }
    return this->_isError;
  }

  R	Call(void)
  {
    this->_owner->Lock();
    R ret;

    if (this->_fct_CB == 0x0)
      ret = (*(this->_owner).*(this->_mthd_CB))();
    else
      ret = this->_fct_CB(this->_owner);

    this->_owner->Unlock();
    return ret;
  }

  R	operator()(void)
  {
    return this->Call();
  }

  inline CB_as_fct	getFctAsCB(void) const
  {
    return this->_fct_CB;
  }

  inline CB_as_mthd	getFctAsMthd(void) const
  {
    return this->_mthd_CB;
  }

  inline  Owner &	getOwner(void)
  {
    return this->_owner;
  }

protected:
  Owner			_owner;
  CB_as_fct		_fct_CB;
  CB_as_mthd		_mthd_CB;
  volatile bool		_isError;
};

// Specialization for void return

template <typename P>
class ACallBack<void, P>
{
public:
  typedef void (*CB_as_fct)(boost::shared_ptr<Lockable>, const P &);
  typedef void (Lockable::* CB_as_mthd)(const P &);
  typedef boost::shared_ptr<Lockable>  Owner;
  typedef Lockable *		       basicOwner_ptr;

  ACallBack(ACallBack * _acb)
  {
    this->_fct_CB	= _acb->getFctAsCB();
    this->_mthd_CB	= _acb->getFctAsMthd();
    this->_owner	= _acb->getOwner();
    this->_param	= _acb->getParam();

    this->_isError = (this->_fct_CB == 0x0 && this->_mthd_CB == 0x0);
  }

  ACallBack(ACallBack & _acb)
  {
    this->_fct_CB	= _acb.getFctAsCB();
    this->_mthd_CB	= _acb.getFctAsMthd();
    this->_owner	= _acb.getOwner();
    this->_param	= _acb.getParam();

    this->_isError = (this->_fct_CB == 0x0 && this->_mthd_CB == 0x0);
  }

  ACallBack(CB_as_mthd cb, const P & p, Owner & o)
  {
    this->_fct_CB	= 0x0;
    this->_mthd_CB	= cb;
    this->_owner	= o;
    this->_param	= p;

    this->_isError = (this->_mthd_CB == 0x0);
  }

  ACallBack(CB_as_fct cb, const P & p, Owner & o)
  {
    this->_mthd_CB	= 0x0;
    this->_fct_CB	= cb;
    this->_owner	= o;
    this->_param	= p;

    this->_isError = (this->_fct_CB == 0x0);
  }

  inline bool	operator!(void)
  {
    return this->_isError;
  }

  bool	getError(void) const
  {
    if (this->_isError == true)
      {
	if (this->_fct_CB == 0x0 && this->_mthd_CB == 0x0)
	  std::cerr << "There's no valid callback available" << std::endl;
	if (this->_owner.get() == 0x0)
	  std::cerr << "Owner is a null ptr !" << std::endl;
      }
    return this->_isError;
  }

  void	Call(void)
  {
    this->_owner->Lock();
    if (this->_fct_CB == 0x0)
      (*(this->_owner).*(this->_mthd_CB))(this->_param);
    else
      this->_fct_CB(this->_owner, this->_param);

    this->_owner->Unlock();
    return ;
  }

  void	Call(P p)
  {
    this->_owner.Lock();

    if (this->_fct_CB == 0x0)
      (*(this->_owner))->*_mthd_CB(p);
    else if (this->_mthd_CB == 0x0)
      this->_fct_CB(this->_owner, p);
    else std::cerr << "no valid callback found" << std::endl;

    this->Unlock();
    return ;
  }

  void	operator()(void)
  {
    this->Call();
  }

  void	operator()(P p)
  {
    this->Call(p);
  }

  inline CB_as_fct	getFctAsCB(void) const
  {
    return this->_fct_CB;
  }

  inline CB_as_mthd	getFctAsMthd(void) const
  {
    return this->_mthd_CB;
  }

  inline Owner &	getOwner(void)
  {
    return this->_owner;
  }

  inline const P &	getParam(void) const
  {
    return this->_param;
  }

protected:
  Owner			_owner;
  CB_as_fct		_fct_CB;
  CB_as_mthd		_mthd_CB;
  P			_param;
  volatile bool		_isError;
};

// Specialization for both void return and parameter


template <>
class ACallBack<void, void>
{
public:
  typedef void (*CB_as_fct)(boost::shared_ptr<Lockable>);
  typedef void (Lockable::* CB_as_mthd)(void);
  typedef boost::shared_ptr<Lockable>  Owner;
  typedef Lockable *		       basicOwner_ptr;

  ACallBack(ACallBack * _acb)
  {
    this->_fct_CB	= _acb->getFctAsCB();
    this->_mthd_CB	= _acb->getFctAsMthd();
    this->_owner	= _acb->getOwner();

    this->_isError = (this->_fct_CB == 0x0 && this->_mthd_CB == 0x0);
  }

  ACallBack(ACallBack & _acb)
  {
    this->_fct_CB	= _acb.getFctAsCB();
    this->_mthd_CB	= _acb.getFctAsMthd();
    this->_owner	= _acb.getOwner();

    this->_isError = (this->_fct_CB == 0x0 && this->_mthd_CB == 0x0);
  }

  ACallBack(CB_as_mthd cb, Owner & o)
  {
    this->_fct_CB	= 0x0;
    this->_mthd_CB	= cb;
    this->_owner	= o;

    this->_isError = (this->_mthd_CB == 0x0);
  }

  ACallBack(CB_as_fct cb, Owner & o)
  {
    this->_mthd_CB	= 0x0;
    this->_fct_CB	= cb;
    this->_owner	= o;

    this->_isError = (this->_fct_CB == 0x0);
  }

  inline bool	operator!(void)
  {
    return this->_isError;
  }

  bool	getError(void)
  {
    if (this->_isError == true)
      {
	if (this->_fct_CB == 0x0 && this->_mthd_CB == 0x0)
	  std::cerr << "There's no valid callback available" << std::endl;
	if (this->_owner.get() == 0x0)
	  std::cerr << "Owner is a null ptr !" << std::endl;
      }
    return this->_isError;
  }

  void	Call(void)
  {
    this->_owner->Lock();

    if (this->_fct_CB == 0x0)
      (*(this->_owner).*(this->_mthd_CB))();
    else
      this->_fct_CB(this->_owner);

    this->_owner->Unlock();
  }

  void	operator()(void)
  {
    this->Call();
  }

  inline CB_as_fct	getFctAsCB(void) const
  {
    return this->_fct_CB;
  }

  inline CB_as_mthd	getFctAsMthd(void) const
  {
    return this->_mthd_CB;
  }

  inline Owner &	getOwner(void)
  {
    return this->_owner;
  }

protected:
  Owner			_owner;
  CB_as_fct		_fct_CB;
  CB_as_mthd		_mthd_CB;
  volatile bool		_isError;
};

#endif // __ACALLBACK__
