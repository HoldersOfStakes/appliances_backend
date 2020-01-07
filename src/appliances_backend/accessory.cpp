#include <appliances_backend/accessory.h>


namespace appliances_backend
{
  Accessory::Accessory(std::string name, std::string label, AccessoryType accessory_type)
    : name_{ name }
    , label_{ label }
    , accessory_type_{ accessory_type }
    , can_be_switched_off_{ true }
    , min_value_{ 0.0 }
    , max_value_{ 100.0 }
  {
  }

  std::string Accessory::getName() const
  {
    return name_;
  }

  std::string Accessory::getLabel() const
  {
    return label_;
  }

  AccessoryType Accessory::getType() const
  {
    return accessory_type_;
  }

  void Accessory::setCanBeSwitchedOff(bool can_be_switched_off)
  {
    can_be_switched_off_ = can_be_switched_off;
  }

  bool Accessory::getCanBeSwitchedOff() const
  {
    return can_be_switched_off_;
  }

  void Accessory::setMinValue(double min_value)
  {
    min_value_ = min_value;
  }

  double Accessory::getMinValue() const
  {
    return min_value_;
  }

  void Accessory::setMaxValue(double max_value)
  {
    max_value_ = max_value;
  }

  double Accessory::getMaxValue() const
  {
    return max_value_;
  }
}
