#include "option.h"
#include "parser.h"

class Barrier : public Option 
{
	protected :
		_PnlVect * payoffCoeff;
		_PnlVect * upperBarrier;
		_PnlVect * lowerBarrier;
	public:
		double payoff(const PnlMat *path);
		void print();

	// Constructor
	Barrier();
	Barrier(const Barrier& theBarrier);
	Barrier(_PnlVect * thePayoffCoeff, double maturity, int strike, int timeSteps, int size, _PnlVect * theLowerBarrier, _PnlVect * theUpperBarrier);
	Barrier(Param& theParam);
	
	// Destructor 
	virtual ~Barrier();


	
};
