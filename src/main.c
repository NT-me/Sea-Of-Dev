#include "../include/dependance.h"
#include "../include/graphique.c"
#include "../include/lecture.c"
#include "../include/action.c"

int main(int argc, char *argv[])
{
//Test ligne de commande
  if (argc<2){
    printf("Pas d'argument, merci d'en mettre un.\n");
    return EXIT_SUCCESS;
  }
  if (argc>2){
    printf("Trop d'argument.\n");
  }
//------------------------
// Variables du main
    SDL_Surface *ecran = NULL;
    navalmap_t *carte;
    SHIP *armada;
    fichier temp;
    int idB=0;
    int nbTour=0;
    VAR_JEU PartieCourante;
    int ** nb_scan = NULL;
//------------------------
//Initialisation des bibliothèques
    SDL_Init(SDL_INIT_VIDEO);
    initNavalMapLib();
//------------------------
//Corps du main
    ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);


//Lecture
    temp = read_file(argv[1]); //Lecture du fichier
    carte=init_navalmap(temp.type, temp.size, temp.nbShips);
    carte->initEntityMap(carte);
    armada=init_SHIP(temp.nbShips, temp.KerMax, temp.CoqMax);


//------------------------

placeRemainingShipsAtRandom(carte);

//Dessin du cadrillage
		// PositionX, PostionY, nbre de case de côté, nbre de case de haut, taille des cases de px, taille bordure en px, ecran
    if(carte->type==MAP_RECT){
      draw_cadrillage(20, 20, carte->size.x ,carte->size.y , 44, 2, ecran);
    }


    SDL_WM_SetCaption("Sea of dev - Théo Nardin, Emile Dadou", NULL);
//-----------------------

//Processus de jeu

nb_scan = malloc(sizeof(*nb_scan)*carte->nbShips);

for (int kl = 0; kl < carte->nbShips; kl++){
  nb_scan[kl]=malloc(sizeof(**nb_scan)*2);
}

int retour[1]; // tab de 3 int qui contient le code action, et ses coordonnées
retour[0] = 0;

for(nbTour=0;nbTour<temp.NbTours;++nbTour){
  printf("-------------- | Tour %d | --------------\n", nbTour);


  draw_flotte(carte, ecran, 0);
  SDL_Flip(ecran);
  for(int zimbabwe = 0; zimbabwe<carte->nbShips; ++zimbabwe){
    idB=zimbabwe;
// Initialisation des tubes
      int pipefd[2];  // fd du tube qui renvoie l'action dans le père
      if (pipe(pipefd)==-1){
        printf("pipe failed\n");
        return -1;
      }
//-----------------------
      pid_t a;

      if ((a=fork()) < 0)
        {
          printf("fork failed\n");
          return -1;
        }

      if (a!=0){
        //Processus père
        close(pipefd[1]); // on ferme le canal d'ecriture de pipefd
        read(pipefd[0],retour,sizeof(int) * 3); // on lit le tab retour envoyer a partir du proc fils
        PartieCourante = make_action(carte, armada, idB, retour[0], nb_scan[idB][1], ecran);
        nb_scan = verif_scanner(idB,carte,PartieCourante.code_retour,PartieCourante.ID_Det, nb_scan);

        close(pipefd[0]);
        wait(NULL);
      }
      else{
        // Processus fils
        printf("\n-/--/--/--/--/--/-\n\n");
        close(pipefd[0]);

        retour[0] = decision(nbTour, carte, idB, armada, nb_scan[idB][0], nb_scan[idB][1]); // on met dans la 1re case de retour le code action

        printf("Ker %d\n", armada[idB].KER);
        printf("COQ %d\n\n", armada[idB].COQ);
        //------------------------
        write(pipefd[1], retour, sizeof(int) * 3); // on ecrit dans le pipe le tab retour pour l'envoyer dans le Processus père (serveur)
        close(pipefd[1]);
        exit(-1);
      }
      sleep(2);
      samu(carte, armada, idB);
    }
    draw_flotte(carte, ecran, 1);
}

//------------------------

//Free de tout
    SDL_Quit();
    free_SHIP(armada);
    free_navalmap(carte);
    free(nb_scan);

    return EXIT_SUCCESS;
}
