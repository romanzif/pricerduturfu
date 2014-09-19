#include "asian.h"

Asian::~Asian()
{
	_PnlVect ** destruct =&payoffCoeff;
	pnl_vect_free(destruct);	
}

Asian::Asian()
	: Option()
{
	payoffCoeff=pnl_vect_new();
}

Asian::Asian(const Asian& theAsian)
	:Option(), payoffCoeff(0)
{
	payoffCoeff = pnl_vect_copy(theAsian.payoffCoeff);
	set(theAsian.getStrike(),theAsian.getMaturity(),theAsian.getTimeSteps(),theAsian.getSize());
}

Asian::Asian(_PnlVect* thePayoffCoeff, double maturity, int strike, int timeSteps, int size):Option(), payoffCoeff(0)
{
	Option::set(strike, maturity, timeSteps, size);
	payoffCoeff = pnl_vect_copy(thePayoffCoeff);

}

Asian::Asian(Param& theParam):Option(), payoffCoeff(0) {
	double theStrike, theMaturity;
	int theTimeSteps, theSize;	
        _PnlVect* thePayoffCoeff;
	theParam.extract("strike",theStrike);
	theParam.extract("maturity",theMaturity);
	theParam.extract("timestep number", theTimeSteps);
	theParam.extract("option size", theSize);
	std::cout<< theMaturity << std::endl;
	Option::set((int)theStrike,theMaturity,theTimeSteps,theSize);
	theParam.extract("payoff coefficients", thePayoffCoeff);
	if(thePayoffCoeff->size==1){
		payoffCoeff = pnl_vect_create_from_scalar(theSize,GET(thePayoffCoeff,0));
	}
	else{
		payoffCoeff = pnl_vect_copy(thePayoffCoeff);
	}
}

void Asian::print()
{
	std::cout<< strike << TimeSteps_ << T_ << size_ << std::endl;
	pnl_vect_print(payoffCoeff);
}

double Asian::payoff(const PnlMat *path) {
	return(std::max(pnl_mat_sum(path)/path->mn-strike,0.0));
}



