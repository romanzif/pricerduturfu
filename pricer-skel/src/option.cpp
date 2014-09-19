#include "option.h"

  int Option::getStrike() const {
	return strike;
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

  void Option::set () {}
  void Option::set (const int strk, const double T, const int Time, const int taille) {
	strike = strk;
	T_ = T;
	TimeSteps_ = Time;
	size_ = taille;
  }
