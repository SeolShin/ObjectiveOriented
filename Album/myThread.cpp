#include "myThread.h"

using namespace mythread;

thread::thread()
{
  m_threadid = 0;
}

thread::~thread()
{
}

void* thread::start_routine(void* arg)
{
  if(arg == 0)
    return 0;
  thread* p = (thread*)arg;
  p->run();
  return arg;
}

int thread::start()
{
  return::pthread_create(&m_threadid, 0, thread::start_routine, this);
}

int thread::join()
{
  return::pthread_join(m_threadid, 0);
}

int thread::run()
{
  return 0;
}