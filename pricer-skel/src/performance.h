#include "option.h"
#include "parser.h"

class Performance : public Option 
{
	public:
		double payoff(const PnlMat *path);

	// Constructor
	Performance(Param& theParam);


	
};
