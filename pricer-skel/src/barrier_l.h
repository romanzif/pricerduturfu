#include "option.h"
#include "parser.h"

class Barrier_l : public Option 
{
	protected :
		_PnlVect * payoffCoeff;
		_PnlVect * lowerBarrier;
	public:
		double payoff(const PnlMat *path);
		void print();

	// Constructor
	Barrier_l();
	Barrier_l(const Barrier_l& theBarrier_l);
	Barrier_l(_PnlVect * thePayoffCoeff, double maturity, int strike, int timeSteps, int size, _PnlVect * theLowerBarrier);
	Barrier_l(Param& theParam);
	
	// Destructor 
	virtual ~Barrier_l();


	
};
