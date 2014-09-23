#ifndef _OPTION_H
#define _OPTION_H
#include <cstddef>
#include <iostream>

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Classe Option abstraite
class Option
{
protected:
  PnlVect * payOffCoeff_;
  double S_;
  double T_; /// maturit�
  int TimeSteps_; /// nombre de pas de temps de discr�tisation
  int size_; /// dimension du mod�le, redondant avec BS::size_

public:  
  /**
   * Calcule la valeur du payoff sur la trajectoire
   *
   * @param[in] path est une matrice de taille d x (N+1)
   * contenant une trajectoire du mod�le telle que cr��e
   * par la fonction asset.
   * @return phi(trajectoire)
   */
  virtual double payoff(const PnlMat *path) = 0;

  virtual ~Option();

  PnlVect* getPayOffCoeff() const;
  double getStrike() const;
  double getMaturity() const;
  int getTimeSteps () const;
  int getSize () const;
};


#endif /* _OPTION_H */

