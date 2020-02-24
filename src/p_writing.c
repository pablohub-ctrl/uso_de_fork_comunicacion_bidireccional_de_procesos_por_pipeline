#include "head/test.h"
#include "fun/saveImagen.c"
#include "fun/crearnombre.c"


int main(int argc, char *argv[]){
    //int clasifica;
    int i, j;
    int c=atoi(argv[0]);
    //se asigna espacio para dimensiones que serán leida por el pipe
    int* valorLeido=malloc((sizeof(int))*2);
    int valorClasificacion=-1;
    int valorPipe0=0;
    int b=atoi(argv[4]);
    
    for(int ciclo=1; ciclo<=c; ciclo++){
    
                //printf("For Escritura numero %d\n",ciclo);

                if(ciclo==1){
                    //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                    while(valorPipe0 != 8){
                        valorPipe0 = read(7,valorLeido,sizeof(valorLeido));        
                    }
                    valorPipe0=0;
                    //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                    while(valorPipe0 != 4){
                        valorPipe0 = read(10,&valorClasificacion,sizeof(int));        
                    }
                    valorPipe0=0;

                }else{
                            //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                            write(9,&ciclo,sizeof(int));

                            while(valorPipe0 != 8){
                                valorPipe0 = read(7,valorLeido,sizeof(valorLeido));        
                            }
                            valorPipe0=0;

                            while(valorPipe0 != 4){
                                valorPipe0 = read(10,&valorClasificacion,sizeof(int));        
                            }
                            valorPipe0=0;
                    }


                    //Se asigna espacio en memoria para matrizpooling
                    float** matrizpooling=malloc((valorLeido[1])*(sizeof(float*)));

                    if(matrizpooling==NULL){
                        printf("Error en asignación de memoria11\n");
                        return 0;
                    }
                    //Se asigna espacio en memoria para matrizpooling 
                    for(i=0;i<valorLeido[1];i++){
                        
                        matrizpooling[i]=malloc((valorLeido[0])*(sizeof(float*)));
                        if(matrizpooling[i]==NULL){
                            printf("Error en asignación de memoria22\n");
                            return 0;
                        }
                    }
                            
                    //*Esta es la parte del printf de la imagen ya rectificada*//

                    for(i=0;i<valorLeido[1];i++){
                        for(j=0;j<valorLeido[0];j++){
                           read(7,&matrizpooling[i][j], sizeof(float));
                        }
                    }



                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //AQUÍ DEBE COMUNICAR CON PIPELINE PARA INDICAR EL FIN DEL CICLO DE LA IMAGEN ENVIADA POR PIPE DESDE PADRE//
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //ESTRUCTURA DE CONTROL
                    
                    int valorX=c;
                    while(valorPipe0 != 4){
                        valorPipe0= read(7,&valorX,sizeof(int));
                    }
                    valorPipe0=0;
                    int valorZ=13;
                    write(9,&valorZ,sizeof(int));

                    
                    /////////////////////////////////////////////////////////////////////////////
                    /////////////////////////Etapa propia del Proceso escritura//////////////////
                    /////////////////////////////////////////////////////////////////////////////

                    //test de crear la matriz de valores enteros
                    int** ultima_matriz=malloc((valorLeido[1])*(sizeof(int*)));  
                    //se asigna espacio en memoria
                    for(i=0;i<valorLeido[1];i++){
                        ultima_matriz[i]=malloc((valorLeido[0])*(sizeof(int*)));
                        if(ultima_matriz[i]==NULL){
                            printf("Error en asignación de memoria2\n");
                            return 0;
                        }
                    }
                    
                    for(i=0;i<valorLeido[1];i++){
                        for(j=0;j<valorLeido[0];j++){
                            ultima_matriz[i][j] = matrizpooling[i][j];
                        }
                    }

                    MatrizPNG* matrizPng = (MatrizPNG*) malloc(sizeof(MatrizPNG));
                    matrizPng->width = valorLeido[0];
                    matrizPng->height = valorLeido[1];
                    matrizPng->image = ultima_matriz;
                    char* nombre = nombre_imagen_salida(ciclo);
                    
                    //Se escribe la imagen en disco
                    saveImage(matrizPng, nombre);

                    char* nombrePrint=nombre_imagen_printf(ciclo);    
                    
                    char* nearlyBlack=NULL;
                    if(valorClasificacion == 1){
                        nearlyBlack="yes";
                    }else{
                        nearlyBlack="no";
                    }
               
                    if(b==1){
                    printf("|\t%s\t|\t  %s\t      |\n",nombrePrint,nearlyBlack);    
                    }
                
    }

        //printf("Fin Del proceso ecritor\n");
        close(7);   //cierra el descriptor
        close(9);   //cierra el descriptor
        close(10);   //cierra el descriptor
        
        
        
    return 0;  

    
}