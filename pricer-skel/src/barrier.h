#include "option.h"
#include "parser.h"

class Barrier : public Option 
{
	protected :
		PnlVect * upperBarrier_;
		PnlVect * lowerBarrier_;
	public:
		double payoff(const PnlMat *path);

	// Constructor
	Barrier(Param& theParam);
	
	// Destructor 
	virtual ~Barrier();


	
};
