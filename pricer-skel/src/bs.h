#ifndef _BS_H
#define _BS_H

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Mod�le de Black Scholes
class BS
{
public:
  int size_; /// nombre d'actifs du mod�le
  double r_; /// taux d'int�r�t
  double rho_; /// param�tre de corr�lation
  PnlVect *sigma_; /// vecteur de volatilit�s
  PnlVect *spot_; /// valeurs initiales du sous-jacent

  /**
   * G�n�re une trajectoire du mod�le et la stocke dans path
   *
   * @param[out] path contient une trajectoire du mod�le.
   * C'est une matrice de taille d x (N+1)
   * @param[in] T  maturit�
   * @param[in] N nombre de dates de constatation
   */
  void asset(PnlMat *path, double T, int N, PnlRng *rng);

  /**
   * Calcule une trajectoire du sous-jacent connaissant le
   * pass� jusqu' � la date t
   *
   * @param[out] path  contient une trajectoire du sous-jacent
   * donn�e jusqu'� l'instant t par la matrice past
   * @param[in] t date jusqu'� laquelle on connait la trajectoire
   * t n'est pas forc�ment une date de discr�tisation
   * @param[in] N nombre de pas de constatation
   * @param[in] T date jusqu'� laquelle on simule la trajectoire
   * @param[in] past trajectoire r�alis�e jusqu'a la date t
   */
  void asset(PnlMat *path, double t, int N, double T,
             PnlRng *rng, const PnlMat *past);
  /**
   * Calcule le delta de l'option � la date t
   *
   * @param[in] past contient la trajectoire du sous-jacent
   * jusqu'� l'instant t
   * @param[in] t date � laquelle le calcul est fait
   * @param[out] delta contient le vecteur de delta
   * @param[out] ic contient la largeur de l'intervalle
   * de confiance sur le calcul du delta
   */
  void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic);

  /**
   * Shift d'une trajectoire du sous-jacent
   *
   * @param[in]  path contient en input la trajectoire
   * du sous-jacent
   * @param[out] shift_path contient la trajectoire path
   * dont la composante d a �t� shift�e par (1+h)
   * � partir de la date t.
   * @param[in] t date � partir de laquelle on shift
   * @param[in] h pas de diff�rences finies
   * @param[in] d indice du sous-jacent � shifter
   * @param[in] timestep pas de constatation du sous-jacent
   */
  void shift_asset(PnlMat *shift_path, const PnlMat *path,
                   int d, double h, double t, double timestep);

};


#endif /* _BS_H */

