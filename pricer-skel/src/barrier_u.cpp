#include "barrier_u.h"

Barrier_u::~Barrier_u()
{
	_PnlVect ** destruct =&payoffCoeff;
	_PnlVect ** destructUpper = & upperBarrier;
	
	pnl_vect_free(destruct);
	pnl_vect_free(destructUpper);	
}

Barrier_u::Barrier_u()
	: Option()
{

	payoffCoeff=pnl_vect_new();
	upperBarrier=pnl_vect_new();
}

Barrier_u::Barrier_u(const Barrier_u& theBarrier_u)
	:Option(), payoffCoeff(0)
{
	payoffCoeff = pnl_vect_copy(theBarrier_u.payoffCoeff);
	upperBarrier = pnl_vect_copy(theBarrier_u.upperBarrier);
	set(theBarrier_u.getStrike(),theBarrier_u.getMaturity(),theBarrier_u.getTimeSteps(),theBarrier_u.getSize());
}

Barrier_u::Barrier_u(_PnlVect* thePayoffCoeff, double maturity, int strike, int timeSteps, int size, _PnlVect * theUpperBarrier):Option(), payoffCoeff(0)
{
	Option::set(strike, maturity, timeSteps, size);
	payoffCoeff = pnl_vect_copy(thePayoffCoeff);
	upperBarrier = pnl_vect_copy(theUpperBarrier);


}

Barrier_u::Barrier_u(Param& theParam):Option(), payoffCoeff(0) {
	double theStrike, theMaturity;
	int theTimeSteps, theSize;	
        _PnlVect* thePayoffCoeff; 
	_PnlVect* theUpperBarrier; 
	theParam.extract("strike",theStrike);
	theParam.extract("maturity",theMaturity);
	theParam.extract("timestep number", theTimeSteps);
	theParam.extract("option size", theSize);
	std::cout<< theMaturity << std::endl;
	Option::set((int)theStrike,theMaturity,theTimeSteps,theSize);
	theParam.extract("payoff coefficients", thePayoffCoeff);
	theParam.extract("upper barrier", theUpperBarrier);
	if(thePayoffCoeff->size==1){
		payoffCoeff = pnl_vect_create_from_scalar(theSize,GET(thePayoffCoeff,0));
		upperBarrier = pnl_vect_create_from_scalar(theSize,GET(theUpperBarrier,0));
	} else{
		payoffCoeff = pnl_vect_copy(thePayoffCoeff);
		upperBarrier = pnl_vect_copy(theUpperBarrier);
	}
}

void Barrier_u::print()
{
	std::cout<< strike << std::endl;
	std::cout<< TimeSteps_ << std::endl;
	std::cout<< T_ << std::endl;
	std::cout<< size_ << std::endl;
	std::cout<< "payoff coefficients" << std::endl;
	pnl_vect_print(payoffCoeff);
	std::cout<< "upper barrier" << std::endl;
	pnl_vect_print(upperBarrier);
}

double Barrier_u::payoff(const PnlMat *path) {
	_PnlVect * lastrow = pnl_vect_new();
	pnl_mat_get_row(lastrow, path, path->m-1);
	double prod = pnl_vect_scalar_prod(lastrow, payoffCoeff) - strike;
	if (std::max(prod,0.0) == 0.0) {
		std::cout<< "ici" << std::endl;
		return 0.0;
	}
	for (int i = 0; i < size_; i++) {
		for (int j = 0; j < TimeSteps_; j++) {
			if (MGET(path, j, i) > GET(upperBarrier, i)) {
				std::cout<< "ici1" << std::endl;
				return 0.0;
			}
		}
	}
	return prod;
}
