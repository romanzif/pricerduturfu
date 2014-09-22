#include "basket.h"

using namespace std;

Basket::~Basket()
{
	_PnlVect ** destruct =&payoffCoeff;
	pnl_vect_free(destruct);

}

Basket::Basket()
	: Option()
{
	payoffCoeff=pnl_vect_new();
}

Basket::Basket(const Basket& theBasket)
	:Option(), payoffCoeff(0)
{
	payoffCoeff = pnl_vect_copy(theBasket.payoffCoeff);
	set(theBasket.getStrike(),theBasket.getMaturity(),theBasket.getTimeSteps(),theBasket.getSize());
}

Basket::Basket(_PnlVect* thePayoffCoeff, double maturity, int strike, int timeSteps, int size):Option(), payoffCoeff(0)
{
	Option::set(strike, maturity, timeSteps, size);
	payoffCoeff = pnl_vect_copy(thePayoffCoeff);


}

Basket::Basket(Param& theParam):Option(), payoffCoeff(0) {
	double theStrike, theMaturity;
	int theTimeSteps, theSize;	
        _PnlVect* thePayoffCoeff; 
	theParam.extract("strike",theStrike);
	theParam.extract("maturity",theMaturity);
	theParam.extract("timestep number", theTimeSteps);
	theParam.extract("option size", theSize);
	Option::set((int)theStrike,theMaturity,theTimeSteps,theSize);
	theParam.extract("payoff coefficients", thePayoffCoeff);
	if(thePayoffCoeff->size==1){
		payoffCoeff = pnl_vect_create_from_scalar(theSize,GET(thePayoffCoeff,0));
	}
	else{
		payoffCoeff = pnl_vect_copy(thePayoffCoeff);
	}
}

void Basket::print()
{
	std::cout<< strike << TimeSteps_ << T_ << size_ << std::endl;
	pnl_vect_print(payoffCoeff);
}

double Basket::payoff(const PnlMat *path) {
	
	PnlVect * lastrow = pnl_vect_new();
	pnl_mat_get_row(lastrow, path, path->m-1);
	//cout<<"BOUOUUUUUUUUUU"<<GET(lastrow,4)<<endl;
	
	double prod = pnl_vect_scalar_prod(lastrow, payoffCoeff) - strike;

	if (std::max(prod,0.0) == 0.0) {
			return 0.0;
	}
	return prod;
}

