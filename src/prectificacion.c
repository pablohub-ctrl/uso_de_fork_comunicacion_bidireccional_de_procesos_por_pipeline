#include "head/test.h"
#include "fun/rectificacion.c"
#include "fun/funcionrecorreMatrix.c"



int main(int argc, char *argv[]){
    
        int c=atoi(argv[0]);

        
        //Se crea el pipe para la etapa pooling
        int fd3[2];
        int fd3z[2];
        pipe(fd3);
        pipe(fd3z);
        
        //Se crea el fork 
        int pid3;
        pid3 = fork();

        if(pid3==0){
            //printf("EtapaRectificación\n");
			close(fd3[1]);                  //Ciero el pipe como escritura
            close(fd3z[0]);                 //cierro el pipe como lectura
			
            dup2(fd3[0], 5);     //especifico al file descriptor que será un pipe lectura
            dup2(fd3z[1], 7); //especifico la escritura
            execv("src/ppooling", argv);           //cargo el código del pooling
            printf("Si esto es visible es porque no se a cargado el codig de execve_Pooling\n");
			return 0;
        }else{
            
            //Cuyo caso es el padre
            close(fd3[0]);  //se cierra el pipe como lectura
            close(fd3z[1]);  //se cierra el pipe como escritura
            
    


            for(int ciclo=1;ciclo<=c;ciclo++){ //ciclo for para cada rectificación de imagenes cargadas

                // printf("For Rectificacion numero %d\n",ciclo);

                
                int i,j;
                int* valorLeido=malloc((sizeof(int))*2);  
                int valorPipe0=0;

                if(ciclo==1){
                    //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                    while(valorPipe0 != 8){

                        valorPipe0 = read(4,valorLeido,sizeof(valorLeido));        
                                
                    }
                    valorPipe0=0;

                }
                else{
                            //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                            write(8,&ciclo,sizeof(int));

                            while(valorPipe0 != 8){
                                    valorPipe0 = read(4,valorLeido,sizeof(valorLeido));          
                            }
                            valorPipe0=0;
                    }
                
                //Se asigna espacio en memoria para matrizRectificada
                float** matrizRectificada=malloc(((valorLeido[1])-3)*(sizeof(float*)));

                if(matrizRectificada==NULL){
                    printf("Error en asignación de memoria11\n");
                    return 0;
                }
                //Se asigna espacio en memoria para matrizRectificada 
                for(i=0;i<((valorLeido[1])-3);i++){
                    
                    matrizRectificada[i]=malloc(((valorLeido[0])-3)*(sizeof(float*)));
                    
                    if(matrizRectificada[i]==NULL){
                        printf("Error en asignación de memoria22\n");
                        return 0;
                    }
                }
                
                

                for(i=0;i<valorLeido[1]-3;i++){
                    for(j=0;j<valorLeido[0]-3;j++){

                        int numeroRetorno = read(4,&matrizRectificada[i][j], sizeof(float));
                        
                    }
                }


                ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //AQUÍ DEBE COMUNICAR CON PIPELINE PARA INDICAR EL FIN DEL CICLO DE LA IMAGEN ENVIADA POR PIPE DESDE PADRE//
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO

                int valorX=-1;
                while(valorPipe0 != 4){

                    valorPipe0= read(4,&valorX,sizeof(int));
                                
                }
                valorPipe0=0;
                write(8,&c,sizeof(int));


                /////////////////////////////////////////////////////////
                ////////////// PREPARO LA ETAPA RECTIFICACIÓN ///////////   ESTAS SON LAS ETAPAS QUE SON PROPIAS DEL PROCESO
                /////////////////////////////////////////////////////////   LAS DEMÁS ETAPAS SON SÓLO DE COMUNICACIÓN

                //////////////////////////////AQUÍ SE APLICA LA RECTIFICACIÓN A LA MATRIZ CONVOLUCIONADO
                //////////////////////////////PASA DE SER UNA MATRIZ CON CONTENIDO CONVOLUCIONADO A CONTENIDO RECTIFICADO
                rectificar(matrizRectificada, valorLeido);

                //crear la nueva dimension sin la diferencia de menos 3
                //para enviarla al pipe como una matriz ya rectificada
                //por tanto se asigna espacio en memoria
                int* dimensionNuevo=malloc((sizeof(int))*2);
                dimensionNuevo[0]=((valorLeido[0])-3);
                dimensionNuevo[1]=((valorLeido[1])-3);


                ///////////////////////////Aquí comienzan los parámetros que
                ///////////////////////////Indican al proceso hijo la comunicacion de datos y envíos de los mismos
            
                if(ciclo==1){

                    int numeroRetorno = write(fd3[1],dimensionNuevo, sizeof(dimensionNuevo));
                
                    for(i=0;i<dimensionNuevo[1];i++){
                        for(j=0;j<dimensionNuevo[0];j++){
                
                            int numeroRetorno2 = write(fd3[1], &matrizRectificada[i][j], sizeof(float));
                
                        }     
                    }
                
                    
                }else{
                        
                        int valorPipe2 = 5;
                        int valorLeido2 = 0;
                        //ESTRUCTURA DE CONTROL O  SEÑALES PARA INICIAR COMUNICACIÓN DE LOS PROCESOS PADRE E HIJO                   
                        while(valorPipe2 != 4){
                            //printf("PADRE ELSE- VALORPIPE2 READ  %d\n", valorPipe2);
                            valorPipe2 = read(fd3z[0], &valorLeido2, sizeof(int));        
                        }
                        
                        valorPipe2 = 0;

                        //escribo por el pipe las dimensiones de la matriz
                        int numeroRetorno = write(fd3[1],dimensionNuevo, sizeof(dimensionNuevo));
                        
                        for(i=0;i<dimensionNuevo[1];i++){
                            for(j=0;j<dimensionNuevo[0];j++){
                                int numeroRetorno2 = write(fd3[1], &matrizRectificada[i][j], sizeof(float));
                            }
                        }
                                
                    }



                

                int valorPipe2 = 0;
                int valorLeido2 = -1;
                int valorZ=ciclo;
                
                //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                //PADRE ENVÍA UNA SEÑAL PARA QUE HIJO LE RESPONDA, SIGNIFICANDO EL NUEVO CICLO
                write(fd3[1],&valorZ,sizeof(int));

                while(valorPipe2 != 4){
                    
                    valorPipe2 = read(fd3z[0], &valorLeido2, sizeof(int));        
                    
                }                  
                valorPipe2 = 0;

        
                free(valorLeido);
                free(dimensionNuevo);
                //se libera el espacio del contenido de matriz rectificada
                liberarMatriz(matrizRectificada, ((valorLeido[1])-2));
       
            } // termino del for

        } //su termino piola de su ELSE

        wait(NULL);
        close(4);
        close(8);
        close(fd3z[0]);
        close(fd3[1]);
       
        
        //printf("Fin Del proceso Rectificacion\n");
       return 0;      
}