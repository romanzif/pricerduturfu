#include "performance.h"

Performance::Performance(Param& theParam):Option() {
	theParam.extract("strike",S_);
	theParam.extract("maturity",T_);
	theParam.extract("timestep number", TimeSteps_);
	theParam.extract("option size", size_);
	theParam.extract("payoff coefficients", payOffCoeff_, size_);
}

double Performance::payoff(const PnlMat *path) {
	PnlVect * currentrow = pnl_vect_new();
	double sum = 0;
	double numerateur = 0;
	double denominateur = 0;
	pnl_mat_get_row(currentrow, path, 0);
	for (int i = 1; i<TimeSteps_; i++) {
		denominateur = pnl_vect_scalar_prod(currentrow, payOffCoeff_);
		pnl_mat_get_row(currentrow, path, i);
		numerateur = pnl_vect_scalar_prod(currentrow, payOffCoeff_);
		sum = sum + numerateur/denominateur;
	}
	sum = sum/TimeSteps_ - 1;
}

