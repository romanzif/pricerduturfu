#include "option.h"
#include "parser.h"

class Barrier_u : public Option 
{
	protected :
		_PnlVect * payoffCoeff;
		_PnlVect * upperBarrier;
	public:
		double payoff(const PnlMat *path);
		void print();

	// Constructor
	Barrier_u();
	Barrier_u(const Barrier_u& theBarrier_u);
	Barrier_u(_PnlVect * thePayoffCoeff, double maturity, int strike, int timeSteps, int size, _PnlVect * theUpperBarrier);
	Barrier_u(Param& theParam);
	
	// Destructor 
	virtual ~Barrier_u();


	
};
