//Gerador de spawn points para os robos
//A função AddQuadrado adiciona áreas válidas onde o robo pode 'nascer'

#ifndef spawnpoint_H
#define spawnpoint_H

#define SP_MAX_QUAD 50
#define SP_MAX_HIST 250

typedef struct {
  int x1, y1, z1; 
  int x2, y2, z2;
  int x3, y3, z3;
  int x4, y4, z4;
} tquadrado;


class TSpawnPoint {
public:
  TSpawnPoint();
  void AddQuadrado(tquadrado quadrado); //adiciona área válida onde o robo pode 'nascer'
  void GetSpawnPoint(int *x, int *y, int *z); //retorna um spawn point aleatorio
  void AbrirArquivo(const char *nome); //abre um arquivo de spawnpoints
private:
  tquadrado quadrados_validos[SP_MAX_QUAD];
  int n_quadrados;
  struct {
    int x, y, z;
  } spawns_anteriores[SP_MAX_HIST];
  int n_spawns;
  
  int Menor(int a, int b, int c, int d);
  int Maior(int a, int b, int c, int d);
  bool GuardarSpawn(int x, int y, int z); //guarda as coords no historico, retorna false se ja houver um igual
  void AddCoord(int x, int y, int z); //guarda as coords na estrutura de dados do historioco
};

#endif

