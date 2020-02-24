#include "head/test.h"
#include "fun/pasabajo.c"
#include "fun/convolucion.c"
#include "fun/funcionrecorreMatrix.c"



int main(int argc, char *argv[]){


    int opt;
    int c=atoi(argv[0]);
    char* m=argv[1];
    float n=0;
    int p=0;
    int b=-1;

    int fd2[2]; 
    int fd2z[2];
    //Se crea el pipe para la etapa rectificación
    
    pipe(fd2);
    pipe(fd2z);
    int pid2;    
    pid2 = fork();
    
    if(pid2 == 0){

 //printf("EtapaRectificacion\n");
			
			close(fd2[1]);              //cierro el pipe como escritura
			close(fd2z[0]);             //cierro el pipe como lectura
            dup2(fd2[0], 4); //especifico en el descriptor lectura
            dup2(fd2z[1], 8); //especifico la escritura
				
			execv("src/prectificacion", argv);       //cargo el código de la etapa rectificación

			printf("Si esto es visible es porque no se a cargado el codig de execve_Rectificacion\n");
			return 0;
   }else{

        //printf("EtapaConvolucion\n");
        

        close(fd2[0]);                      //cierro el pipe como lectura
        close(fd2z[1]);                     //cierro el pipe como escritura


        /////////////////////////////////////////////////////
        /////////////////// La Magia ////////////////////////
        /////////////////////////////////////////////////////


        //Cuyo caso es el padre
        int i,j,comprueba;
            for(int ciclo=1;ciclo<=c;ciclo++){   //ciclo for para cada convolución de imagenes cargadas
            //printf("For Convolucion numero %d\n",ciclo);
            int valorLeido[2];
            int valorPipe0=0;
            
            if(ciclo==1){

                    //leo las dimensiones de la imagen cargada desde main
                    valorPipe0=0;
                    //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                    while(valorPipe0 != 8){

                        valorPipe0 = read(3,valorLeido,sizeof(valorLeido));        
                        
                    }

                    valorPipe0=0;


            }else{
                        //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                        write(6,&ciclo,sizeof(int));
                        
                        valorPipe0=0;
                        //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
                        while(valorPipe0 != 8){

                                valorPipe0 = read(3,valorLeido,sizeof(valorLeido));      
                        
                        }

                        valorPipe0=0;
            }


        
            int** matrix=malloc(((valorLeido[1]))*sizeof(int*));   
        
            for(i=0;i<(valorLeido[1]);i++){

                matrix[i]=malloc(((valorLeido[0]))*sizeof(int*));

            }
            
            ////////////////////////////////////////////////////////////
            /////// CAPTURA LA IMAGEN BINARIZADA DESDE EL PADRE ////////
            ////////////////////////////////////////////////////////////    

        
            //Se lee desde el pipe el valor de cada pixel como imagen PNG
            for(i=0; i<valorLeido[1]; i++){
            //for(i=0; i<1080; i++){
                for(j=0; j<valorLeido[0];j++){
                    comprueba=read(3, &matrix[i][j],sizeof(int));
                }
                
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //AQUÍ DEBE COMUNICAR CON PIPELINE PARA INDICAR EL FIN DEL CICLO DE LA IMAGEN ENVIADA POR PIPE DESDE PADRE//
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //ESTRUCTURA DE CONTROL


            int valorX=c;
            while(valorPipe0 != 4){
                valorPipe0= read(3,&valorX,sizeof(int));
            }
            valorPipe0=0;

            write(6,&valorX,sizeof(int));

            ////////////////////////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////////////////////////
            
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            ////////////////DESDE AQUÍ SE PASA AL TRABAJO PROPIO DEL PROCESO - CONVOLUCIÓN DE LA IMAGEN BINARIZADA///////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
        
            /////////////////////////////////////////////////////////
            ////////////// PREPARO LA ETAPA CONVOLUCIÓN /////////////   
            /////////////////////////////////////////////////////////   
            //ESTAS SON LAS ETAPAS QUE SON PROPIAS DEL PROCESO
            //LAS DEMÁS ETAPAS SON SÓLO DE COMUNICACIÓN
            

            int** pasabajo;         //se crea un doble puntero para la matriz 3x3
            pasabajo=leerFichero(m);     //es cargada la matriz


            //reserva de espacio para el contenedor de la etapa convolucion
            float** nuevaMatriz=malloc(((valorLeido[1])-2)*(sizeof(float*)));


            for(i=0;i<((valorLeido[1])-2);i++){
                
                nuevaMatriz[i]=malloc(((valorLeido[0])-2)*(sizeof(float*)));

            }
            
            int ValordeRetorno=convolucion(matrix, valorLeido, nuevaMatriz, pasabajo);
            
            

            ///////////////////////////////////Aquí comienzan los parámetros que
            ///////////////////////////////////Indican al proceso hijo la comunicacion de datos y envíos de los mismos

            if(ciclo==1){

                int numeroRetorno = write(fd2[1],valorLeido, sizeof(valorLeido));
                
                for(i=0;i<valorLeido[1]-3;i++){
                    for(j=0;j<valorLeido[0]-3;j++){
                    
                    int numeroRetorno2 = write(fd2[1], &nuevaMatriz[i][j], sizeof(float));
                    
                    }
                }
            }
            else{  
                    int valorPipe2 = 5;
                    int valorLeido2 = 0;
                    //ESTRUCTURA DE CONTROL O  SEÑALES PARA INICIAR COMUNICACIÓN DE LOS PROCESOS PADRE E HIJO                    
                    while(valorPipe2 != 4){
                    
                        valorPipe2 = read(fd2z[0], &valorLeido2, sizeof(int));        
                        
                    }
                    valorPipe2 = 0;
                    
                    //escribo por el pipe las dimensiones de la matriz
                    int numeroRetorno = write(fd2[1],valorLeido, sizeof(valorLeido));
                    
                    for(i=0;i<valorLeido[1]-3;i++){
                            for(j=0;j<valorLeido[0]-3;j++){
                                int numeroRetorno2 = write(fd2[1], &nuevaMatriz[i][j], sizeof(float));
                            }
                    }
                            
                }

            /////////////////////////////////////////////////////////////////////////////////////////////////
            ///AQUÍ COMUNICA CON RECTIFICACIÓN PARA INDICAR EL FIN DEL CICLO DE LA IMAGEN ENVIADA POR PIPE///
            /////////////////////////////////////////////////////////////////////////////////////////////////

            int valorPipe2 = 0;
            int valorLeido2 = -1;
            //int valorZ=ciclo;
            int valorZ=ciclo;
            

            //ESTRUCTURA DE CONTROL O DE SEÑALES PARA LA COMUNICACIÓN BIDIRECCIONAL DE LOS PROCESOS PADRE E HIJO
            //PADRE ENVÍA UNA SEÑAL PARA QUE HIJO LE RESPONDA, SIGNIFICANDO EL NUEVO CICLO
            write(fd2[1],&valorZ,sizeof(int));

            while(valorPipe2 != 4){
                
                valorPipe2 = read(fd2z[0], &valorLeido2, sizeof(int));        
            
            }
            
            valorPipe2 = 0;
                


            // free(valorLeido);     //no se encuenta reservada con malloc
            free(pasabajo);         //se libera la matriz del filtro 3x3

            liberarMatrix(matrix, (valorLeido[1]));             //Se libera el espacio para el valor de la imagen png
            liberarMatriz(nuevaMatriz, ((valorLeido[1])-2));    //Se libera el espacio para el valor de la imagen convolucionada


            } //término del ciclo for

                  


       }//termino el else

        wait(NULL);
        close(3);
        close(6);
        close(fd2z[0]);
        close(fd2[1]);
        
    //        printf("Fin Del proceso convolución\n");
    
        return 0;
}
