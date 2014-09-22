#include "barrier_l.h"


using namespace std;

Barrier_l::~Barrier_l()
{
	_PnlVect ** destruct =&payoffCoeff;
	_PnlVect ** destructLower = &lowerBarrier;
	
	pnl_vect_free(destruct);
	pnl_vect_free(destructLower);	
}

Barrier_l::Barrier_l()
	: Option()
{

	payoffCoeff=pnl_vect_new();
	lowerBarrier=pnl_vect_new();
}

Barrier_l::Barrier_l(const Barrier_l& theBarrier_l)
	:Option(), payoffCoeff(0)
{
	payoffCoeff = pnl_vect_copy(theBarrier_l.payoffCoeff);
	lowerBarrier = pnl_vect_copy(theBarrier_l.lowerBarrier);
	set(theBarrier_l.getStrike(),theBarrier_l.getMaturity(),theBarrier_l.getTimeSteps(),theBarrier_l.getSize());
}

Barrier_l::Barrier_l(_PnlVect* thePayoffCoeff, double maturity, int strike, int timeSteps, int size, _PnlVect * theLowerBarrier):Option(), payoffCoeff(0)
{
	Option::set(strike, maturity, timeSteps, size);
	payoffCoeff = pnl_vect_copy(thePayoffCoeff);
	lowerBarrier = pnl_vect_copy(theLowerBarrier);


}

Barrier_l::Barrier_l(Param& theParam):Option(), payoffCoeff(0) {
	double theStrike, theMaturity;
	int theTimeSteps, theSize;	
        _PnlVect* thePayoffCoeff; 
	_PnlVect* theLowerBarrier;
	theParam.extract("strike",theStrike);
	theParam.extract("maturity",theMaturity);
	cout<<"la matu est"<<theMaturity<<endl;
	theParam.extract("timestep number", theTimeSteps);
	theParam.extract("option size", theSize);
	Option::set((int)theStrike,theMaturity,theTimeSteps,theSize);
	theParam.extract("payoff coefficients", thePayoffCoeff);
	theParam.extract("lower barrier", theLowerBarrier);
	if(thePayoffCoeff->size==1){
		payoffCoeff = pnl_vect_create_from_scalar(theSize,GET(thePayoffCoeff,0));
		lowerBarrier = pnl_vect_create_from_scalar(theSize,GET(theLowerBarrier,0));
	}
	else{
		payoffCoeff = pnl_vect_copy(thePayoffCoeff);
		lowerBarrier = pnl_vect_copy(theLowerBarrier);
	}
}

void Barrier_l::print()
{
	std::cout<< strike << TimeSteps_ << T_ << size_ << std::endl;
	pnl_vect_print(payoffCoeff);
	pnl_vect_print(lowerBarrier);
}

double Barrier_l::payoff(const PnlMat *path) {
	_PnlVect * lastrow = pnl_vect_new();
	pnl_mat_get_row(lastrow, path, TimeSteps_-1);
	double prod = pnl_vect_scalar_prod(lastrow, payoffCoeff) - strike;
	if (std::max(prod,0.0) == 0.0) {
		return 0.0;
	}
	for (int i = 0; i < size_; i++) {
		for (int j = 0; j < TimeSteps_; j++) {
			if (MGET(path, j, i) < GET(lowerBarrier, i)) {
				return 0.0;
			}
		}
	}
	return prod;
}

