#include "option.h"
#include "parser.h"

class Barrier_u : public Option 
{
	protected :
		PnlVect * upperBarrier_;
	public:
		double payoff(const PnlMat *path);

	// Constructor
	Barrier_u(Param& theParam);
	
	// Destructor 
	virtual ~Barrier_u();


	
};
