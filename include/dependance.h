#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "./navalmap/navalmap.h"
#include "./navalmap/nm_rect.h"
#include "./navalmap/navalmap.c"
#include "./navalmap/nm_rect.c"
#include <tgmath.h>

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
// Struct variable de jeu
typedef struct {
  navalmap_t * nm;
  SHIP * armada;
  int ID;
  int ID_Det;
  int code_retour;
} VAR_JEU;
