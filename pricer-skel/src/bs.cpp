#include "bs.h"

using namespace std;
/// \brief Modèle de Black Scholes

  BS::BS(Param *P) {  
    P->extract("option size", size_);
    P->extract("interest rate", r_);
    P->extract("correlation", rho_);
    P->extract("volatility", sigma_, size_);
    P->extract("spot", spot_, size_);
    P->extract("trend", trend, size_);
    Gamma = pnl_mat_create_from_scalar(size_, size_, rho_);
    pnl_mat_set_diag(Gamma, 1, 0);
    if (pnl_mat_chol(Gamma) == FAIL) {
      cout << "ERREUR: Cholesky  decomposition failed" <<endl;
    }
  }

  int BS::getSize() {
    return size_;
  }

  BS::~BS() {
    pnl_vect_free(&sigma_);
    pnl_vect_free(&spot_);
    pnl_vect_free(&trend);
    pnl_mat_free(&Gamma);
  }

void BS::asset(PnlMat *path, double T, int N, PnlRng *rng, bool market, PnlVect* initVect){
    if (initVect == NULL) {
    pnl_mat_set_row(path, spot_, 0);
    }
  else {
    pnl_mat_set_row(path, initVect, 0);
    }
    if (size_ == 1) {
    for (int ti = 1; ti < N + 1; ti++) {
      double alea = pnl_rng_normal(rng);
      if (market) {
        MLET(path, ti, 0) = MGET(path, ti - 1, 0) * exp((GET(trend, 0) - SQR(GET(sigma_, 0)) / 2) * (T / N) + GET(sigma_, 0) * sqrt(T / N) * alea);
      }
      else {
        MLET(path, ti, 0) = MGET(path, ti - 1, 0) * exp((r_ - SQR(GET(sigma_, 0)) / 2) * (T / N) + GET(sigma_, 0) * sqrt(T / N) * alea);
      }
    }
  }
  else {
    if (rho_ > (-1.0 / (size_ - 1)) && rho_ < 1) {
      for (int ti = 1; ti < N + 1; ti++) {
        PnlVect *alea = pnl_vect_create(size_);
        pnl_vect_rng_normal(alea, size_, rng);
        PnlVect *LG = pnl_mat_mult_vect(Gamma, alea);
        for (int Si = 0; Si < size_; Si++) {
          if (market) {
            MLET(path, ti, Si) = MGET(path, ti - 1, Si) * exp((GET(trend, Si) - SQR(GET(sigma_, Si)) / 2) * (T / N) + GET(sigma_, Si) * sqrt(T / N) * GET(LG, Si));
          }
          else {
            MLET(path, ti, Si) = MGET(path, ti - 1, Si) * exp((r_ - SQR(GET(sigma_, Si)) / 2) * (T / N) + GET(sigma_, Si) * sqrt(T / N) * GET(LG, Si));
          }
        }
        pnl_vect_free(&alea);
        pnl_vect_free(&LG);
      }
    }
    else {
      cout << "ERREUR: rho = " << rho_ << " n'est pas dans l'intervalle voulu ]" << (-1.0 / (size_ - 1)) << ",1[, le marché n'est pas complet" <<endl;
    }
  }
  }

  void BS::asset(PnlMat *path, double t, int N, double T, PnlRng *rng, const PnlMat *past) {
    bool market = false;
    int constaDate = findConstatationDate(T / N, t);
    pnl_mat_set_subblock(path, past, 0, 0);
    PnlMat *sub_path = pnl_mat_create(N - constaDate + 1, size_);
    PnlVect *lastRow = pnl_vect_create(size_);
    pnl_mat_get_row(lastRow, path, past->m - 1);
    asset(sub_path, T * (N - constaDate) / N, N - constaDate, rng, market, lastRow);
    pnl_mat_del_row(sub_path,0);
    pnl_mat_set_subblock(path, sub_path, constaDate + 1, 0);
    pnl_mat_free(&sub_path);
    pnl_vect_free(&lastRow);
  }
  void BS::shift_asset(PnlMat *shift_path, const PnlMat *path,
           int d, double h, double t, int N, double T) {
    int constaDate = findConstatationDate(T / N, t);
    pnl_mat_clone(shift_path, path);
    for (int i = constaDate + 1; i < N + 1; i++) {
    MLET(shift_path, i, 
      d) = (1 + h) * MGET(path, i, d);
    }
  }

  int BS::findConstatationDate(double timestep, double t) {
      return int (t / timestep);
  }
  double BS::getR() {
    return r_;
  }

  PnlVect* BS::GetSpot() {
    return spot_;
  }

void BS::simul_market(PnlMat *path, double T, int H, PnlRng *rng) {
    bool market = true;
    asset(path, T, H, rng, market);
  }
