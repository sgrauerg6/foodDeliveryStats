#include <vector>
#include <string>

namespace ubereats {
  const std::string kTimeHeader{"Local Timestamp"};
  const std::vector<std::string> kNonTipClass{
      "transport.fare.upfront_base", "intermediary.eats.courier.fare.supplement", "intermediary.incentive.guarantee_surge",
      "delivery.charges.return_time", "delivery.fare.upfront_base"};
  const std::vector<std::string> kTipClass{"transport.misc.tip", "eats.dapi.courier_tip"};
  const std::string kEarningHeader{"Local Amount"};
  const std::string kAcceptedHeader{"Delivery Status"};
  const std::string kTripIDHeader{"Trip UUID"};
  const std::string kClassificationHeader{"Classification"};
};