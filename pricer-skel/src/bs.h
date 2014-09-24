#pragma once
#include <iostream>
#include "parser.h"
#include <cmath>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_mathtools.h"
#include "pnl/pnl_random.h"

/// \brief Modèle de Black Scholes
class BS
{

private:

  int size_; /// nombre d'actifs du modèle
  double r_; /// taux d'intérêt sans risque
  double rho_; /// paramètre de corrélation entre les actifs
  PnlVect *sigma_; /// vecteur de volatilités des actifs
  PnlVect *spot_; /// vecteur de valeurs initiales du sous-jacent
  PnlVect *trend; /// vecteur de rendements espérés
  PnlMat *Gamma; // matrice de correllation

public:

  BS(Param *P);

  ~BS();

  int getSize();

  /**
   * Génère une trajectoire du modèle
   *
   * @param[out] path contient la trajectoire du modèle (matrice de taille (N + 1) x D)
   * @param[in] T maturité
   * @param[in] N nombre de dates de constatation
   * @param[in] rng générateur de nombres aléatoires (ici, des lois normales centrées réduites)
   * @param[in] initVect vecteur d'initialisation du sous jacent en t > 0 (inexistant en t = 0 car on initialise avec spot_)
   * @param[in] market indique si on simule sous la probabilité historique (vrai) ou risque-neutre (faux)
   */
  void asset(PnlMat *path, double T, int N, PnlRng *rng, bool market, PnlVect *initVect = NULL);

  /**
   * Calcule une trajectoire du sous-jacent connaissant le passé jusqu'à la date t
   *
   * @param[out] path contient une trajectoire du sous-jacent donnée jusqu'à l'instant t par la matrice past
   * @param[in] t date jusqu'à laquelle on connaît la trajectoire (t n'est pas forcément une date de discrétisation)
   * @param[in] N nombre de pas de constatation
   * @param[in] T date jusqu'à laquelle on simule la trajectoire
   * @param[in] rng générateur de nombres aléatoires (ici, des lois normales centrées réduites)
   * @param[in] past contient en input la trajectoire réalisée jusqu'a la date t
   */
  void asset(PnlMat *path, double t, int N, double T,
             PnlRng *rng, const PnlMat *past);
  /**
   * Calcule le delta de l'option à la date t
   *
   * @param[in] past contient en input la trajectoire du sous-jacent jusqu'à l'instant t
   * @param[in] t date à laquelle le calcul est fait
   * @param[out] delta contient en output le vecteur de delta
   * @param[out] ic contient en output la largeur de l'intervalle de confiance sur le calcul du delta
   */
  /// void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic);

  /**
   * Shift d'une trajectoire du sous-jacent
   *
   * @param[in] path contient en input la trajectoire du sous-jacent
   * @param[out] shift_path contient en output la trajectoire path dont la composante d a été shiftée par (1 + h) à partir de la date t
   * @param[in] t date à partir de laquelle on shift
   * @param[in] h pas de différences finies
   * @param[in] d indice du sous-jacent à shifter
   * @param[in] timestep pas de constatation du sous-jacent
   */
  void shift_asset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, int N, double T);

  /**
  * Détermine la date de constatation juste avant la date t
  *
  * @param[in] timestep pas de temps de discrétisation
  * @param[in] t date à laquelle le calcul est fait
  */
  int findConstatationDate(double timestep, double t);

  /**
  * Génère une simulation du marché (modèle sous la probabilité historique)
  *
  * @param[out] path contient en output la trajectoire du marché (matrice de taille (N + 1) x D)
  * @param[in] T maturité
  * @param[in] H nombre de jours ou de semaine jusqu'à maturité
  * @param[in] rng générateur de nombres aléatoires (ici, des lois normales centrées réduites)
  */
  void simul_market(PnlMat *path, double T, int H, PnlRng *rng);

  double getR();

};
