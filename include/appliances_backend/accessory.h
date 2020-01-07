#ifndef __APPLIANCES_BACKEND_ACCESSORY_H__
#define __APPLIANCES_BACKEND_ACCESSORY_H__


// System
#include <string>


namespace appliances_backend
{
  enum class AccessoryType
  {
    Fan
  };

  class Accessory
  {
  public:
    Accessory(std::string name, std::string label, AccessoryType accessory_type);
    virtual ~Accessory() = default;

    std::string getName() const;
    std::string getLabel() const;
    AccessoryType getType() const;

    void setCanBeSwitchedOff(bool can_be_switched_off);
    bool getCanBeSwitchedOff() const;

    void setMinValue(double min_value);
    double getMinValue() const;

    void setMaxValue(double max_value);
    double getMaxValue() const;

  private:
    std::string name_;
    std::string label_;
    AccessoryType accessory_type_;
    bool can_be_switched_off_;
    double min_value_;
    double max_value_;
  };
}


#endif /* __APPLIANCES_BACKEND_ACCESSORY_H__ */
