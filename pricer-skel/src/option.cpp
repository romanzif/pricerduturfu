#include "option.h"


  PnlVect* Option::getPayOffCoeff() const {
    return payOffCoeff_;
  }

  double Option::getStrike() const {
	  return S_;
  }

  double Option::getMaturity() const{
	return T_;
  }

  int Option::getTimeSteps () const {
	return TimeSteps_;
  }

  int Option::getSize () const {
	return size_;
  }

  Option::~Option() { pnl_vect_free(&payOffCoeff_); };
