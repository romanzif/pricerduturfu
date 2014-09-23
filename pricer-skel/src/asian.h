#include "option.h"
#include "parser.h"

class Asian : public Option 
{
	public:
		double payoff(const PnlMat *path);

		// Constructor
		Asian(Param& theParam);
};
