#include "option.h"
#include "parser.h"

class Barrier_l : public Option 
{
	protected :
		PnlVect * lowerBarrier_;
	public:
		double payoff(const PnlMat *path);

	// Constructor
	Barrier_l(Param& theParam);
	
	// Destructor 
	virtual ~Barrier_l();
};
