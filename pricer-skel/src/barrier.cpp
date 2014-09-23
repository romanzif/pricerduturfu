#include "barrier.h"

Barrier::~Barrier()
{
	_PnlVect ** destruct =&payoffCoeff;
	_PnlVect ** destructLower = &lowerBarrier;
	_PnlVect ** destructUpper = & upperBarrier;
	
	pnl_vect_free(destruct);
	pnl_vect_free(destructLower);
	pnl_vect_free(destructUpper);	
}

Barrier::Barrier()
	: Option()
{

	payoffCoeff=pnl_vect_new();
	upperBarrier=pnl_vect_new();
	lowerBarrier=pnl_vect_new();
}

Barrier::Barrier(const Barrier& theBarrier)
	:Option(), payoffCoeff(0)
{
	payoffCoeff = pnl_vect_copy(theBarrier.payoffCoeff);
	lowerBarrier = pnl_vect_copy(theBarrier.lowerBarrier);
	upperBarrier = pnl_vect_copy(theBarrier.upperBarrier);
	set(theBarrier.getStrike(),theBarrier.getMaturity(),theBarrier.getTimeSteps(),theBarrier.getSize());
}

Barrier::Barrier(_PnlVect* thePayoffCoeff, double maturity, int strike, int timeSteps, int size, _PnlVect * theLowerBarrier, _PnlVect * theUpperBarrier):Option(), payoffCoeff(0)
{
	Option::set(strike, maturity, timeSteps, size);
	payoffCoeff = pnl_vect_copy(thePayoffCoeff);
	upperBarrier = pnl_vect_copy(theUpperBarrier);
	lowerBarrier = pnl_vect_copy(theLowerBarrier);


}

Barrier::Barrier(Param& theParam):Option(), payoffCoeff(0) {
	double theStrike, theMaturity;
	int theTimeSteps, theSize;	
        _PnlVect* thePayoffCoeff; 
	_PnlVect* theUpperBarrier; 
	_PnlVect* theLowerBarrier;
	theParam.extract("strike",theStrike);
	theParam.extract("maturity",theMaturity);
	theParam.extract("timestep number", theTimeSteps);
	theParam.extract("option size", theSize);
	std::cout<< theMaturity << std::endl;
	Option::set((int)theStrike,theMaturity,theTimeSteps,theSize);
	theParam.extract("payoff coefficients", thePayoffCoeff);
	theParam.extract("upper barrier", theUpperBarrier);
	theParam.extract("lower barrier", theLowerBarrier);
	if(thePayoffCoeff->size==1){
		payoffCoeff = pnl_vect_create_from_scalar(theSize,GET(thePayoffCoeff,0));
		upperBarrier = pnl_vect_create_from_scalar(theSize,GET(theUpperBarrier,0));
		lowerBarrier = pnl_vect_create_from_scalar(theSize,GET(theLowerBarrier,0));
	}
	else{
		payoffCoeff = pnl_vect_copy(thePayoffCoeff);
		upperBarrier = pnl_vect_copy(theUpperBarrier);
		lowerBarrier = pnl_vect_copy(theLowerBarrier);
	}
}

void Barrier::print()
{
	std::cout<< strike << TimeSteps_ << T_ << size_ << std::endl;
	pnl_vect_print(payoffCoeff);
	pnl_vect_print(upperBarrier);
	pnl_vect_print(lowerBarrier);
}

double Barrier::payoff(const PnlMat *path) {
	_PnlVect * lastrow = pnl_vect_new();
	pnl_mat_get_row(lastrow, path, TimeSteps_-1);
	double prod = pnl_vect_scalar_prod(lastrow, payoffCoeff) - strike;
	if (std::max(prod,0.0) == 0.0) {
		return 0.0;
	}
	for (int i = 0; i < size_; i++) {
		for (int j = 0; j < TimeSteps_; j++) {
			if (MGET(path, j, i) < GET(lowerBarrier, i) || MGET(path, j, i) > GET(upperBarrier, i)) {
				return 0.0;
			}
		}
	}
	return prod;
}

