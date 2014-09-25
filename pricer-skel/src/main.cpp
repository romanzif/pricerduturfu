#include <iostream>
#include "parser.h"
#include "bs.h"
#include "pnl/pnl_random.h"
#include "mc.h"
#include "option.h"
#include "asian.h"
#include "basket.h"
#include "barrier_l.h"
#include "barrier_u.h"
#include "barrier.h"
#include "performance.h"
#include <cstddef>
#include <stdlib.h> 
#include <stdio.h> 
#include <dirent.h> 
#include <sys/types.h>
#include <cstdlib>
#include <limits>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
  bool couverture = false;
  char * Infile = argv[1];
  if(Infile != NULL && strcmp(Infile,"-c") == 0) {
    cout << "Pricer lancé avec option -c" << endl;
    couverture = true;
    Infile = argv[2];
  } else {
    cout << "Pricer lancé sans l'option -c \n" << endl;
    cout << "Aucun fichier source en entrée, veuillez choisir celui que vous voulez" << endl;
  }
      
  if (Infile == NULL || strcmp(Infile,"")==0) {
    std::cout << "\n              Liste des options disponibles : \n" << std::endl;
    DIR* rep = NULL;
    struct dirent* fichierLu = NULL; /* Déclaration d'un pointeur vers la structure dirent. */
    rep = opendir("../data");
    fichierLu = readdir(rep);
    fichierLu = readdir(rep);
    int i=1;
    while ((fichierLu = readdir(rep)) != NULL)
      {
	printf("%i : '%s' \n", i, fichierLu->d_name); 
	i++;
      }
	
    int nombre=0;
    bool stay_in = true;
    std::cout << "\n Entrez un nombre valide: ";
    std::cin >> nombre;
    while(std::cin.fail() || stay_in)
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        if (!(nombre<1 || nombre>i-1)) {
          stay_in = false;
        } else {
          std::cout << "\n Entrez un nombre valide: ";
          std::cin >> nombre;
        }
                
      }

    closedir(rep);
    rep = opendir("../data");
    fichierLu = readdir(rep);
    fichierLu = readdir(rep);
	
    i=0;
    while (i<nombre)
      {
	(fichierLu = readdir(rep));
	i++;
      }
    printf("\n              Chargement de l'option : %s \n \n", fichierLu->d_name);
    char repertoire []="../data/";
    Infile = strcat(repertoire,fichierLu->d_name);
    closedir(rep);
  } 
  Param *P = new Parser(Infile);

  PnlRng * rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng,0);
  char* typeOption;
  P->extract("option type", typeOption);
  Option *opt;

  if (strcmp(typeOption,"asian")==0)
    {
	
      opt = new Asian(*P);
      cout<<"Option type : Asian"<<endl;
    }
  else if (strcmp(typeOption,"basket")==0)
          {
            cout<<"Option type : Basket"<<endl;
		
            opt = new Basket(*P);
          }
        else if (strcmp(typeOption,"barrier_l")==0)
              {
               cout<<"Option type : Lower barrier"<<endl;
                opt = new Barrier_l(*P);
              }
        else if (strcmp(typeOption,"barrier_u")==0)
              {
                cout<<"Option type : Upper barrier"<<endl;
                opt = new Barrier_u(*P);
              }
          else if (strcmp(typeOption,"barrier")==0)
              {
                  cout<<"Option type : Barrier"<<endl;
                 opt = new Barrier(*P);
                }
                else if (strcmp(typeOption,"performance")==0)
                      {
                        cout<<"Option type : Performance"<<endl;
                        opt = new Performance(*P);
                      } 
                      else 
                      {cout<<"Type Non reconnu";
                      exit(1);
                      }
	
         
         printf("\n              Execution de l'algorithme de Monte Carlo ... \n \n");   

MonteCarlo *mc = new MonteCarlo(P,opt,rng);
double prix=0;
double ic =0;

mc->price(prix,ic);

printf("\n    Calcul du prix ... \n");
cout<<"Le prix vaut : "<<prix<<endl;
cout<<"L'intervalle de confiance est : ["<<prix - ic<< "," << prix + ic << "]"<<endl;

 if(couverture ==true){
    printf("\n --------Erreur de couverture--------- \n");
    if (mc->getH() == 0) {
      cout << "Pas de hedging date spécifiée dans le fichier en entrée, impossible de continuer" <<endl;
    } else if (mc->getH() % mc->getN() != 0) {
      cout << "Les dates de constatation ne coincident pas avec les dates de rebalancement, impossible de continuer" <<endl;
    } else if (mc->getTrend() == NULL) {
      cout << "Pas de trend spécidié dans le fichier en entrée, impossible de continuer" <<endl;
    } else {
      cout<<"Simulation du marché en cours"<<endl;
      PnlMat* marketmat = pnl_mat_create_from_zero(mc->getT()*mc->getH()+1,mc->getD());
      mc->simul_market(marketmat);
      
      cout<<"\nCalcul des delta en cours"<<endl;
      PnlMat* MatDelta = pnl_mat_create(marketmat->m,mc->getD());
      for (int i =0;i < marketmat->m ;i++) {
        PnlVect *deltavect= pnl_vect_create(mc->getD()); 
        PnlMat * currentMat = mc->extract(marketmat, i);
        mc->delta(currentMat, i/mc->getH(), deltavect);
        pnl_mat_set_row(MatDelta,deltavect,i);
        pnl_vect_free(&deltavect);
        pnl_mat_free(&currentMat);
      }
      cout<<"\nCalcul du PnL"<<endl;
      double PnL = 0;
      PnlMat* my_mat = mc->extract(marketmat, marketmat->m-1);
      mc->ProfitAndLoss(PnL,MatDelta,prix, my_mat, marketmat);
      cout << "L'erreur de couverture vaut : " << PnL << endl;
      pnl_mat_free(&marketmat);
      pnl_mat_free(&MatDelta);
      pnl_mat_free(&my_mat);
    }
    
  }

  delete(P);
  pnl_rng_free(&rng);
  delete(opt);
  delete(mc);
  exit(0);
}
