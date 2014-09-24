#pragma once
#include <iostream>
#include "parser.h"
#include <cmath>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_mathtools.h"
#include "pnl/pnl_random.h"

/// \brief Mod�le de Black Scholes
class BS
{

private:

  int size_; /// nombre d'actifs du mod�le
  double r_; /// taux d'int�r�t sans risque
  double rho_; /// param�tre de corr�lation entre les actifs
  PnlVect *sigma_; /// vecteur de volatilit�s des actifs
  PnlVect *spot_; /// vecteur de valeurs initiales du sous-jacent
  PnlVect *trend; /// vecteur de rendements esp�r�s
  PnlMat *Gamma; // matrice de correllation

public:

  BS(Param *P);

  ~BS();

  int getSize();

  /**
   * G�n�re une trajectoire du mod�le
   *
   * @param[out] path contient la trajectoire du mod�le (matrice de taille (N + 1) x D)
   * @param[in] T maturit�
   * @param[in] N nombre de dates de constatation
   * @param[in] rng g�n�rateur de nombres al�atoires (ici, des lois normales centr�es r�duites)
   * @param[in] initVect vecteur d'initialisation du sous jacent en t > 0 (inexistant en t = 0 car on initialise avec spot_)
   * @param[in] market indique si on simule sous la probabilit� historique (vrai) ou risque-neutre (faux)
   */
  void asset(PnlMat *path, double T, int N, PnlRng *rng, bool market, PnlVect *initVect = NULL);

  /**
   * Calcule une trajectoire du sous-jacent connaissant le pass� jusqu'� la date t
   *
   * @param[out] path contient une trajectoire du sous-jacent donn�e jusqu'� l'instant t par la matrice past
   * @param[in] t date jusqu'� laquelle on conna�t la trajectoire (t n'est pas forc�ment une date de discr�tisation)
   * @param[in] N nombre de pas de constatation
   * @param[in] T date jusqu'� laquelle on simule la trajectoire
   * @param[in] rng g�n�rateur de nombres al�atoires (ici, des lois normales centr�es r�duites)
   * @param[in] past contient en input la trajectoire r�alis�e jusqu'a la date t
   */
  void asset(PnlMat *path, double t, int N, double T,
             PnlRng *rng, const PnlMat *past);
  /**
   * Calcule le delta de l'option � la date t
   *
   * @param[in] past contient en input la trajectoire du sous-jacent jusqu'� l'instant t
   * @param[in] t date � laquelle le calcul est fait
   * @param[out] delta contient en output le vecteur de delta
   * @param[out] ic contient en output la largeur de l'intervalle de confiance sur le calcul du delta
   */
  /// void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic);

  /**
   * Shift d'une trajectoire du sous-jacent
   *
   * @param[in] path contient en input la trajectoire du sous-jacent
   * @param[out] shift_path contient en output la trajectoire path dont la composante d a �t� shift�e par (1 + h) � partir de la date t
   * @param[in] t date � partir de laquelle on shift
   * @param[in] h pas de diff�rences finies
   * @param[in] d indice du sous-jacent � shifter
   * @param[in] timestep pas de constatation du sous-jacent
   */
  void shift_asset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, int N, double T);

  /**
  * D�termine la date de constatation juste avant la date t
  *
  * @param[in] timestep pas de temps de discr�tisation
  * @param[in] t date � laquelle le calcul est fait
  */
  int findConstatationDate(double timestep, double t);

  /**
  * G�n�re une simulation du march� (mod�le sous la probabilit� historique)
  *
  * @param[out] path contient en output la trajectoire du march� (matrice de taille (N + 1) x D)
  * @param[in] T maturit�
  * @param[in] H nombre de jours ou de semaine jusqu'� maturit�
  * @param[in] rng g�n�rateur de nombres al�atoires (ici, des lois normales centr�es r�duites)
  */
  void simul_market(PnlMat *path, double T, int H, PnlRng *rng);

  double getR();

};
