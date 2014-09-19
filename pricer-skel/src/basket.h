#include "option.h"
#include "parser.h"

class Basket : public Option 
{
	protected :
		_PnlVect * payoffCoeff;
	public:
		double payoff(const PnlMat *path);
		void print();

	// Constructor
	Basket();
	Basket(const Basket& theBasket);
	Basket(_PnlVect * thePayoffCoeff, double maturity, int strike, int timeSteps, int size);
	Basket(Param& theParam);
	
	// Destructor 
	virtual ~Basket();


	
};
