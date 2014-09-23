#include "asian.h"

using namespace std;

Asian::Asian(Param& theParam):Option() {
	theParam.extract("strike",S_);
	theParam.extract("maturity",T_);
	theParam.extract("timestep number", TimeSteps_);
	theParam.extract("option size", size_);
	theParam.extract("payoff coefficients", payOffCoeff_, size_);
}

double Asian::payoff(const PnlMat *path) {
	return(std::max(pnl_mat_sum(path)/path->mn-S_,0.0));
}



