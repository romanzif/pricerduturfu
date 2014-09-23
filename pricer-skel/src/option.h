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
  double T_; /// maturité
  int TimeSteps_; /// nombre de pas de temps de discrétisation
  int size_; /// dimension du modèle, redondant avec BS::size_

public:  
  /**
   * Calcule la valeur du payoff sur la trajectoire
   *
   * @param[in] path est une matrice de taille d x (N+1)
   * contenant une trajectoire du modèle telle que créée
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

