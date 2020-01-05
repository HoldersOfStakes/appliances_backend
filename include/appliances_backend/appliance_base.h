#ifndef __APPLIANCES_BACKEND_APPLIANCE_BASE_H__
#define __APPLIANCES_BACKEND_APPLIANCE_BASE_H__


// System
#include <atomic>
#include <thread>


namespace appliances_backend
{
  class ApplianceBase
  {
  public:
    ApplianceBase() = default;
    virtual ~ApplianceBase() = default;

    void start();
    void stop();
    
    virtual void run() = 0;

  protected:
    std::atomic<bool> should_run_;
    std::thread worker_thread_;
  };
}


#endif /* __APPLIANCES_BACKEND_APPLIANCE_BASE_H__ */
