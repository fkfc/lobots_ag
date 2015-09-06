#ifndef TARGUMENTOS_H
#define TARGUMENTOS_H

class TArgumento {
  public:
	char *opc;
	char *val;
	TArgumento(const char* opc, const char *val);
	~TArgumento();
};


class TArgumentos {
  public:
	int n_argumentos;
	TArgumento **Argumento;
	
	TArgumentos(int argc, char *argv[]);
	~TArgumentos();
	char *getVal(char *opc);
	int isSet(char *opc);
	
};


#endif