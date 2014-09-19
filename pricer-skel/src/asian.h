#include "option.h"
#include "parser.h"

class Asian : public Option 
{
	protected :
		_PnlVect * payoffCoeff;
	public:
		double payoff(const PnlMat *path);
		void print();

	// Constructor
	Asian();
	Asian(const Asian& theAsian);
	Asian(_PnlVect * thePayoffCoeff, double maturity, int strike, int timeSteps, int size);
	Asian(Param& theParam);
	
	// Destructor 
	virtual ~Asian();


	
};
