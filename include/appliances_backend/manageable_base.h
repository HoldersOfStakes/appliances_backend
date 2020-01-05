#ifndef __APPLIANCES_BACKEND_MANAGEABLE_BASE_H__
#define __APPLIANCES_BACKEND_MANAGEABLE_BASE_H__


namespace appliances_backend
{
  class ManageableBase
  {
  public:
    ManageableBase() = default;
    virtual ~ManageableBase() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
  };
}


#endif /* __APPLIANCES_BACKEND_MANAGEABLE_BASE_H__*/
