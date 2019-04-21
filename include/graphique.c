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

	SDL_FillRect(c, NULL, SDL_MapRGB(c->format, 255, rand()%200, rand()%200));
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
	for(i=0;i<hauteur; ++i){
		for(j=0; j<largeur; ++j){
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
    SDL_FillRect(c, NULL, SDL_MapRGB(c->format, 50*IDnavire-1, 50*IDnavire, 255-((IDnavire+50)*4)));
  }
  else{
    SDL_FillRect(c, NULL, SDL_MapRGB(c->format, 0, 0, 0));
  }
  pos=repere_cadrillage(20, 20, 44, nm->shipPosition[IDnavire].x, nm->shipPosition[IDnavire].y);
  /*position.x = nm->shipPosition[IDnavire].x;
  position.y = nm->shipPosition[IDnavire].y;*/

  positionR.x=pos.x; positionR.y=pos.y;

  SDL_BlitSurface(c, NULL, ecran, &positionR); // Collage de la surface sur l'écran
  SDL_FreeSurface(c);

}

void draw_flotte(navalmap_t *nm, SDL_Surface *ecran, int Noir){
  int i;
  for(i=0;i<nm->nbShips;++i){
    draw_ship(nm, ecran, Noir, i);
  }
}
