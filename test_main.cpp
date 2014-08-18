# include <signal.h>
# include "./SERVER/MultiThreadServer.h"

static SERVER::MultiThread * server_addr;

void	signal_cb_quit(int sig)
{
  std::cout << "quit signal caugth" << std::endl;
  sleep(1);
  server_addr->Stop();
  //delete server_addr;
}

int	main(void)
{
  SERVER::MultiThread	* s = new SERVER::MultiThread(boost::make_shared<ASOCK::TCPSocket>("127.0.0.1",
											   static_cast<ASOCK::PORT>(4242),
											   ASOCK::Socket::SERVER));

  server_addr = s;

  signal(SIGINT,  &signal_cb_quit);

  s->Run();
  delete s;
  return 0;
};
