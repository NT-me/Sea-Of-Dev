
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
