#ifndef __READONLY_SINGLETON__
# define __READONLY_SINGLETON__

# include <boost/thread.hpp>

template <class Y>
class TemplateConstSingleton
{
public:
  static Y *			_instance;
  static boost::mutex		_mutex;

  static const Y *		GetInstance(void)
  {
    boost::mutex::scoped_lock lock(TemplateConstSingleton<Y>::_mutex);
    return (TemplateConstSingleton<Y>::_instance == 0x0		?
	    (TemplateConstSingleton<Y>::_instance = new Y())	:
	    TemplateConstSingleton<Y>::_instance);
  }

  template <class X>
  static const Y *		GetInstanceTyped(void)
  {
    boost::mutex::scoped_lock lock(TemplateConstSingleton<Y>::_mutex);
    return (TemplateConstSingleton<Y>::_instance == 0x0		?
	    (TemplateConstSingleton<Y>::_instance = new X())	:
	    TemplateConstSingleton<Y>::_instance);
  }

  static void			FreeInstance(void)
  {
    boost::mutex::scoped_lock lock(TemplateConstSingleton<Y>::_mutex);
    if (TemplateConstSingleton<Y>::_instance == 0x0) return;
    delete TemplateConstSingleton<Y>::_instance;
    TemplateConstSingleton<Y>::_instance = 0x0;
  }

protected:
  TemplateConstSingleton(){}
  TemplateConstSingleton(const TemplateConstSingleton &){}
  TemplateConstSingleton & operator=(TemplateConstSingleton const &){}
};


#endif // __READONLY_SINGLETON__
