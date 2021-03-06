#ifndef _MC_H
#define _MC_H

#include "option.h"
#include "bs.h"
#include "pnl/pnl_random.h"
#include "parser.h"

class MonteCarlo
{
public:
  BS *mod_; /*! pointeur vers le mod�le */
  Option *opt_; /*! pointeur sur l'option */
  double h_; /*! pas de diff�rence finie */
  int samples_; /*! nombre de tirages Monte Carlo */
  PnlRng *rng_;
  int H_;

    /*constructeur del fuego*/
  MonteCarlo(Param *P, Option *opt, PnlRng *rng);  
    ~MonteCarlo();
  /**
   * Calcule le prix de l'option � la date 0
   *
   * @param[out] prix valeur de l'estimateur Monte Carlo
   * @param[out] ic largeur de l'intervalle de confiance
   */
  void price(double &prix, double &ic);

  /**
   * Calcule le prix de l'option � la date t
   *
   * @param[in]  past contient la trajectoire du sous-jacent
   * jusqu'� l'instant t
   * @param[in] t date � laquelle le calcul est fait
   * @param[out] prix contient le prix
   * @param[out] ic contient la largeur de l'intervalle
   * de confiance sur le calcul du prix
   */
  void price(const PnlMat *past, double t, double &prix, double &ic);

  void delta(const PnlMat *past, double t, PnlVect *delta);

  void TestDelta(double t) ;

  PnlMat * extract(const PnlMat *pastIn, int indice);

  void fillMatPast(PnlMat *past, double t);

  void ProfitAndLoss(double &PL, const PnlMat *delta, double price0, const PnlMat *pastN, const PnlMat *past);

  void simul_market(PnlMat* marketmat);


  int getH();
  double getT();
  int getD();
  int getN();
  PnlVect* getTrend();
};

#endif /* _MC_H */

