#include <vector>
#include <string>

namespace grubhub {
  const std::string kGrubhubName{"Grubhub"};
  const std::string kTimeHeader{"Line Item Date Time"};
  const std::vector<std::string> kNonTipHeaders{"Delivery Base Pay Total Amount", "Delivery Bonus Pay Total Amount", "Delivery Return Pay Total Amount"};
  const std::vector<std::string> kTipHeaders{"Delivery Tips Total Amount"};
  const std::string kOriginHeader{"Delivery Pickup Name"};
  const std::string kAcceptedHeader{"Delivery Status"};
  const std::string kMilesHeader{"Delivery Engaged Distance Miles"};
  const std::string kOrderAccepted{"ACCEPTED"};
};