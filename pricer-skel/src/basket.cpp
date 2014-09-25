#include "basket.h"

using namespace std;

Basket::~Basket()
{
  pnl_vect_free(&payOffCoeff_);
}
Basket::Basket(Param& theParam):Option() {
	theParam.extract("strike",S_);
	theParam.extract("maturity",T_);
	theParam.extract("timestep number", TimeSteps_);
	theParam.extract("option size", size_);
	theParam.extract("payoff coefficients", payOffCoeff_, size_);
}

double Basket::payoff(const PnlMat *path) {
	
	PnlVect * lastrow = pnl_vect_new();
	pnl_mat_get_row(lastrow, path, path->m-1);
	double prod = pnl_vect_scalar_prod(lastrow, payOffCoeff_) - S_;
	pnl_vect_free(&lastrow);
	return max(prod,0.0);
}

