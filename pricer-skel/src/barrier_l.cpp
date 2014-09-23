#include "barrier_l.h"


using namespace std;

Barrier_l::~Barrier_l()
{
	pnl_vect_free(&lowerBarrier_);	
}

Barrier_l::Barrier_l(Param& theParam):Option() {
  theParam.extract("strike",S_);
  theParam.extract("maturity",T_);
  theParam.extract("timestep number", TimeSteps_);
  theParam.extract("option size", size_);
  theParam.extract("payoff coefficients", payOffCoeff_, size_);
  theParam.extract("lower barrier", lowerBarrier_, size_);
}

double Barrier_l::payoff(const PnlMat *path) {
	_PnlVect * lastrow = pnl_vect_new();
	pnl_mat_get_row(lastrow, path, path->m-1);
	double prod = pnl_vect_scalar_prod(lastrow, payOffCoeff_) - S_;
	pnl_vect_free(&lastrow);
	if (max(prod,0.0) == 0.0) {
	  return 0.0;
	}
	for (int i = 0; i < size_; i++) {
	  for (int j = 0; j < TimeSteps_; j++) {
	    if (MGET(path, j, i) < GET(lowerBarrier_, i)) {
	      return 0.0;
	    }
	  }
	}
	return prod;
}

