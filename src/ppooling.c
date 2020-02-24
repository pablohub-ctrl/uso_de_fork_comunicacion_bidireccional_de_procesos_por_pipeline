#include "head/test.h"
#include "fun/pooling.c"
#include "fun/funcionrecorreMatrix.c"



void funcionleer(int* o){
        printf("el valor de la funcion es %d\n",*o);
        printf("el valor de la funcion es %d\n",o[1]);
}

int main(int argc, char *argv[]){




        int valordeRetorno;
        int c=atoi(argv[0]);
        //Se crea el pipe para la etapa pooling


        int fd4[2];
        int fd4z[2];
        pipe(fd4);
        pipe(fd4z);
        //Se crea el fork 
        int pid4;
        pid4 = fork();

        if(pid4==0){

          //  printf("Etapa Clasificador\n");
			close(fd4[1]);                      //Ciero el pipe como escritura
            close(fd4z[0]);                     //Ciero el pipe como lectura

			dup2(fd4[0], 6);                    //especifico al file descriptor que será un pipe lectura
            dup2(fd4z[1], 8);                   //especifico la escritura
			execv("src/pclasificador", argv);            //cargo el código del pooling
            printf("Si esto es visible es porque no se a cargado el codig de execve_clasificador\n");
			return 0;
        }else{
                //Cuyo caso es el padre
                close(fd4[0]);                  //se cierra el pipe como lectura
                close(fd4z[1]);                 //se cierra el pipe como escritura

               for(int ciclo=1;ciclo<=c;ciclo++){
        
                    // printf("For Pooling numero %d\n",ciclo);
                        int i,j;
                        int* valorLeido=malloc((sizeof(int))*2);  
                        int valorPipe0=0;

                        if(ciclo==1){

                            while(valorPipe0 != 8){
                                //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                                valorPipe0 = read(5,valorLeido,sizeof(valorLeido));        
                                                                        
                            }
                            valorPipe0=0;
                        }
                        else{
                                    //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                                    write(7,&ciclo,sizeof(int));

                                    while(valorPipe0 != 8){

                                            valorPipe0 = read(5,valorLeido,sizeof(valorLeido));        
                                                    
                                    }
                                    valorPipe0=0;
                        }


                        //Se asigna espacio en memoria para matrizRectificada
                        float** matrizRectificada=malloc((valorLeido[1])*(sizeof(float*)));

                        if(matrizRectificada==NULL){
                            printf("Error en asignación de memoria11\n");
                            return 0;
                        }
                        //Se asigna espacio en memoria para matrizRectificada 
                        for(i=0;i<valorLeido[1];i++){
                            
                            matrizRectificada[i]=malloc((valorLeido[0])*(sizeof(float*)));
                            
                            if(matrizRectificada[i]==NULL){
                                printf("Error en asignación de memoria22\n");
                                return 0;
                            }
                        }
                        
                        ////////////////////////////////////////////////////////////
                        //*Esta es la parte del printf de la imagen ya rectificada*//

                        for(i=0;i<valorLeido[1];i++){
                            for(j=0;j<valorLeido[0];j++){
                            
                                int numeroRetorno = read(5,&matrizRectificada[i][j], sizeof(float));
                            
                            }
                            
                        }
                        

                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        //AQUÍ DEBE COMUNICAR CON PIPELINE PARA INDICAR EL FIN DEL CICLO DE LA IMAGEN ENVIADA POR PIPE DESDE PADRE//
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        //ESTRUCTURA DE CONTROL
                        int valorX=c;
                        while(valorPipe0 != 4){
                            
                            valorPipe0= read(5,&valorX,sizeof(int));
                                                
                        }
                        valorPipe0=0;
                        int valorZ=13;
                        
                        write(7,&valorZ,sizeof(int));

                                        
                        /////////////////////////////////////////////////////////////////////////////////
                        ///// PROCESANDO INFORMACIÓN PROPIA DEL PROCESO O ETAPA DEL PIPELINE//////
                        /////////////////////////////////////////////////////////////////////////////////

                        //Se asigna espacio para la nueva matriz que contendrá los valores de 
                        //matriz pooling
                        float** matrizpooling=malloc(((valorLeido[1])/3)*(sizeof(float*)));

                        for(i=0;i<((valorLeido[1])/3);i++){
                            matrizpooling[i]=malloc(((valorLeido[0])/3)*(sizeof(float*)));
                            if(matrizpooling[i]==NULL){
                                    printf("Error en asignación de memoria22\n");
                                    return 0;
                            }
                        }
                        //asignación en memoria para valores correspondiente
                        //a la matriz que contendrá el pooling
                        int* dimensionPooling=malloc((sizeof(int))*2);
                        dimensionPooling[0]=((valorLeido[0])/3);
                        dimensionPooling[1]=((valorLeido[1])/3);

                        //////////////////////////aquí agrego a la matrizpooling sus píxeles de valor máximo proveniente de 3x3
                        pooling(matrizRectificada, matrizpooling, dimensionPooling);
                        ////////Se envía la información de la matriz pooling al pipe4


                        ///////////////////////////////Aquí comienzan los parámetros que
                        ///////////////////////////////Indican al proceso hijo la comunicacion de datos y envíos de los mismos
            
                
                        if(ciclo==1){

                                int numeroRetorno = write(fd4[1],dimensionPooling, sizeof(dimensionPooling));
                                
                                for(i=0;i<dimensionPooling[1];i++){
                                    for(j=0;j<dimensionPooling[0];j++){
                                        int numeroRetorno2 = write(fd4[1], &matrizpooling[i][j], sizeof(float));
                                    }
                                }

                        }else{
                        
                        
                                int valorPipe2 = 5;
                                int valorLeido2 = 0;
                                //ESTRUCTURA DE CONTROL O  SEÑALES PARA INICIAR COMUNICACIÓN DE LOS PROCESOS PADRE E HIJO        
                                while(valorPipe2 != 4){
                                    
                                    valorPipe2 = read(fd4z[0], &valorLeido2, sizeof(int));        
                            
                                }
                                valorPipe2 = 0;
                                


                                //escribo por el pipe las dimensiones de la matriz
                                int numeroRetorno = write(fd4[1],dimensionPooling, sizeof(dimensionPooling));
                                
                                //escribo por el pipe la matriz con los valores ya convolucionados
                                for(i=0;i<dimensionPooling[1];i++){
                                    for(j=0;j<dimensionPooling[0];j++){
                                            int numeroRetorno2 = write(fd4[1], &matrizpooling[i][j], sizeof(float));
                                    }
                                }
                                
                            }



            
                        ////////////////////////////////Aquí comienzan los parámetros que
                        ////////////////////////////////Indican al proceso cierre de ciclo con hijo

                        // ASÍ COMIENZO DICIENDO A MI HIJO QUE ESPERO PARA FINALIZAR CON EL CICLO

                        int valorPipe2 = 0;
                        int valorLeido2 = -1;
                        //int valorZ=ciclo;
                        valorZ=ciclo;
                        
                        //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                        //PADRE ENVÍA UNA SEÑAL PARA QUE HIJO LE RESPONDA, SIGNIFICANDO EL NUEVO CICLO
                        write(fd4[1],&valorZ,sizeof(int));

            
                        while(valorPipe2 != 4){
                        
                        valorPipe2 = read(fd4z[0], &valorLeido2, sizeof(int));        
                        
                        }
                        valorPipe2 = 0;   

                        //libero el espacio para las dos matrices, rectifica y pooling
                        liberarMatriz(matrizRectificada, valorLeido[1]);
                        liberarMatriz(matrizpooling, dimensionPooling[1]);
                        free(valorLeido);          //Libero espacio de las dimensiones guardadas
                        free(dimensionPooling);    

                }//fin del ciclo for

            }

        // printf("Fin Del proceso Pooling\n");
        wait(NULL); //espera a que el proceso hijo termine
        close(5);   //cierra el descriptor
        close(7);
        
        close(fd4[1]);  
        close(fd4z[0]); 
        
        
    return 0;      

}
