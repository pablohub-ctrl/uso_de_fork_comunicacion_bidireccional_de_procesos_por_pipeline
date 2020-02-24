#include "../head/test.h"

/*
   Entradas:        un puntero doble 'matriz' de tipo float. Un putero 'dimensiones' de tipo entero.
   Funcionamiento:  Encuentra el valor del umbral de oscuridad de la imagen.
   Salidas:         un valor de tipo float: 'umbral'.
*/
float clasificacion(float** matriz, int* dimensiones){

        int ancho=dimensiones[0];
        int alto=dimensiones[1];
        int anchoxalto=ancho*alto;
        int black=0;

        for(int i=0;i<alto;i++){
            for(int j=0;j<ancho;j++){
                //por cada elemento de la mtriz pooling se evalua en si es igual a 0.
                //si el elemento es igual a cero, incremente el valor de la variable black
                if(matriz[i][j]==0){
                    black=black+1;
                }
            }
        }
        // valor umbral corresponde a la división de 'black' dividido por el área de la matriz poolinizada
        float umbral=(float)black/anchoxalto;
        
        return umbral;
}

/*
   Entradas:        un valor 'n' de tipo float, un valor 'umbral' de tipo float y un valor 'porcentaje' de tipo entero. 
   Funcionamiento:  verifica si una imagen es nearly black mediante un 1 o 0, 1==true y 0==false.
   Salidas:         un valor de tipo entero.
*/
int clasifica(float n, float umbral, int porcentaje){

    int x= (100*umbral)/n;
    x=x-100;
    
    if(x>=porcentaje){
        return 1;
    }else{
            return 0;
        }
}

