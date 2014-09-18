#ifndef _OPTION_H
#define _OPTION_H

//#include "pnl/pnl_vector.h"
//#include "pnl/pnl_matrix.h"

/// \brief Classe Option abstraite
class Option
{
public:
  int strike;
  double T_; /// maturit�
  int TimeSteps_; /// nombre de pas de temps de discr�tisation
  int size_; /// dimension du mod�le, redondant avec BS::size_
  /**
   * Calcule la valeur du payoff sur la trajectoire
   *
   * @param[in] path est une matrice de taille d x (N+1)
   * contenant une trajectoire du mod�le telle que cr��e
   * par la fonction asset.
   * @return phi(trajectoire)
   */
  int getStrike() {
	return strike;
  }

  double getMaturity () {
	return T_;
  }

  int getTimeSteps () {
	return TimeSteps;
  }

  int getSize () {
	return size_;
  }

  void set ();
  void set (int strk, double T, int Time, int taille);
  //virtual double payoff(const PnlMat *path) = 0;
};


#endif /* _OPTION_H */

