#include "head/test.h"
#include "fun/clasificacion.c"



int main(int argc, char *argv[]){

        int c=atoi(argv[0]);
        int opt;
        float n=atof(argv[2]);
        int p=atoi(argv[3]);
        int b=atoi(argv[4]);
        
        int valordeRetorno; 
        //Se crea el pipe para la etapa escritura
      
        int fd5[2];
        int fd5z[2];
        int fdclasificacion[2];
        pipe(fd5);
        pipe(fd5z);
        pipe(fdclasificacion);
        
        //Se crea el fork 
        int pid5;
        pid5 = fork();

        if(pid5==0){
            
         //   printf("Etapa escritura\n");
			close(fd5[1]);                  //Ciero el pipe como escritura
            close(fd5z[0]);                  //Ciero el pipe como lectura
			dup2(fd5[0], 7);     //especifico al file descriptor que será un pipe lectura
            dup2(fd5z[1], 9); //especifico la escritura
            close(fdclasificacion[1]);                  //Ciero el pipe como escritura
			dup2(fdclasificacion[0], 10);     //especifico al file descriptor que será un pipe lectura


			execv("src/p_writing", argv);           //cargo el código del pooling
            printf("Si esto es visible es porque no se a cargado el codig de execv_Escritura\n");
			
            return 0;
        }else{
            //Cuyo caso es el padre
            close(fd5[0]);  //se cierra el pipe como lectura
            close(fd5z[1]);  //se cierra el pipe como escritura
            close(fdclasificacion[0]);  //se cierra el pipe como lectura

            for(int ciclo=1;ciclo<=c;ciclo++){
                
                    //    printf("For Claficador numero %d\n",ciclo);
                    int i,j;
                    int* valorLeido=malloc((sizeof(int))*2);  
                    int valorPipe0=0;
                    //se asigna espacio para dimensiones que serán leida por el pipe

                    if(ciclo==1){
                        while(valorPipe0 != 8){
                            valorPipe0 = read(6,valorLeido,sizeof(valorLeido));                 
                        }
                        valorPipe0=0;
                    }
                    else{
                            write(8,&ciclo,sizeof(int));
                            while(valorPipe0 != 8){
                                valorPipe0 = read(6,valorLeido,sizeof(valorLeido));        
                            }
                            valorPipe0=0;
                    }
                    
                    float** matrizpooling=malloc((valorLeido[1])*(sizeof(float*)));
                    if(matrizpooling==NULL){
                        printf("Valor leido %d\n",(valorLeido[1]));
                        printf("Error en asignación de memoria00\n");
                        return 0;
                    }
                    //se asigna espacio en memoria
                    for(i=0;i<valorLeido[1];i++){
                        matrizpooling[i]=malloc((valorLeido[0])*(sizeof(float*)));
                        if(matrizpooling[i]==NULL){
                            printf("Error en asignación de memoria2\n");
                            return 0;
                        }
                    }

                    //ciclo for para la lectura de la matriz rectificada desde el pipe
                    for(i=0;i<valorLeido[1];i++){
                        for(j=0;j<valorLeido[0];j++){
                            valordeRetorno = read(6,&matrizpooling[i][j],sizeof(float));
                        }
                    }


                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //AQUÍ DEBE COMUNICAR CON PIPELINE PARA INDICAR EL FIN DEL CICLO DE LA IMAGEN ENVIADA POR PIPE DESDE PADRE//
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //ESTRUCTURA DE CONTROL
                    int valorX=c;
                    while(valorPipe0 != 4){
                        valorPipe0= read(6,&valorX,sizeof(int));
                    }
                    valorPipe0=0;

                    //un valor cualquiera para comunicar
                    int valorZ=13;
                    write(8,&valorZ,sizeof(int));


                    ///////////////////////////////////////////////////////////////////////
                    //////// AQÚI VA LA ETAPA NETA DEL PROCESO EN CUANTO A COMPUTO ////////  
                    ///////////////////////////////////////////////////////////////////////
    

                    float umbral=clasificacion(matrizpooling, valorLeido);
                    //printf("El umbral de clasificacion es %f\n",umbral);

                    int clasificacionFinal=clasifica(n, umbral, p);

                    //////////////////////////////////////////////////////////
                    ////// Aquí va LA COMUNICACIÓ CON EL HIJO DEL PROCESO/////  
                    //////////////////////////////////////////////////////////

                    //////////////////////////////////////////////////////////
                    //////// Aquí va el envío de la matriz resultante ////////  
                    //////////////////////////////////////////////////////////

                    if(ciclo==1){

                            int numeroRetorno = write(fd5[1],valorLeido, sizeof(valorLeido));
                        
                            write(fdclasificacion[1],&clasificacionFinal, sizeof(int));

                            for(i=0;i<valorLeido[1];i++){
                                for(j=0;j<valorLeido[0];j++){
                                    int numeroRetorno2 = write(fd5[1], &matrizpooling[i][j], sizeof(float));
                                }      
                            }

                    }else{
                            
                            
                                int valorPipe2 = 5;
                                int valorLeido2 = 0;
                                //ESTRUCTURA DE CONTROL O  SEÑALES PARA INICIAR COMUNICACIÓN DE LOS PROCESOS PADRE E HIJO                    
                                while(valorPipe2 != 4){
                                    valorPipe2 = read(fd5z[0], &valorLeido2, sizeof(int));        
                                }
                                valorPipe2 = 0;
                                
                                //escribo por el pipe las dimensiones de la matriz
                                int numeroRetorno = write(fd5[1],valorLeido, sizeof(valorLeido));

                                //escribo por el pipe el valor de la clasificación
                                write(fdclasificacion[1],&clasificacionFinal, sizeof(int));
                                
                                //escribo por el pipe la matriz con los valores ya pasados por el pooling - proceso anterior
                                for(i=0;i<valorLeido[1];i++){
                                        for(j=0;j<valorLeido[0];j++){
                                            int numeroRetorno2 = write(fd5[1], &matrizpooling[i][j], sizeof(float));
                                        }
                                }
                                    
                        }

                    /////////////Aquí comienzan los parámetros que
                    /////////////comunican señal al hijo para el cierre de ciclo

                        
                    int valorPipe2 = 0;
                    int valorLeido2 = -1;
                    valorZ=ciclo;
                    //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                    //PADRE ENVÍA UNA SEÑAL PARA QUE HIJO LE RESPONDA, SIGNIFICANDO EL NUEVO CICLO
                    write(fd5[1],&valorZ,sizeof(int));
                    while(valorPipe2 != 4){
                        valorPipe2 = read(fd5z[0], &valorLeido2, sizeof(int));        
                    }
                    valorPipe2 = 0;    



            }//termino de for

        }//cierre else

        // printf("Fin Del proceso Clasificacion\n");
        wait(NULL); //espera a que el proceso hijo termine
        close(6);   //cierra el descriptor
        close(8);   //cierra el descriptor
        close(fd5z[0]);   //cierra el descriptor
        close(fd5[1]);   //cierra el descriptor
        close(fdclasificacion[1]); //cierra el pipe
        
        
    return 0;      

}