#include "my_pthread.h"

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

namespace mythread
{
  class thread
  {
  private:
    static void* start_routine(void *);
  
  public:
    thread();
    virtual ~thread();
    virtual int start();
    virtual int join();
  
  protected:
    virtual int run();
  
  protected:
    pthread_t m_threadid;
  };
}

