#include "spawnpoint.h"
#include <stdlib.h> 

#include <stdio.h>

TSpawnPoint::TSpawnPoint() {
  n_quadrados = 0;
  n_spawns = 0;
}

void TSpawnPoint::AddQuadrado(tquadrado quadrado) {
  quadrados_validos[n_quadrados] = quadrado;
  n_quadrados++;
}



void TSpawnPoint::GetSpawnPoint(int *x, int *y, int *z) {
  do {
    int quadrado = rand() % n_quadrados; //em qual dos quadrados vai nascer

    int menor_x = Menor(quadrados_validos[quadrado].x1, quadrados_validos[quadrado].x2, quadrados_validos[quadrado].x3, quadrados_validos[quadrado].x4);
    
    int menor_y = Menor(quadrados_validos[quadrado].y1, quadrados_validos[quadrado].y2, quadrados_validos[quadrado].y3, quadrados_validos[quadrado].y4);
    
    int menor_z = Menor(quadrados_validos[quadrado].z1, quadrados_validos[quadrado].z2, quadrados_validos[quadrado].z3, quadrados_validos[quadrado].z4);
    
    
    
    int maior_x = Maior(quadrados_validos[quadrado].x1, quadrados_validos[quadrado].x2, quadrados_validos[quadrado].x3, quadrados_validos[quadrado].x4);
    
    int maior_y = Maior(quadrados_validos[quadrado].y1, quadrados_validos[quadrado].y2, quadrados_validos[quadrado].y3, quadrados_validos[quadrado].y4);
    
    int maior_z = Maior(quadrados_validos[quadrado].z1, quadrados_validos[quadrado].z2, quadrados_validos[quadrado].z3, quadrados_validos[quadrado].z4);

    
    int dx = maior_x - menor_x; if (dx == 0) dx = 1;
    int dy = maior_y - menor_y; if (dy == 0) dy = 1;
    int dz = maior_z - menor_z; if (dz == 0) dz = 1;
    
    *x = menor_x + rand() % dx;
    *y = menor_y + rand() % dy;
    *z = menor_z + rand() % dz;
    
  } while ( GuardarSpawn(*x, *y, *z) == false ); //sorteia novamente caso este ponto já tenha sido utilizado
}

int TSpawnPoint::Maior(int a, int b, int c, int d) {
  int n = a;
  if (b > n) n = b;
  if (c > n) n = c;
  if (d > n) n = d;
  
  return n;
}



int TSpawnPoint::Menor(int a, int b, int c, int d) {
  int n = a;
  if (b < n) n = b;
  if (c < n) n = c;
  if (d < n) n = d;
  
  return n;
}

bool TSpawnPoint::GuardarSpawn(int x, int y, int z) { //guarda as coords no historico, retorna false se ja houver um igual
   int i = 0;
   while (i < n_spawns && (
     spawns_anteriores[i].x != x || 
     spawns_anteriores[i].y != y || 
     spawns_anteriores[i].z != z)
   ) i++;
   
   if (i < n_spawns) return false; //encontrou um spawn igual na lista de historico
   
   //armazena o novo spawnpoint no historico
   AddCoord(x, y, z);
   
   AddCoord(x + 1, y, z);
   AddCoord(x + 1, y, z + 1);
   AddCoord(x + 1, y, z - 1);
   AddCoord(x, y, z + 1);
   AddCoord(x - 1, y, z + 1);
   AddCoord(x - 1, y, z);
   AddCoord(x - 1, y, z - 1);
   AddCoord(x, y, z - 1);
   return true;
}

void TSpawnPoint::AddCoord(int x, int y, int z) {
   spawns_anteriores[n_spawns].x = x;
   spawns_anteriores[n_spawns].y = y;
   spawns_anteriores[n_spawns].z = z;
   n_spawns++; 
}

//abre um arquivo de spawnpoints
void TSpawnPoint::AbrirArquivo(const char *nome) {
  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  pFile = fopen ( nome , "rb" );
  if (pFile==NULL) return;

  //tamanho do arquivo
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  // aloca memoria para o arquivo
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) return;

  // copia o arquivo para a memoria
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) return;

  //fecha o arquivo
  fclose (pFile);
  
  //troca todos os caracteres 'estranhos' por espaço
  long p = 0; //ponteiro iterador
  while (p < lSize) {
    if (buffer[p] != ' ' && (buffer[p] < '0' || buffer[p] > '9')) buffer[p] = ' ';
    p++;
  }
  
  p = 0;
  int vetor_coords[1024];
  int cont_numeros = 0;
  while (p < lSize) {
     while (p < lSize && buffer[p] == ' ') p++; //encontra o proximo numero
     if (p < lSize) { //encontrou o numero, copiar p/ string temp
	char str_num[10];
        int str_i = 0;
	while (p < lSize && buffer[p] != ' ') {
	   str_num[str_i] = buffer[p];
	   str_i++;
	   p++;
	}
	str_num[str_i] = 0; //finaliza string
	vetor_coords[cont_numeros] = atoi(str_num); //converte para inteiro
	cont_numeros++;
     }
  }
  
  tquadrado q;
  int i = 0;
  while (i < cont_numeros) { //monta os quadrados e adiciona na lista de areas de spawn
      q.x1 = vetor_coords[i++];
      q.y1 = vetor_coords[i++];
      q.z1 = vetor_coords[i++];
      q.x2 = vetor_coords[i++];
      q.y2 = vetor_coords[i++];
      q.z2 = vetor_coords[i++];
      q.x3 = vetor_coords[i++];
      q.y3 = vetor_coords[i++];
      q.z3 = vetor_coords[i++];
      q.x4 = vetor_coords[i++];
      q.y4 = vetor_coords[i++];
      q.z4 = vetor_coords[i++];
      
      AddQuadrado(q);
  }
  
  free (buffer);
  
}