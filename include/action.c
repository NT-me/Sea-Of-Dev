void ATK (navalmap_t * nm, SHIP* tab, int IDatk,coord_t Impact){
  //FOnction à refaire, on utilise une fonction à risque (get target) pour rien
  // en effet il n'a pas besoin de détecter tout ce que qu'il y a dans le range

  /*---Pseudo code
   Si la case Impact est un bateau :
      lui faire les dégats associés
  Sinon
      voir s'il n'est pas dans la cas au dessus/dessous/gauche/droite

      */
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
}

void MOV(navalmap_t * nmap, int IDship, coord_t deplacement, SDL_Surface *ecran){
  draw_flotte(nmap, ecran, 1);
  moveShip(nmap, IDship, deplacement);
  draw_flotte(nmap, ecran, 0);
  SDL_Flip(ecran); // Mise à jour de l'écran
}

int SCN(navalmap_t *m, int shipID, SHIP * s){
  int* liste = malloc(sizeof(int)*m->nbShips);
  coord_t posSelf;
  posSelf=m->shipPosition[shipID];

  int i=1, nb_ship=-666;
  while (i<m->size.x && nb_ship == -666){
    liste = m->getTargets(m, posSelf, i, &nb_ship);
    ++i;
  }
  printf("%d\n", liste[0]);
  return liste[0];
}

int conversion (char choix[3]){
  int res=0;

  //printf("%s\n", choix);
  if (strcmp(choix, "NON")==0){
    res = 100;
  }

  if (strcmp(choix, "ATK")==0){
    res =  101;
  }

  if (strcmp(choix, "MOV")==0){
    res =  102;
  }
  if (strcmp(choix, "SCN")==0){
    res = 103;
  }
  printf("%s\n", choix);
  return res;
}

int decision(int nbTour, navalmap_t *m, int shipID, SHIP * s, int lastSCN, int IDCible){
  coord_t pos_ennemie; // Variable stockant la position d'un navire
  printf("(%d)",shipID);
  if (IDCible==-1 || nbTour == 0){//Verification que c'est le premier tour
    return conversion("SCN");
  }

  pos_ennemie.x = m->shipPosition[IDCible].x;
  pos_ennemie.y = m->shipPosition[IDCible].y;

  coord_t pos_self = m->shipPosition[shipID];

  if(lastSCN > 3){
     return conversion("SCN");
  }
  if (pos_ennemie.x - pos_self.x <4 && pos_ennemie.y - pos_self.y <4){
    return conversion("ATK");
  }
  else{
    return conversion("MOV");
  }
}

// Vérification si scanner util
int** verif_scanner (int id, navalmap_t * carte, int code_action, int ID_Det, int** nb_scan) {
        if (code_action == 103 || code_action == 0) {
          nb_scan[id][0] = 0;
          nb_scan[id][1] = ID_Det;
        }
        else {
          nb_scan[id][0] ++;
        }
      return nb_scan;
}


// Fonction de Jeu;

VAR_JEU make_action (navalmap_t * nmap, SHIP * ships, int id, int act, int ID_d, SDL_Surface *ecran) {
  VAR_JEU res;
  res.nm = nmap;
  res.armada = ships;
  res.ID = id;
  res.code_retour = act;
  coord_t dep;
  dep.x=rand()%2;
  dep.y=rand()%2;

  switch (res.code_retour) {
    case 100: break;
    case 101: /*ATK (res.nm, res.armada, res.ID, res.nm->shipPosition[ID_d]);*/break;
    case 102: MOV (nmap, id, dep, ecran);break;
    case 103: /*if (id==0){res.ID_Det =1;} else {res.ID_Det =0;}*/SCN(res.nm, res.ID, res.armada);break;


    return res;
  }
  return res;
}
