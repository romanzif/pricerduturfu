#ifndef _OPTION_H
#define _OPTION_H
#include <cstddef>
#include <iostream>

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Classe Option abstraite
class Option
{
public:
  int strike;
  double T_; /// maturité
  int TimeSteps_; /// nombre de pas de temps de discrétisation
  int size_; /// dimension du modèle, redondant avec BS::size_
  /**
   * Calcule la valeur du payoff sur la trajectoire
   *
   * @param[in] path est une matrice de taille d x (N+1)
   * contenant une trajectoire du modèle telle que créée
   * par la fonction asset.
   * @return phi(trajectoire)
   */
  int getStrike() const;

  double getMaturity() const;

  int getTimeSteps () const;

  int getSize () const;

  void set ();
  void set (const int strk, const double T, const int Time, const int taille);
  virtual double payoff(const PnlMat *path) = 0;
};


#endif /* _OPTION_H */

