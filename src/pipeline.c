#include "head/test.h"
#include "fun/estructura_png.c"
#include "fun/funcionrecorreMatrix.c"
#include "fun/crearnombre.c"


/*
   Entradas:
   Funcionamiento:
   Salidas:
*/


int main(int argc, char *argv[]){
   
    int opt;
    int c=0;
    char* m=NULL;
    float n=0;
    int p=0;
    int b=-1;
    char* argumentos[6];    
    //argumentos[4]=b;
    //c=atoi(argv[0]);

   /*
    Entradas:       nombreArchivo *Flag(-c) Numero_de_imagenes *Flag(-m) Nombre_del_filtro.txt *Flag(-n) Valor_decimal_umbral 
                    *Flag(-p) Valor_porcentaje_de_clasificacion 
                    *Flag(-b):opcional; de aparecer el flag -b muestra por pantalla parametros resultante para fines del laboratorio.
                
    Funcionamiento: recepción de parámetros de entradas para sus correspondientes validación de datos para el correcto funcionamiento del programa.
   
    Salidas:        tipo void, modifica valores en variables locales y arreglos.
                    -Caso error retorna un 0 e imprime por pantalla dicho factor.
                    -Caso de funcionamiento correcto agrega valores verificador al arreglo 'argumentos'.
   */
   while((opt=getopt(argc, argv, "c:m:n:p:b"))!= -1){
       switch (opt){
            case 'c':
                        c=atoi(optarg);
                        if(c<=0){
                            printf("Cantidad de las imagenes no pueden ser menor o igual a cero\n");
                            return 0;
                        }
                        argumentos[0]=optarg;
                        break;
            case 'm':
                        m= optarg;
                        int largoNombre;
                        largoNombre=strlen(m);
                        //printf("El largo de nombre: %d\n",largoNombre);
                        if(largoNombre<6){
                            printf("Nombre de longitud incorrecta\n");
                            return 0;
                        }
                        char t=m[largoNombre-1];
                        char x=m[largoNombre-2];
                        char t1=m[largoNombre-3];
                        char punto=m[largoNombre-4];
                        if(punto!='.' || t!='t' || x!='x' || t1!='t'){
                            printf("El achivo debe ser de extensión .txt\n");
                            return 0;                          
                        }
                        argumentos[1]=m;
                        break;
            case 'n':
                        n=atof(optarg);
                        if(n>=0.1 && n<1){
                             //printf("el valor de n es: %f\n",n);
                            argumentos[2]=optarg;
                            break;
                        }
                        else{
                            printf("%f",n);
                            printf("Valor incorrecto para umbral\n");
                            return 0;
                        }
            case 'p':
                        p=atoi(optarg);
                        if(p>=15 && p<100){
                            //printf("el valor de p es: %f\n",p);
                            argumentos[3]=optarg;
                            break;
                        }
                        else{
                            printf("%d",p);
                            printf("los valores del porcentaje de clasificacion han de ser entre 0.5 y 0.99\n");
                            return 0;
                        }
            case 'b':
                         argumentos[4]="1";
                         b=1;
                         break;
            case '?':
                    printf("Valor no reconocico\n");
                    printf("El formato a ingresar es: main.c -c N_imagenes -m filtro.txt -n umbral -p porciento_clasificacion -b\n");
                    return 0;
                        
       }
   }
    //se indica para excev el fin de los argumentos 
    argumentos[5]=NULL; 

   //se crea el pipe con la etapa convulución
   
    int fd[2];
    pipe(fd);

    int fdz[2];
    pipe(fdz);

    if(b==1){
    printf("|  \timagen\t\t|\tnearly black\t|\n");
    printf("|-----------------------|-----------------------|\n");
    }
   
   //variable de tipo entero para el llamado de un fork() 
   int pid;
   pid = fork();

    //cuyo caso de ser hijo el valor pid==0
   if(pid == 0)
		{

          //  printf("EtapaConvolucion\n");
			close(fd[1]); //ciero el pipe como escritura
			close(fdz[0]);//cierro el pipe como lectura
            
            
            dup2(fd[0], 3); //especifico la lectura
            dup2(fdz[1], 6); //especifico la escritura
			
            execv("src/pconvolucion", argumentos); //llamo a execv para cargar el código de la etapa convolución

			printf("Si esto es visible es porque no se a cargado el codig de execve_convolucion\n");
			return 0;
		}
		else{


            
            //Cuyo caso es el padre
            close(fd[0]); //ciero el pipe como lectura
            close(fdz[1]);//cierro el pipe como escritura
            //ciclo for por cada imagen cargada
            for(int ciclo=1;ciclo<=c;ciclo++){
                
                    // printf("For MAIN %d\n",ciclo);
                    char* nombre=nombre_imagen(ciclo);
                    //printf("For MAIN %d\n",ciclo);
                    //char* nombre="foto3.png";

                    //se crea una estructura tipo MatrizPNG y es cargada a esta la imagen png
                    MatrizPNG* matrizPng = readImage(nombre);
                    
                    //reservo espacio en memoria para guardar las dimensiones de la imagen cargada
                    int* dimensiones= (int*)malloc(sizeof(int*)*2);
                    dimensiones[0]=matrizPng->width; //ancho 0
                    dimensiones[1]=matrizPng->height;//alto 1

//                    printf("Ancho %d\n", dimensiones[0]);
  //                  printf("alto %d\n", dimensiones[1]);

                    
                    //se crea una variable doble puntero para albergar la dirección de la matriz o imagen png
                    int** matrixPNG;
                    matrixPNG = matrizPng->image;


                    if(ciclo==1){
                        //printf("PADRE IF \n");
                        //se envía las dimensiones por el pipe
                        write(fd[1],dimensiones, sizeof(dimensiones));

                        //se envía por el pipe la imagen png
                        loadImagen(dimensiones,matrixPNG,fd[1]);


                }else{
                    
                        int valorPipe2 = 5;
                        int valorLeido = 0;

                        while(valorPipe2 != 4){
                            //printf("PADRE ELSE- VALORPIPE2 READ  %d\n", valorPipe2);
                            valorPipe2 = read(fdz[0], &valorLeido, sizeof(int));        
                            //printf("desde el padre, EL VALOR LEIDO ENVIADO POR HIJO ES: %d\n",valorLeido);
                            //printf("El valor READ l188 en el padre es: %d\n", valorPipe2);
                        }

                        
                        valorPipe2 = 0;
                        
                        
                        //se envía las dimensiones por el pipe
                        write(fd[1],dimensiones, sizeof(dimensiones));

                        //se envía por el pipe la imagen png
                        loadImagen(dimensiones,matrixPNG,fd[1]);

                }

            
            int valorPipe2 = 5;
            int valorLeido = 0;
            int valorZ=ciclo;

            write(fd[1],&valorZ,sizeof(int));
    
            while(valorPipe2 != 4){
                
                valorPipe2 = read(fdz[0], &valorLeido, sizeof(int));        
                   
            }
    
 
            valorPipe2 = 0;
                      

            free(dimensiones);
            }//fin del ciclo for
          
          
          } //else del fork
    
        close(fd[1]);
        close(fdz[1]);
        wait(NULL);
        printf("Soy el padre y he finalizado\n");

         
    return 0;
}