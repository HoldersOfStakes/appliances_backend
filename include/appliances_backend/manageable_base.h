#ifndef __APPLIANCES_BACKEND_MANAGEABLE_BASE_H__
#define __APPLIANCES_BACKEND_MANAGEABLE_BASE_H__


// System
#include <atomic>
#include <thread>


namespace appliances_backend
{
  class ManageableBase
  {
  public:
    ManageableBase() = default;
    virtual ~ManageableBase() = default;

    void start();
    void stop();

  protected:
    std::atomic<bool> should_run_;
    std::thread worker_thread_;

    virtual void run() = 0;
  };
}


#endif /* __APPLIANCES_BACKEND_MANAGEABLE_BASE_H__*/
