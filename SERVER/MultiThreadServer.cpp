#include "./MultiThreadServer.h"

SERVER::MultiThread::MultiThread(const ASOCK::Socket & sock)
  : SERVER::Abstract(sock),
    _threadGroup(0x0)
{
  this->_clientList		= boost::make_shared<CLIENT::List>();
  this->_asyncClientList	= boost::make_shared<CLIENT::List>();
  this->_threadStatus		= boost::make_shared<ThreadRegisterStatus>();
  this->_sock->UnlockSock();
  this->_tasksSystem		= new ATaskSystem<TASK_TEMPLATE_PARAM>(this->_tasksList, 2); // test 2 consumers
}

SERVER::MultiThread::MultiThread(boost::shared_ptr<ASOCK::Socket> const & sock)
  : SERVER::Abstract(sock),
    _threadGroup(0x0)
{
  this->_clientList		= boost::make_shared<CLIENT::List>();
  this->_asyncClientList	= boost::make_shared<CLIENT::List>();
  this->_threadStatus		= boost::make_shared<ThreadRegisterStatus>();
  this->_sock->UnlockSock();
  this->_tasksSystem		= new ATaskSystem<TASK_TEMPLATE_PARAM>(this->_tasksList, 2); // test 2 consumers
}

SERVER::MultiThread::~MultiThread()
{
  if (this->_threadGroup != 0x0)
    delete _threadGroup;
  delete this->_tasksSystem;
}

void			SERVER::MultiThread::RunSystem(void)
{
  if (this->_threadGroup == 0x0)
    {
      this->_threadGroup = new boost::thread_group();

      boost::thread * t1 = new boost::thread(boost::bind(&SERVER::MultiThread::CheckNewClients,		this));
      boost::thread * t2 = new boost::thread(boost::bind(&SERVER::MultiThread::RefreshAllClients,	this));

      this->_threadGroup->add_thread(t1);
      this->_threadGroup->add_thread(t2);

      this->_threadStatus->AddThread(t1);
      this->_threadStatus->AddThread(t2);
    }

  this->_tasksSystem->Run();

  // Keep this thread (main) lock
  while (!(this->_threadStatus->empty()))
    {
      if (this->_status == SERVER::RUNNING)
	this->SyncClientLists();
      sleep(1);
    }
 }

void			SERVER::MultiThread::StopSystem(void)
{
  this->_threadGroup->join_all(); // pretty useless

  while (!(this->_threadStatus->empty()))
    {
      std::cout << "Waiting for Stop" << std::endl;
      sleep(1);
    }
  delete this->_threadGroup;
  this->_threadGroup = 0x0;

  this->_tasksSystem->Stop();
}

// thread cb method
void			SERVER::MultiThread::ExecClientsTasks(void)
{
  while (this->_status == SERVER::RUNNING)
    {
      
      boost::this_thread::sleep(boost::posix_time::milliseconds(400));
    }
  std::cout << "ExecClientsTasks : over" << std::endl;
  this->_threadStatus->NotifyStop(boost::this_thread::get_id());
}

// thread cb method
void			SERVER::MultiThread::CheckNewClients(void)
{
  unsigned int add_try = 0;

  while (this->_status == SERVER::RUNNING)
    {
      for (add_try = 0;
	   (this->_status == SERVER::RUNNING) && (add_try < 10);
	   ++add_try)
	this->AddNewClient();
      boost::this_thread::sleep(boost::posix_time::milliseconds(400));
    }
  this->_threadStatus->NotifyStop(boost::this_thread::get_id());
}

// thread cb method
void			SERVER::MultiThread::RefreshAllClients(void)
{
  CLIENT::List::ContentType_iterator		it;
  ATasksList<TASK_TEMPLATE_PARAM>::ACB_type *	cb_test;

  while (this->_status == SERVER::RUNNING)
    {
      this->_clientList->Lock();

      it = this->_clientList->Get().begin();
    res:
      for (; it != this->_clientList->Get().end(); ++it)
	{
	  if ((*it)->Refresh() == false)
	    {
	      std::cout << *it << " : deleting ..." << std::endl;
	      // this->_tasksSystem.RemoveProducer(*it); // to test
	      // delete (*it).get();
	      (*it)->CleanSelfPtr(); // -- [CURRENT] : test
	      (*it).reset();
	      it = this->_clientList->Get().erase(it);
	      goto res;
	    }
	  else if ((*it)->GetContent().GetStatus() == ASOCK::Socket::COMPLETE)
	    {
	      std::cout << *it << " : Stream content : " << (*it)->GetContent().Get().str() << std::endl;
	      (*it)->Behave();
	      // [TEST]
	      // if ((cb_test = (*it)->Behave()) != 0x0)
	      // 	{
	      // 	  cb_test->Call();
	      // 	  delete cb_test;
	      // 	}
	      // [/TEST]
	    }
	}
      this->_clientList->Unlock();
      boost::this_thread::sleep(boost::posix_time::milliseconds(400));
    }
  this->_threadStatus->NotifyStop(boost::this_thread::get_id());
}

bool			SERVER::MultiThread::AddNewClient(void)
{
  ASOCK::Attr *		tmp;

  boost::shared_ptr<CLIENT::Fake> n;

  this->_asyncClientList->Lock();
  if ((tmp = this->_sock->Accept()) != 0x0)
    {
      std::cout << "new Client temporaly add" << std::endl;

      this->_asyncClientList->Get().push_back(boost::make_shared<CLIENT::Fake>(this->_tasksList,
									       this->_sock,
									       tmp));
      (this->_asyncClientList->Get().back())->InitSelfPtr();
      //this->_tasksSystem.AddProducer(this->_asyncClientList->Get().back()); // For testing
      delete tmp;
    }
  else
    {
      this->_asyncClientList->Unlock();
      return false;
    }
  this->_asyncClientList->SetUpdateStatus(this->_asyncClientList->Get().size() != 0 ?
					  CLIENT::UPDATE_NEEDED :
					  this->_asyncClientList->GetUpdateStatus());
  this->_asyncClientList->Unlock();
  return true;
}

void			SERVER::MultiThread::SyncClientLists(void)
{
  if (this->_asyncClientList->GetUpdateStatus() != CLIENT::UPDATE_NEEDED) return;

  this->_asyncClientList->Lock();
  this->_clientList->Lock();

  CLIENT::List::ContentType_iterator it;
  for (it = this->_asyncClientList->Get().begin(); it != this->_asyncClientList->Get().end(); ++it)
    {
      if ((*it)->GetStatus() == CLIENT::ALIVE)
	{
	  this->_clientList->Get().push_back(*it);
	  std::cout << *it << " : client added" << std::endl;
	}
      
    }

  this->_asyncClientList->Get().clear();
  this->_asyncClientList->SetUpdateStatus(CLIENT::UPTODATE);

  this->_clientList->Unlock();
  this->_asyncClientList->Unlock();  
}
 
