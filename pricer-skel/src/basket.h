#include "option.h"
#include "parser.h"

class Basket : public Option 
{
	
	public:
		double payoff(const PnlMat *path);

		Basket(Param& theParam);

		virtual ~Basket();
};
