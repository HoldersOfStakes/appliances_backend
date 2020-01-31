#ifndef __APPLIANCES_BACKEND_MANAGEABLE_BASE_H__
#define __APPLIANCES_BACKEND_MANAGEABLE_BASE_H__


// System
#include <atomic>
#include <chrono>
#include <thread>

// Private
#include <appliances_backend/logging_base.h>


namespace appliances_backend
{
  class ManageableBase : public LoggingBase
  {
  public:
    ManageableBase(Log log);
    virtual ~ManageableBase() = default;

    void start();
    void stop();

  protected:
    std::atomic<bool> should_run_;
    std::thread worker_thread_;

    virtual void initialize() {}
    virtual void run() = 0;

    void waitFor(std::chrono::steady_clock::duration duration);
  };
}


#endif /* __APPLIANCES_BACKEND_MANAGEABLE_BASE_H__*/
