#include "argumentos.h"
#include <stdlib.h>
#include <string.h>

bool startsWith(const char *pre, const char *str) {
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

TArgumento::TArgumento(const char* opc, const char *val) {
    this->opc = (char*) malloc(sizeof(char)*(strlen(opc)+1));
    this->val = (char*) malloc(sizeof(char)*(strlen(val)+1));
    strcpy(this->opc, opc);
    strcpy(this->val, val);
}

TArgumento::~TArgumento() {
    free(opc);
    free(val);
}

TArgumentos::TArgumentos(int argc, char *argv[]) {
    n_argumentos = 0;
    Argumento = (TArgumento**) malloc(sizeof(TArgumento*)*argc);
    for (int i = 1; i < argc; i++) {
	if (startsWith("-", argv[i])) {
	    if (((i + 1) < argc) && (startsWith("-", argv[(i+1)]) == false)) { //não é último argumento, e o próximo é parâmetro
		Argumento[n_argumentos] = new TArgumento(&(argv[i][1]), argv[(i+1)]);
		i++;
	    } else {
		Argumento[n_argumentos] = new TArgumento(&(argv[i][1]), "");
	    }
	} else {
	  Argumento[n_argumentos] = new TArgumento("", argv[i]);
	}
	n_argumentos++;
    }
  
}

char *TArgumentos::getVal(char *opc) {
    for (int i = 0; i < n_argumentos; i++) {
      if (strcmp(Argumento[i]->opc, opc) == 0) return Argumento[i]->val;
    } 
    return NULL;
}

int TArgumentos::isSet(char *opc) {
  for (int i = 0; i < n_argumentos; i++) {
      if (strcmp(Argumento[i]->opc, opc) == 0) return (i + 1);
  }
  return 0;
}


TArgumentos::~TArgumentos() {
  for (int i = 0; i < n_argumentos; i++) {
      delete(Argumento[i]);
  }
  if (n_argumentos > 0) delete(Argumento);
}