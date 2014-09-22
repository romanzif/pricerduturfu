#include "option.h"
#include "parser.h"

class Performance : public Option 
{
	protected :
		_PnlVect * payoffCoeff;
	public:
		double payoff(const PnlMat *path);
		void print();

	// Constructor
	Performance();
	Performance(const Performance& thePerformance);
	Performance(_PnlVect * thePayoffCoeff, double maturity, int strike, int timeSteps, int size);
	Performance(Param& theParam);
	
	// Destructor 
	virtual ~Performance();


	
};
