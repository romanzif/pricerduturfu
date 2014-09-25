#include "mc.h"

using namespace std;

	MonteCarlo::MonteCarlo(Param *P, Option *opt, PnlRng *rng, double H)
  {
  	mod_ = new BS(P);
  	opt_= opt;
	P->extract("fd step",h_);
	P->extract("hedging date number",H_);
  	P->extract("sample number", samples_);
    rng_ = rng;
    H_ = 2;


    MonteCarlo::~MonteCarlo() {
    delete(mod_);
  }


  PnlMat * MonteCarlo::extract(const PnlMat *pastIn, int indice) {
  int N = opt_->getTimeSteps();
  int D =opt_->getSize();
  int RebalancementNumber = H_/N;
  int taille = indice / RebalancementNumber + 1;
  PnlMat *pastOut = pnl_mat_create(taille, D);
  for (int i=0;i<taille; i++) {
    for (int j=0; j<D; j++) {
      cout<<"i = "<<i<<"et j =  "<<j<<endl;
      MLET(pastOut, i, j) = MGET(pastIn, i * RebalancementNumber, j);
    }  
  }
  return pastOut;
  }  
  
  void MonteCarlo::price(double &prix, double &ic)
  {
    //double ListPriceSimulation[samples_];
      PnlMat *mat= pnl_mat_create(this->opt_->getTimeSteps()*this->opt_->getMaturity()+1,this->opt_->getSize());
       //double MatPnl[this->opt_->size_][ this->opt_->TimeSteps_];
    prix = 0;
    double xhi =0;
    double sumSquare = 0;
    for (int i =0; i<samples_;i++)
    {
      mod_->asset(mat, opt_->getMaturity(), opt_->getTimeSteps()*opt_->getMaturity(),rng_, 0);
      //mod_->asset(mat, 1, 12, rng);
     // cout<<"le payoff(mat) vaut "<< opt_->payoff(mat)<<endl;
      sumSquare += opt_->payoff(mat)*opt_->payoff(mat);
      prix += this->opt_->payoff(mat);

	   }

	sumSquare /=samples_;
	prix /= samples_;
    xhi  = sqrt(exp(2*(-mod_->getR()*opt_->getMaturity()))*(sumSquare-prix*prix));
    ic = 1.96*xhi/(sqrt(samples_));
	prix *= exp(-mod_->getR()*opt_->getMaturity());
  pnl_mat_free(&mat);
  }

  void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic)
  {
    PnlMat *mat= pnl_mat_create(this->opt_->getTimeSteps()*this->opt_->getMaturity()+1,this->opt_->getSize());
     prix = 0;
    double xhi =0;
    double sumSquare = 0;
    for (int i =0; i<samples_;i++)
    {
      mod_->asset(mat, t, opt_->getTimeSteps() * opt_->getMaturity(), opt_->getMaturity(),rng_,past);
     // cout<<"le payoff(mat) vaut "<< opt_->payoff(mat)<<endl;
      sumSquare += opt_->payoff(mat)*opt_->payoff(mat);
      prix += this->opt_->payoff(mat);
     }
     sumSquare /=samples_;
  prix /= samples_;
    xhi  = sqrt(exp(2*(-mod_->getR()*opt_->getMaturity()))*(sumSquare-prix*prix));
    ic = 1.96*xhi/(sqrt(samples_));
  prix *= exp(-mod_->getR()*opt_->getMaturity());
  pnl_mat_free(&mat);
  }

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta) {
  // initialisation des données (calcul g+ et g- ainsi que S~ avec asset)
  double T =  opt_->getMaturity();
  int D = mod_->getSize();
  int N = opt_->getTimeSteps() * T;
  int M = samples_;
  double r = mod_->getR();
  PnlVect* st = pnl_vect_create(D);
  pnl_mat_get_row(st, past, past->m - 1);
  PnlMat* simul_mat = pnl_mat_create(N+1,D);
  PnlMat* MatGplus = pnl_mat_create(N+1,D);
  PnlMat* MatGminus = pnl_mat_create(N+1,D);
  for (int d = 0; d < D; d++) {
    double frontCoef = exp(-r*(T-t))/(M*2*h_*GET(st,d));
    double sum = 0;
    for (int j=1; j < M+1; j++) {
      if (t==0) {
	mod_->asset(simul_mat,T,N,rng_,false);
      }
      else {
	mod_->asset(simul_mat,t,N,T,rng_,past);
      }
      mod_->shift_asset(MatGplus,simul_mat,d, h_,t,N,T);
      mod_->shift_asset(MatGminus,simul_mat,d, -h_,t,N,T);
      sum += opt_->payoff(MatGplus) - opt_->payoff(MatGminus);
    }
    LET(delta,d) = frontCoef * sum;
  }
  pnl_mat_free(&MatGplus);
  pnl_mat_free(&MatGminus);
  pnl_mat_free(&simul_mat);
  pnl_vect_free(&st);
}



void MonteCarlo::TestDelta(double t) 
{
 PnlVect *deltavect= pnl_vect_create(opt_->getSize()); 
 PnlVect *ic;
 PnlMat *past = pnl_mat_create_from_zero(1,opt_->getSize());
 PnlVect * spot;;
 for (int i=0;i<this->opt_->getSize();i++)
  {
    MLET(past,0,i) = GET(mod_->GetSpot(),i);
  } 
  delta(past, t, deltavect);
  pnl_vect_print(deltavect);
  pnl_vect_free(&deltavect);
  pnl_mat_free(&past);
}


void MonteCarlo::ProfitAndLoss(double &PL, const PnlMat *delta, double price0, double payoff, PnlMat *past) {
  double r = mod_->getR();
  double T =  opt_->getMaturity();
    int D =opt_->getSize();
    int H = H_ * T;
  //PnlMat *S = pnl_mat_create(H + 1, D);

  PnlVect *Si = pnl_vect_create(D);
  PnlVect *deltai = pnl_vect_create(D);
  //mod_->simul_market(S, T, H, rng_);
  pnl_mat_get_row(Si, past, 0);
  pnl_mat_get_row(deltai, delta, 0);
  double V = price0 - pnl_vect_scalar_prod(deltai, Si);
  for (int i = 1; i < H + 1; i++) {
    for (int j = 0; j < D + 1; j++) {
      LET(deltai, j) = MGET(delta, i, j) - MGET(delta, i - 1, j);
    }
    V = V * exp(r * T / H) - pnl_vect_scalar_prod(deltai, Si);
  }
  pnl_mat_get_row(Si, past, H);
  pnl_mat_get_row(deltai, delta, H);
  PL = V + pnl_vect_scalar_prod(deltai, Si) - payoff;
   pnl_vect_free(&Si);
pnl_vect_free(&deltai);    
}


void MonteCarlo::TestPnl () {
double T = opt_->getMaturity();
double D = opt_->getSize();
PnlMat *marketmat = pnl_mat_create(T*H_+1,D);


mod_->simul_market(marketmat,T,T*H_,rng_);
pnl_mat_print(marketmat);

//PnlMat *extractmat =pnl_mat_copy( extract(marketmat, T*H_-1));

for (int i =0;i<T*H_+1;i++) {
pnl_mat_print(extract(marketmat, i));
 }
}
