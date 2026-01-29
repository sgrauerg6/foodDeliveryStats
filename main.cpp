#include "Delivery.h"
#include "ProcessStats.h"
#include <iostream>
#include <algorithm>

int main(int argc, char* argv[]) {
  ProcessStats proc_stats;
  proc_stats.processGrubhubCsvFile("GrubhubEarnings2025Simplified.csv");
  proc_stats.processUberEatsCsvFile("UberEarnings.csv");
  proc_stats.analyzeTips();
  //proc_stats.analyzeLocations();
  //std::sort(deliveries.begin(), deliveries.end(), [](const auto& d1, const auto& d2) { return d1.date() < d2.date();});
  /*for (const auto& d : deliveries) {
    std::cout << (int)d.date().year() << " " << (unsigned)d.date().month() << " " << (unsigned)d.date().day() << " " << d.earnings() << " " << d.tip() << std::endl;
  }*/
}