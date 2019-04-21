void ATK (navalmap_t * nm, SHIP* tab, int IDatk,coord_t Impact){
  //FOnction à refaire, on utilise une fonction à risque (get target) pour rien
  // en effet il n'a pas besoin de détecter tout ce que qu'il y a dans le range

  /*---Pseudo code
   Si la case Impact est un bateau :
      lui faire les dégats associés
  Sinon
      voir s'il n'est pas dans la cas au dessus/dessous/gauche/droite
      */
    printf("CAPITAINE nous tirons en %d x %d y\n", Impact.y, Impact.x);
    coord_t posAtk = nm->shipPosition[IDatk];
    tab[IDatk].KER = tab[IDatk].KER - 5;

      if (nm->map [Impact.y][Impact.x] .type == ENT_SHIP && ((Impact.x - posAtk.x < 4 && Impact.y - posAtk.y < 4) || (Impact.x - posAtk.x > 2 && Impact.y - posAtk.y > 2))) { //En plein dessus
        printf("(%d) a touché (%d) il avait %d COQ \n\n", IDatk, nm->map[Impact.y][Impact.x].id, tab[nm->map[Impact.y][Impact.x].id].COQ );
        tab[nm->map [Impact.y][Impact.x] .id].COQ = tab[nm->map [Impact.y][Impact.x].id].COQ - 40;
      }
      //En dessous
      else if (nm->map [Impact.y-1][Impact.x] .type == ENT_SHIP && ((Impact.x - posAtk.x < 4 && Impact.y - posAtk.y < 4) || (Impact.x - posAtk.x > 2 && Impact.y - posAtk.y > 2))) { //En plein dessus
        printf("(%d) a touché par éclat du dessous (%d) il avait %d COQ \n\n", IDatk, nm->map[Impact.y][Impact.x].id, tab[nm->map[Impact.y][Impact.x].id].COQ );
        tab[nm->map [Impact.y-1][Impact.x] .id].COQ = tab[nm->map [Impact.y-1][Impact.x].id].COQ - 20;
      }
      //A gauche
      else if (nm->map [Impact.y][Impact.x-1] .type == ENT_SHIP && ((Impact.x - posAtk.x < 4 && Impact.y - posAtk.y < 4) || (Impact.x - posAtk.x > 2 && Impact.y - posAtk.y > 2))) { //En plein dessus
        printf("(%d) a touché par éclat sur la gauche (%d) il avait %d COQ \n\n", IDatk, nm->map[Impact.y][Impact.x].id, tab[nm->map[Impact.y][Impact.x].id].COQ );
        tab[nm->map [Impact.y][Impact.x-1] .id].COQ = tab[nm->map [Impact.y][Impact.x-1].id].COQ - 20;
      }
      // Au dessus
      else if (nm->map [Impact.y+1][Impact.x] .type == ENT_SHIP && ((Impact.x - posAtk.x < 4 && Impact.y - posAtk.y < 4) || (Impact.x - posAtk.x > 2 && Impact.y - posAtk.y > 2))) { //En plein dessus
        printf("Touché par éclat au dessus de l'Impact\n\n");
        tab[nm->map [Impact.y+1][Impact.x] .id].COQ = tab[nm->map [Impact.y+1][Impact.x].id].COQ - 20;
      }
      //A droite
      else if (nm->map [Impact.y][Impact.x+1] .type == ENT_SHIP && ((Impact.x - posAtk.x < 4 && Impact.y - posAtk.y < 4) || (Impact.x - posAtk.x > 2 && Impact.y - posAtk.y > 2))) { //En plein dessus
        printf("Touché par éclat à droite de l'Impact\n\n");
        tab[nm->map [Impact.y][Impact.x+1] .id].COQ = tab[nm->map [Impact.y][Impact.x+1].id].COQ - 20;
      }
      else{
          printf("Raté\n\n");
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
  int * nbship_det = malloc(sizeof(int));

  int i= m->size.x + m->size.y;
   for (int j = 1; j<i; j++) {
    liste = m->getTargets(m, posSelf, j, nbship_det);
    if (*nbship_det != 0 && liste[0]!=shipID) {
      j = i+1;
    }
  }
  printf("Capitaine nous avons trouvé: %d navire(s) !\n", *nbship_det);
  printf("==> %d <==\n", liste[0]);
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
  printf("(%d) en %d x %d y fait ",shipID, m->shipPosition[shipID].y,m->shipPosition[shipID].x);
  if (IDCible==-1 || nbTour == 0){//Verification que c'est le premier tour
    return conversion("SCN");
  }

  pos_ennemie.x = m->shipPosition[IDCible].x;
  pos_ennemie.y = m->shipPosition[IDCible].y;

  coord_t pos_self = m->shipPosition[shipID];

  if(lastSCN > 3){
     return conversion("SCN");
  }
  if ((pos_ennemie.x - pos_self.x <4 && pos_ennemie.y - pos_self.y <4) /*&& (pos_ennemie.x - pos_self.x > 2 && pos_ennemie.y - pos_self.y > 2)*/){
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
    case 101: ATK (res.nm, res.armada, res.ID, res.nm->shipPosition[ID_d]);break;
    case 102: MOV (nmap, id, dep, ecran);break;
    case 103: res.ID_Det = SCN(res.nm, res.ID, res.armada);break;
  }
  return res;
}

void samu(navalmap_t * m, SHIP * s, int id){
  //Statut Actuel de Mort Unifié
  for(int i = 0; i<m->nbShips;++i){
    if (s[i].KER <= 0){
      printf("(%d) est immobilisé. Fin du jeu.\n", i);
      sleep(3);
      exit(0);
    }
    if (s[i].COQ <= 0){
      printf("(%d) est mort. Fin du jeu.\n", i);
      sleep(3);
      exit(0);
    }
  }
}

void repare (navalmap_t * m, SHIP * ships,int shipID) {
  if (ships[shipID].KER < 20) {
    return;
  }
  else {
    if (ships[shipID].COQ > 99) {
      return;
    }
    else {
      if((ships[shipID].KER > 74) && (ships[shipID].KER < 100)) {
        ships[shipID].COQ = 100;
        ships[shipID].KER = ships[shipID].KER - 20;
        return;
      }
      else {
        ships[shipID].COQ = ships[shipID].COQ + 25;
        ships[shipID].KER = ships[shipID].KER - 20;
        return;
      }
    }
  }

}
