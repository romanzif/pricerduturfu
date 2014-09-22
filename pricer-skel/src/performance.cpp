#include "performance.h"

Performance::~Performance()
{
	_PnlVect ** destruct =&payoffCoeff;
	pnl_vect_free(destruct);	
}

Performance::Performance()
	: Option()
{
	payoffCoeff=pnl_vect_new();
}

Performance::Performance(const Performance& thePerformance)
	:Option(), payoffCoeff(0)
{
	payoffCoeff = pnl_vect_copy(thePerformance.payoffCoeff);
	set(thePerformance.getStrike(),thePerformance.getMaturity(),thePerformance.getTimeSteps(),thePerformance.getSize());
}

Performance::Performance(_PnlVect* thePayoffCoeff, double maturity, int strike, int timeSteps, int size):Option(), payoffCoeff(0)
{
	Option::set(strike, maturity, timeSteps, size);
	payoffCoeff = pnl_vect_copy(thePayoffCoeff);

}

Performance::Performance(Param& theParam):Option(), payoffCoeff(0) {
	double theStrike, theMaturity;
	int theTimeSteps, theSize;	
        _PnlVect* thePayoffCoeff;
	theParam.extract("strike",theStrike);
	theParam.extract("maturity",theMaturity);
	theParam.extract("timestep number", theTimeSteps);
	theParam.extract("option size", theSize);
	std::cout << theMaturity << std::endl;
	Option::set((int)theStrike,theMaturity,theTimeSteps,theSize);
	theParam.extract("payoff coefficients", thePayoffCoeff);
	if(thePayoffCoeff->size==1){
		payoffCoeff = pnl_vect_create_from_scalar(theSize,GET(thePayoffCoeff,0));
	}
	else{
		payoffCoeff = pnl_vect_copy(thePayoffCoeff);
	}
}

void Performance::print()
{
	std::cout<< strike << TimeSteps_ << T_ << size_ << std::endl;
	pnl_vect_print(payoffCoeff);
}

double Performance::payoff(const PnlMat *path) {
	_PnlVect * currentrow = pnl_vect_new();
	double sum;
	double numerateur, denominateur;
	for (int i = 2; i<TimeSteps_; i++) {
		denominateur = pnl_vect_scalar_prod(currentrow, payoffCoeff);
		pnl_mat_get_row(currentrow, path, i);
		numerateur = pnl_vect_scalar_prod(currentrow, payoffCoeff);
		sum = sum + numerateur/denominateur;
	}
	sum = sum/TimeSteps_ - 1;
	if (std::max(sum,0.0) == 0.0) {
		return 1.0;
	}
	if (sum > 0.1) {
		return 1.1;
	}	
	return (1+sum);
}

