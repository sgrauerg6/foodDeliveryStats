#ifndef PROCESS_STATS_H_
#define PROCESS_STATS_H_

#include "rapidcsv.h"
#include "Delivery.h"
#include "GrubhubStats.h"
#include "UberEatsStats.h"
#include <chrono>
#include <map>

class ProcessStats {
public:
  void processGrubhubCsvFile(const std::string& file_path) {
    rapidcsv::Document doc(file_path, rapidcsv::LabelParams(0, -1));
    //convert each row in Grubhub deliveries csv file to Delivery object and add
    //to vector of Delivery objects
    for (size_t i=0; i < doc.GetRowCount(); i++) {
      const auto tip_pay = std::stof(doc.GetCell<std::string>(grubhub::kTipHeaders.at(0), i).substr(1));
      auto non_tip_pay{0.0f};
      for (const auto& non_tip_header : grubhub::kNonTipHeaders)
      {
        non_tip_pay += std::stof(doc.GetCell<std::string>(non_tip_header, i).substr(1));
      }
      const auto time_stamp = doc.GetCell<std::string>(grubhub::kTimeHeader, i);
      deliveries.push_back(Delivery(non_tip_pay, tip_pay, timestampToYMD(time_stamp)));
      deliveries.back().setOrigin(doc.GetCell<std::string>(grubhub::kOriginHeader, i));
      deliveries.back().setMiles(std::stof(doc.GetCell<std::string>(grubhub::kMilesHeader, i)));
      deliveries.back().setAccepted(
        (doc.GetCell<std::string>(grubhub::kAcceptedHeader, i) == grubhub::kOrderAccepted) ? true : false);
      deliveries.back().setCompany(grubhub::kGrubhubName);
    }
    //remove any duplicate deliveries in vector (seems that the Grubhub csv file can contain duplicates)
    std::sort(deliveries.begin(), deliveries.end());
    const auto newEnd = std::unique(deliveries.begin(), deliveries.end());
    deliveries.erase(newEnd, deliveries.cend());
  }

  void processUberEatsCsvFile(const std::string& file_path) {
    rapidcsv::Document doc(file_path, rapidcsv::LabelParams(0, -1));
    //convert each row in Grubhub deliveries csv file to Delivery object and add
    //to vector of Delivery objects
    //mapping between delivery ID and delivery
    std::map<std::string, Delivery> deliveries_id_map;
    for (size_t i=0; i < doc.GetRowCount(); i++) {
      //get the trip ID
      const auto id = doc.GetCell<std::string>(ubereats::kTripIDHeader, i);
      if (!(deliveries_id_map.contains(id))) {
        //insert delivery and retrieve/add timestamp
        deliveries_id_map.insert({id, Delivery()});
        //add timestamp
        const auto time_stamp = doc.GetCell<std::string>(ubereats::kTimeHeader, i);
        deliveries_id_map.at(id).setDate(timestampToYMD(time_stamp));
      }
      auto val_classification = doc.GetCell<std::string>(ubereats::kClassificationHeader, i);
      if (std::find(ubereats::kNonTipClass.begin(), 
                    ubereats::kNonTipClass.end(),
                    val_classification) != 
                    ubereats::kNonTipClass.end()) {
        const auto val = doc.GetCell<float>(ubereats::kEarningHeader, i);
        deliveries_id_map.at(id).addEarningsNonTip(val);
      }
      else if (std::find(ubereats::kTipClass.begin(), 
                         ubereats::kTipClass.end(),
                         val_classification) != 
                         ubereats::kTipClass.end()) {
        const auto val = doc.GetCell<float>(ubereats::kEarningHeader, i);
        deliveries_id_map.at(id).addEarningsTip(val);
      }
    }
    //add all added deliveries in map to vector of deliveries
    for (const auto& id_delivery : deliveries_id_map) {
      deliveries.push_back(id_delivery.second);
      deliveries.back().setCompany(ubereats::kUberEatsName);
    }

    //delete all deliveries before 2025
    std::erase_if(
      deliveries,
      [](const auto& delivery) { return (int)delivery.date().year() < 2025; });
  }

  void analyzeTips() const {
    const auto num_deliveries = deliveries.size();
    std::cout << "TIP ANALYSIS (ALL ORDERS)" << std::endl;
    for (float amount = 0; amount <= 15; amount += 0.5) {
      const auto order_tip_under_amount = 
        std::count_if(
          deliveries.cbegin(),
          deliveries.cend(),
          [amount](const auto& delivery) { return delivery.tip() < amount; });
      const auto order_tip_under_or_equal_amount = 
        std::count_if(
          deliveries.cbegin(),
          deliveries.cend(),
          [amount](const auto& delivery) { return delivery.tip() <= amount; });
      std::cout << "Percent of orders with tip <  $" <<
                   std::fixed << std::setprecision(2) << amount << ": " << 
                   ((float)order_tip_under_amount / (float)num_deliveries) * 100 << "%" << std::endl;
      std::cout << "Percent of orders with tip <= $" <<
                   std::fixed << std::setprecision(2) << amount << ": " << 
                   ((float)order_tip_under_or_equal_amount / (float)num_deliveries) * 100 << "%" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "TIP ANALYSIS (UBER EATS ONLY)" << std::endl;
    const auto num_deliveries_ue = 
        std::count_if(
          deliveries.cbegin(),
          deliveries.cend(),
          [](const auto& delivery) { return delivery.getCompany() == ubereats::kUberEatsName; });
    for (float amount = 0; amount <= 15; amount += 0.5) {
      const auto order_tip_under_amount_ue = 
        std::count_if(
          deliveries.cbegin(),
          deliveries.cend(),
          [amount](const auto& delivery) { return ((delivery.tip() < amount) && (delivery.getCompany() == ubereats::kUberEatsName)); });
      const auto order_tip_under_or_equal_amount_ue = 
        std::count_if(
          deliveries.cbegin(),
          deliveries.cend(),
          [amount](const auto& delivery) { return ((delivery.tip() <= amount) && (delivery.getCompany() == ubereats::kUberEatsName)); });
      std::cout << "Percent of orders with tip <  $" <<
                   std::fixed << std::setprecision(2) << amount << ": " << 
                   ((float)order_tip_under_amount_ue / (float)num_deliveries_ue) * 100 << "%" << std::endl;
      std::cout << "Percent of orders with tip <= $" <<
                   std::fixed << std::setprecision(2) << amount << ": " << 
                   ((float)order_tip_under_or_equal_amount_ue / (float)num_deliveries_ue) * 100 << "%" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "TIP ANALYSIS (GRUBHUB ONLY)" << std::endl;
    const auto num_deliveries_gh = 
        std::count_if(
          deliveries.cbegin(),
          deliveries.cend(),
          [](const auto& delivery) { return delivery.getCompany() == grubhub::kGrubhubName; });
    for (float amount = 0; amount <= 15; amount += 0.5) {
      const auto order_tip_under_amount_gh = 
        std::count_if(
          deliveries.cbegin(),
          deliveries.cend(),
          [amount](const auto& delivery) { return ((delivery.tip() < amount) && (delivery.getCompany() == grubhub::kGrubhubName)); });
      const auto order_tip_under_or_equal_amount_gh = 
        std::count_if(
          deliveries.cbegin(),
          deliveries.cend(),
          [amount](const auto& delivery) { return ((delivery.tip() <= amount) && (delivery.getCompany() == grubhub::kGrubhubName)); });
      std::cout << "Percent of orders with tip <  $" <<
                   std::fixed << std::setprecision(2) << amount << ": " << 
                   ((float)order_tip_under_amount_gh / (float)num_deliveries_gh) * 100 << "%" << std::endl;
      std::cout << "Percent of orders with tip <= $" <<
                   std::fixed << std::setprecision(2) << amount << ": " << 
                   ((float)order_tip_under_or_equal_amount_gh / (float)num_deliveries_gh) * 100 << "%" << std::endl;
    }
    std::cout << std::endl;
  }

  void analyzeLocations() const {
    std::map<std::string, unsigned int> locations_w_count;
    for (const auto& delivery : deliveries) {
       if (locations_w_count.contains(delivery.origin())) {
         locations_w_count.at(delivery.origin())++;
       }
       else {
         locations_w_count.insert({delivery.origin(), 1});
       }
    }
    std::vector<std::pair<std::string, unsigned int>> locs_counts;
    for (const auto& loc_count : locations_w_count) {
      locs_counts.push_back({loc_count.first, loc_count.second});
    }
    std::sort(locs_counts.begin(), locs_counts.end(),
      [](const auto& loc_count1, const auto& loc_count2) {
        if (loc_count1.second != loc_count2.second) {
          return loc_count1.second > loc_count2.second;
        }
        else {
          return loc_count1.first < loc_count2.first;
        }
      });
    std::cout << "LOCATIONS DELIVERED FROM W/ COUNTS" << std::endl;
    for (const auto& loc_count : locs_counts) {
      std::cout << loc_count.first << ": " << loc_count.second << std::endl;
    }
    std::cout << std::endl;
  }

private:

  std::chrono::year_month_day timestampToYMD(const std::string& timestamp) const {
    //get the year, month, date part of the timestamp
    std::array<std::string, 3> ymd;
    std::stringstream ss(timestamp);
    std::string ymd_part;
    std::getline(ss, ymd_part, ' ');

    //now get the year, month, and date
    std::stringstream ss_ymd(timestamp);
    std::getline(ss_ymd, ymd[0], '-');
    std::getline(ss_ymd, ymd[1], '-');
    std::getline(ss_ymd, ymd[2], '-');

    const auto year = std::chrono::year(std::stoi(ymd[0]));
    const auto month = std::chrono::month(std::stoi(ymd[1]));
    const auto day = std::chrono::day(std::stoi(ymd[2]));

    return std::chrono::year_month_day(year, month, day);
  }

  std::vector<Delivery> deliveries;
};

#endif //PROCESS_STATS_H_