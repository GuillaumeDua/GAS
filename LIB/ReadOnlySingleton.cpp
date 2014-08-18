#include "ReadOnlySingleton.hpp"

template <class Y> Y *		TemplateConstSingleton<Y>::_instance = 0x0;
template <class Y> boost::mutex	TemplateConstSingleton<Y>::_mutex;
