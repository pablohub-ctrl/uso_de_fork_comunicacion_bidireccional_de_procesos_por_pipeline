#include "../head/test.h"


/*
   Entradas:        un entero, un puntero de tipo entero y un doble puntero de tipo entero
   Funcionamiento:  el puntero de tipo entero contendrá las dimensiones de alto por ancho de la matriz, 
                    el entero es el valor del filedescriptor o valor asignado al pipe.
                    -su funcionamiento consta de enviar la imagen binarizada por el pipe
   Salidas:         un valor de retorno de tipo entero para confirmación de estado.
*/
int loadImagen(int* dimensiones,int** matriz,int fichero){

    int i,j;

    for(i=0; i<dimensiones[1]; i++){
        for(j=0; j<dimensiones[0];j++){
            write(fichero, &(matriz[i][j]),sizeof(int));
            
        }
    }

    return 2;

}
/*
   Entradas:        un entero y un doble puntero de tipo flotante
   Funcionamiento:  libera el espacio en memoria asignado a la matriz entregada como parámetro.
   Salidas:         tipo void.
*/
void liberarMatriz(float** matriz, int alto){
    int i;
    for(i=0;i<alto;i++){
        free(matriz[i]); 
    }
    free(matriz);
}

/*
   Entradas:        un entero y un doble puntero de tipo entero
   Funcionamiento:  libera el espacio en memoria asignado a la matriz entregada como parámetro.
   Salidas:         tipo void.
*/
void liberarMatrix(int** matriz, int alto){
    int i;
    for(i=0;i<alto;i++){
        free(matriz[i]); 
    }
    free(matriz);
}