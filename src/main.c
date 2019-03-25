#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../include/navalmap/navalmap.h"
#include "../include/navalmap/nm_rect.h"
#include "../include/navalmap/navalmap.c"
#include "../include/navalmap/nm_rect.c"

typedef struct{
  map_t type; //Type de la map_t
  coord_t size; //Taille de la map
  int nbShips; //Nom de navire
  int CoqMax; //Coque maximum
  int KerMax; //Kerosen maximum
} fichier;

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
  int fd, count=0, dpa=0, dpb=0, dpc=0, dpd=0, dpe=0, dpf=0, posT=0;
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

  point = ChToCh(nomF, 0,dpa-1);
  if (strcmp(point, "rectangle")==0){
    res.type=MAP_RECT;
  }
  else{
    res.type=MAP_TOTAL;
  }
  free(point);

  point= ChToCh(nomF, dpa, dpb-1);
  res.size.x = atoi(point);
  free(point);
  point= ChToCh(nomF, dpb, dpc-1);
  res.size.y = atoi(point);
  free(point);
  point= ChToCh(nomF, dpc, dpd-1);
  res.nbShips = atoi(point);
  free(point);
  point=ChToCh(nomF, dpd, dpe-1);
  res.CoqMax = atoi(point);
  free(point);
  point=ChToCh(nomF, dpe, dpf-1);
  res.KerMax = atoi(point);
  free(point);

  return res;

}

coord_t repere_cadrillage(int coinX, int coinY,int tailleC, int PosX, int PosY){
  coord_t res;
  res.x=coinX+(tailleC/2)+(tailleC*PosX);
  res.y=coinY+(tailleC/2)+(tailleC*PosY);
  return res;
}


int main(int argc, char *argv[])
{
// Variables du main
    SDL_Surface *ecran = NULL;
    navalmap_t *carte;
    fichier temp;
//------------------------
//Test ligne de commande
if (argc==0){
  printf("Pas d'argument, merci d'en mettre un.\n");
  return EXIT_SUCCESS;
}
if (argc<1){
  printf("Trop d'argument.\n");
}
//------------------------
//Initialisation des bibliothèques
    SDL_Init(SDL_INIT_VIDEO);
    initNavalMapLib();
//------------------------
//Corps du main
    ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
    temp = read_file(argv[1]);

    carte=init_navalmap(temp.type, temp.size, temp.nbShips);

		// PositionX, PostionY, nbre de case de côté, nbre de case de haut, taille des cases de px, taille bordure en px, ecran
    if(carte->type==MAP_RECT){
      draw_cadrillage(20, 20, carte->size.x ,carte->size.y , 44, 2, ecran);
    }

    SDL_WM_SetCaption("Sea of dev - Théo Nardin, Emile Dadou", NULL);
		SDL_Flip(ecran); // Mise à jour de l'écran
    weight_file(argv[1]);
    pauseStop();
//------------------------
//Free de tout
    SDL_Quit();
    free_navalmap(carte);
    return EXIT_SUCCESS;
}
