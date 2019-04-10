#include "../include/dependance.h"

//------Struct----

//Type fichier
typedef struct{
  map_t type; //Type de la map_t
  coord_t size; //Taille de la map
  int nbShips; //Nom de navire
  int CoqMax; //Coque maximum
  int KerMax; //Kerosen maximum
  int NbTours; //Nombre de tours
} fichier;

// Type Ship
typedef struct {
	entityid_t ShipID;  // Contient le type de l'entité ENT_SHIP et son Identifiant
	//coord_t pos; // position du ship
	int KER; // Var contenant le niveaux de kérosene du ship
	int COQ; // Var contenant le niveaux de coque (vie) du ship
} SHIP;

//-----------------

void pauseStop()
{
    int continuer = 1;
    SDL_Event event;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}

int draw_Case(int posX, int posY, int taille, int bordure, SDL_Surface *ecran){
	if (bordure<2){
		printf("Pas en dessous de 2 la bordure");
		return EXIT_FAILURE;
	}
	SDL_Surface *c = NULL;
	SDL_Surface *c2 = NULL;
	SDL_Rect position;
	c = SDL_CreateRGBSurface(SDL_HWSURFACE, taille, taille, 32, 0, 0, 0, 0);
	c2 = SDL_CreateRGBSurface(SDL_HWSURFACE, taille-bordure, taille-bordure, 32, 0, 0, 0, 0);


	// Remplissage de la surface avec du blanc
	SDL_FillRect(c, NULL, SDL_MapRGB(c->format, 255, 255, 255));
	SDL_FillRect(c2, NULL, SDL_MapRGB(c->format, 0, 0, 0));

	position.x = posX;
	position.y = posY;
	SDL_BlitSurface(c, NULL, ecran, &position); // Collage de la surface sur l'écran

	position.x = posX+bordure/2;
	position.y = posY+bordure/2;
	SDL_BlitSurface(c2, NULL, ecran, &position); // Collage de la surface sur l'écran

	 SDL_FreeSurface(c);
	 SDL_FreeSurface(c2);

   return EXIT_SUCCESS;

}

void draw_cadrillage(int posX, int posY, int largeur, int hauteur, int tailleC, int bordureC, SDL_Surface *ecran){
	int i, j, posXC, posYC;
	for(i=0;i<largeur; ++i){
		for(j=0; j<hauteur; ++j){
			posXC=(tailleC*i)+posX;
			posYC=(tailleC*j)+posY;
			draw_Case(posXC, posYC, tailleC, bordureC, ecran);
		}
	}
}

coord_t repere_cadrillage(int coinX, int coinY,int tailleC, int PosX, int PosY){
  coord_t res;
  res.x=coinX+(tailleC/2)+(tailleC*PosX);
  res.y=coinY+(tailleC/2)+(tailleC*PosY);
  return res;
}

void draw_ship(navalmap_t *nm, SDL_Surface *ecran, int Noir, int IDnavire){
  SDL_Surface *c = NULL;
  coord_t pos;
  SDL_Rect positionR;
  c = SDL_CreateRGBSurface(SDL_HWSURFACE, 5, 7, 32, 0, 0, 0, 0);

  if(Noir==0){
    SDL_FillRect(c, NULL, SDL_MapRGB(c->format, 0, 0, 255));
  }
  else{
    SDL_FillRect(c, NULL, SDL_MapRGB(c->format, 0, 0, 0));
  }
  pos=repere_cadrillage(20, 20, 44, nm->shipPosition[IDnavire].x, nm->shipPosition[IDnavire].y);
  /*position.x = nm->shipPosition[IDnavire].x;
  position.y = nm->shipPosition[IDnavire].y;*/

  positionR.x=pos.x; positionR.y=pos.y;

  SDL_BlitSurface(c, NULL, ecran, &positionR); // Collage de la surface sur l'écran

}

void draw_flotte(navalmap_t *nm, SDL_Surface *ecran, int Noir){
  int i;
  for(i=0;i<nm->nbShips;++i){
    draw_ship(nm, ecran, 0, i);
  }
}

long weight_file(char *nomF){
  long taille;
  struct stat st;
  stat(nomF, &st);
  taille = st.st_size;
  return taille;
}

// Fonction servant à traiter les caractères entre PosChDeb et PosChFin d'un fichier nomF
char* ChToCh(char* fic, int PosChDeb, int PosChFin){
  //Ouverture
  int fd;
  char buf, *r;
  r=malloc(sizeof(char)*PosChFin);
  fd=open(fic, O_RDONLY);

  for(int l=0;l<PosChFin;++l){
    read(fd, &buf, sizeof(char));
    if(l>=PosChDeb){
     r[l-PosChDeb]=buf;
    }
  }
  close(fd);
  return r;
  // Attention fuite de mémoire ici (le *r)
}
//TypeCartes (0->dpa-1); TailleX(dpa->dpb-1); TailleY(dpb->dpc-1)
//rectangle ;  1  0  ;  1  0  \n
//123456789 10 11 12 13 14 15 16

//nbJoueurs (dpc->dpd-1);Cmax(dpd->dpe-1);Kmax(dpe->dpf-1);nbTours(dpf->E0F)
//2  ;  1  0  0  ;  1  0  0  ;  2  0  EOF
//17 18 19 20 21 22 23 24 25 26 27 28 29

fichier read_file(char* nomF){
  fichier res;
  char buf1, *point=NULL;
  int fd, count=0, dpa=0, dpb=0, dpc=0, dpd=0, dpe=0, dpf=0;
  fd=open(nomF, O_RDONLY);

  for(int l=0;l<weight_file(nomF);++l){
    read(fd, &buf1, sizeof(char));
    if(buf1==';' || buf1=='\n'){
      count++;
      //printf("buf: %c\n count: %d\n l: %d\n", buf1,count, l);
        if(count==1){
          dpa=l+1;
        }
        if(count==2){
          dpb=l+1;
        }
        if(count==3){
          dpc=l+1;
        }
        if(count==4){
          dpd=l+1;
        }
        if(count==5){
          dpe=l+1;
        }
        if(count==6){
          dpf=l+1;
        }
      }
    }
  close(fd);

  // On récupère le type de la map
  point = ChToCh(nomF, 0,dpa-1);
  if (strcmp(point, "rectangle")==0){
    res.type=MAP_RECT;
  }
  else{
    res.type=MAP_TOTAL;
  }
  free(point);

  //On récupère la taille x de la carte
  point= ChToCh(nomF, dpa, dpb-1);
  res.size.x = atoi(point);
  free(point);

  //On récupère la taille y de la carte
  point= ChToCh(nomF, dpb, dpc-1);
  res.size.y = atoi(point);
  free(point);

  //On récupère le nombre de navire
  point= ChToCh(nomF, dpc, dpd-1);
  res.nbShips = atoi(point);
  free(point);

  //On récupère la coque max
  point=ChToCh(nomF, dpd, dpe-1);
  res.CoqMax = atoi(point);
  free(point);

  //On récupère le Kerosen max
  point=ChToCh(nomF, dpe, dpf-1);
  res.KerMax = atoi(point);
  free(point);

  //On récupère le nombre de tours
  point=ChToCh(nomF, dpf, weight_file(nomF));
  res.NbTours = atoi(point);
  free(point);

  return res;

}

SHIP* init_SHIP (int nbS, int km, int cm){
    SHIP *tab;
    tab = malloc(sizeof(SHIP)*nbS);
    int i;
    for(i=0;i<nbS;i++){
        tab[i].KER = km;
        tab[i].COQ = cm;
        tab[i].ShipID.type = ENT_SHIP;
        tab[i].ShipID.id = i;
        }
    return tab;
}

void free_SHIP (SHIP* tab) {
    free(tab);
}

SHIP* ATK (navalmap_t * nm, SHIP* tab, int IDatk,coord_t Impact){
    coord_t posAtk = nm->shipPosition[IDatk];
    int nb;
    int * list=NULL;
    tab[IDatk].KER = tab[IDatk].KER - 5;
    for (int dragibus=1;dragibus<=4;++dragibus){
      list = nm->getTargets(nm,posAtk,dragibus,&nb);
      if ((Impact.x - posAtk.x <2) && (Impact.y - posAtk.y <2)){
        printf("A pas touché - trop proche\n");
        return tab; // si impact trop proche on quitte
      }

      if (nm->map [Impact.y][Impact.x] .type == ENT_SHIP) {
          int i = 0;
          for (i=0;i<nb;++i) {
          //Si il touche
              if ((nm->shipPosition[list[i]].x == Impact.x) && (nm->shipPosition[list[i]].y == Impact.y)) {
                  tab[list[i]].COQ = tab[list[i]].COQ - 40;
              }
              if (nm->shipPosition[list[i]].x == Impact.x + 1) {
                  tab[list[i]].COQ = tab[list[i]].COQ - 20;
              }
              if (nm->shipPosition[list[i]].y == Impact.y + 1) {
                  tab[list[i]].COQ = tab[list[i]].COQ - 20;
              }
              if (nm->shipPosition[list[i]].x == Impact.x - 1) {
                  tab[list[i]].COQ = tab[list[i]].COQ - 20;
              }
              if (nm->shipPosition[list[i]].y == Impact.y - 1) {
                  tab[list[i]].COQ = tab[list[i]].COQ - 20;
              }
              ++i;
          }
      }
      /*
      if (list==NULL){
        printf("A pas touché - Liste nulle\n");
        return tab;
      }*/
    }
return tab;
}

void MOV(navalmap_t * nmap, int IDship, coord_t deplacement, SDL_Surface *ecran){
  draw_flotte(nmap, ecran, 1);
  moveShip(nmap, IDship, deplacement);
  draw_flotte(nmap, ecran, 0);
  SDL_Flip(ecran); // Mise à jour de l'écran
}

int SCN(navalmap_t *m, int shipID, SHIP * s){
  int *Liste;
  coord_t pos_s;
  pos_s = m->shipPosition[shipID];
  int nb_ship=0, IDNav_Det = -1;
  for(int i = 1; i<m->size.x; i++){
    Liste = m->getTargets(m,pos_s,i,&nb_ship);
    if(nb_ship>0){
      IDNav_Det = Liste[0];
      i=m->size.x+1;
    }
  }
  s[shipID].KER = s[shipID].KER-3;

  return IDNav_Det;

}
int jouer (int nbTour, char choix[3]){
  int flag = 1, res=0;
while (flag){
  printf("Saisissez les trois premières lettres de l'action à accomplir.\n En majuscule\n");
  if (strcmp(choix, "NON")==0){
    flag=0;
    res = 100;
  }

  if (strcmp(choix, "ATK")==0){
    flag=0;
    res =  101;
  }

  if (strcmp(choix, "MOV")==0){
    flag=0;
    res =  102;
  }
  else{
    if (flag== 1){
    printf("Cette commande n'est pas reconnue, merci de se référer au glossaire !\n");
    }
  }
}
  return res;
}

int decision(int nbTour, navalmap_t *m, int shipID, SHIP * s, int lastSCN){
  coord_t pos_ennemie; // Variable stockant la position d'un navire
  pos_ennemie.x = -1;
  pos_ennemie.y = -1;
  coord_t pos_self = m->shipPosition[shipID];

  if((pos_ennemie.x == -1  && pos_ennemie.y == -1) || lastSCN < 3){
     return jouer(nbTour, "SCN");
  }
  else if (pos_ennemie.x - pos_self.x <4 && pos_ennemie.y - pos_self.y <4){
    return jouer(nbTour, "ATK");
  }
  else{
    return jouer(nbTour, "MOV");
  }
}

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
    int me;
    int action=0, nbTour=0;
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

for(nbTour=0;nbTour<temp.NbTours;++nbTour){
  draw_flotte(carte, ecran, 0);
  SDL_Flip(ecran);
  for(int zimbabwe = 0; zimbabwe<carte->nbShips; ++zimbabwe){
// Initialisation des tubes
      int pipefd[2];  // fd du tube qui renvoie l'action dans le père
      int retour[1]; // tab de 3 int qui contient le code action, et ses coordonnées
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


      // caster en void les variables pour l'etat
      // et les stocker dans etat

      if (a!=0){
        //Processus père
        close(pipefd[1]); // on ferme le canal d'ecriture de pipefd
        read(pipefd[0],retour,sizeof(int) * 3); // on lit le tab retour envoyer a partir du proc fils
        printf("Valeur de retour %d\n", retour[0]);
        wait(NULL);
      }
      else{
        // Processus fils
        close(pipefd[0]);
        me = getpid();
        printf("\n%d\n", me);
        action = decision(nbTour, carte, 0, armada, 0);

        retour[0] = action; // on met dans la 1re case de retour le code action
        printf("Donnez coord x de l'action: ");
        printf("\nDonnez coord y de l'action: ");
        write(pipefd[1], retour, sizeof(int) * 3); // on ecrit dans le pipe le tab retour pour l'envoyer dans le Processus père (serveur)
        exit(-1);
      }

    }
    draw_flotte(carte, ecran, 1);
}

//------------------------

//------------------------

// Test
/*printf("Ker ID 0 %d\n", armada[0].KER);
printf("COQ ID 0 %d\n\n", armada[0].COQ);
printf("Ker ID 1 %d\n", armada[1].KER);
printf("COQ ID 1 %d\n", armada[1].COQ);*/
//------------------------

pauseStop();

//Free de tout
    SDL_Quit();
    free_SHIP(armada);
    free_navalmap(carte);

    return EXIT_SUCCESS;
}
