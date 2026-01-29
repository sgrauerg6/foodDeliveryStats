
/**
 * @file Delivery.h
 * @author Scott Grauer-Gray
 * @brief Declares class corresponding to a specific delivery which
 * includes earnings from the delivery and other info.
 * 
 * @copyright Copyright (c) 2025
 */

#ifndef DELIVERY_H_
#define DELIVERY_H_

#include <chrono>
#include <string>

class Delivery {
public:
  Delivery() {}

  Delivery(float earnings_non_tip, float earnings_tip, std::chrono::year_month_day date) :
    earnings_non_tip_(earnings_non_tip), earnings_tip_(earnings_tip), date_(date) {}
  
  void addEarningsNonTip(float amount) {
    earnings_non_tip_ += amount;
  }

  void addEarningsTip(float amount) {
    earnings_tip_ += amount;
  }

  void setDate(const std::chrono::year_month_day& date) {
    date_ = date;
  }

  void setOrigin(const std::string& origin) {
    origin_ = origin;
  }

  void setMiles(float miles) {
    miles_ = miles;
  }
  void setAccepted(bool accepted) {
    accepted_ = accepted;
  }

  std::string origin() const {
    return origin_;
  }

  float earnings() const {
    return earnings_non_tip_ + earnings_tip_;
  }

  float tip() const {
    return earnings_tip_;
  }

  bool accepted() const { 
    return accepted_;
  }

  std::chrono::year_month_day date() const {
    return date_;
  }

   bool operator==(const Delivery& other) const {
        return (
          (earnings_non_tip_ == other.earnings_non_tip_) &&
          (earnings_tip_ == other.earnings_tip_) &&
          (date_ == other.date_) &&
          (origin_ == other.origin_) &&
          (miles_ == other.miles_));
    }

  bool operator<(const Delivery& other) const {
    if (date_ != other.date_) {
      return date_ < other.date_;
    }
    else if (earnings_tip_ != other.earnings_tip_) {
      return earnings_tip_ < other.earnings_tip_;
    }
    else if (date_ != other.date_) {
      return date_ < other.date_;
    }
    else if (earnings_non_tip_ != other.earnings_non_tip_) {
      return earnings_non_tip_ < other.earnings_non_tip_;
    }
    else if (origin_ != other.origin_) {
      return origin_ < other.origin_;
    }
    else if (miles_ != other.miles_) {
      return miles_ < other.miles_;
    }
    return 0;
  }

private:
  float earnings_non_tip_{0};
  float earnings_tip_{0};
  std::chrono::year_month_day date_;
  std::string origin_{};
  float miles_{0};
  bool accepted_{true};
};

#endif //DELIVERY_H_