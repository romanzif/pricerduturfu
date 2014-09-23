#include "barrier.h"

Barrier::~Barrier()
{
	pnl_vect_free(&lowerBarrier_);
	pnl_vect_free(&upperBarrier_);	
}

Barrier::Barrier(Param& theParam):Option() {
  theParam.extract("strike",S_);
  theParam.extract("maturity",T_);
  theParam.extract("timestep number", TimeSteps_);
  theParam.extract("option size", size_);
  theParam.extract("payoff coefficients", payOffCoeff_, size_);
  theParam.extract("upper barrier", upperBarrier_,size_);
  theParam.extract("lower barrier", lowerBarrier_,size_);
}

double Barrier::payoff(const PnlMat *path) {
	PnlVect * lastrow = pnl_vect_new();
	pnl_mat_get_row(lastrow, path, TimeSteps_-1);
	double prod = pnl_vect_scalar_prod(lastrow, payOffCoeff_) - S_;
	pnl_vect_free(&lastrow);
	if (std::max(prod,0.0) == 0.0) {
	  return 0.0;
	}
	for (int i = 0; i < size_; i++) {
	  for (int j = 0; j < TimeSteps_; j++) {
	    if (MGET(path, j, i) < GET(lowerBarrier_, i) || MGET(path, j, i) > GET(upperBarrier_, i)) {
	      return 0.0;
	    }
	  }
	}
	return prod;
}

