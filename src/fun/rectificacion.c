#include "../head/test.h"



/*
   Entradas:        un puntero doble 'nuevamatriz1' de tipo float. Un putero 'dimensiones' de tipo entero.
   Funcionamiento:  recorre y evalua todos los elemento de la matriz nuevamatriz1, cuyo valores son menor a cero 
                    o bien, valores negativos; son cambiados por un cero.
   Salidas:         De tipo void, modifica valores en espacio de memoria a 'nuevamatriz1'.
*/
void rectificar(float** nuevamatriz1, int* dimensiones){
    int i,j;
        for(i=0;i<((dimensiones[1])-3);i++){
                    for(j=0;j<((dimensiones[0])-3);j++){
                        if((nuevamatriz1[i][j])<0){
                            nuevamatriz1[i][j]=0;
                        }
                        //printf("rectificado %.2f ",nuevamatriz1[i][j]); 
                     

                    }
              //  printf("\n");    
                }                
}


