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

using namespace std;

int main(int argc, char **argv)
{
  bool couverture = false;
  char * Infile = argv[1];
      if(Infile != NULL && strcmp(Infile,"-c") == 0) {
	cout << "pricer lancé avec option -c" << endl;
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
	while ( nombre<1 || nombre>i-1)
	{
		std::cout << "\n Entrez un nombre valide: ";
		std::cin >> nombre;
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

double date=-1;
while(date<0.0 || date > opt->getMaturity()){
	cout << "\nEn quelle date voulez vous afficher le prix de l'option ? (compris entre 0 et "<< opt->getMaturity()<< ")  : ";   
	cin>>date;	
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

   char  reponse[2];
   bool b = false;
   while (b != true) {
     cout<<"\nVoulez vous afficher le Delta en date "<<date<<" ? (y/n) "<<endl;   
     cin >> reponse;
     if (strcmp(reponse,"y") == 0) {
       cout<<"Calcul du delta en cours"<<endl;
       mc->TestDelta(0);
       b = true;
     } else if  (strcmp(reponse,"n") == 0){
       b = true;
     }
   }
   PnlVect * currentdelta = pnl_vect_new();
   // mc->mod_.simul_market(path, 
   //for (double i = 0; i < mc->H_; i++) {
   //	mc->delta(mc->mod_, i, currentdelta);
	
   //}
   cout << "L'erreur de couverture vaut : ";
   pnl_vect_free(&currentdelta);
 }

  delete(P);
  pnl_rng_free(&rng);
  delete(opt);
  delete(mc);
  exit(0);
}
