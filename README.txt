Renderizar imágenes astronómicas de formato png para obtener por medio de los parámetros ingresados si dicha imágenes son nearly-black o no.

Función:
El programa pipeline.c crea un proceso hijo y el padre carga la imágenes .png y la binariza en una matriz, dicha información la envía por medio de un pipe al proceso hijo. El hijo crea un proceso hijo, el hijo que ahora es padre lee la información recibida por su padre y una vez leída la matriz con la imagen binarizada procesa dicha información en función a la tarea a realizar como proceso, luego, se la envía a su hijo y una vez que su hijo leyó dicha información le notifica a éste padre que a finalizado con la lectura, entonces éste proceso padre le notifica a su padre también que él a terminado y está listo para leer nuevamente la nueva imagen png. 
Este proceso se va repitiendo hasta el último proceso hasta que se completen todas la imágenes a renderizar y el main finaliza junto a todos los demás procesos. 



PRIMERO ES REQUERIDO INSTALAR EN EL SISTEMA LO SIGUIENTE:   "sudo apt-get install libpng-dev pkg-config"


 -c: cantidad de imágenes
 -m: Máscara del filtro que se le aplicará a la imagen en la fase de convolución. Consiste en un archivo .txt
 -n: UMBRAL para clasificación (decimal)
 -p: porcentaje de clasificación (%)
 -b: bandera que indica si se deben mostrar los resultados por pantalla, es decir, la conclusión obtenida al leer la imagen binarizada.

*NOTA: PARA HACER PRUEBA DE ESTE TIPO ./src/pipeline -c 2 -m filtro.txt -n 0.25 -p 25 -b
EN LO IDEAL MODIFICAR EL VALOR CONTENIDO EN EL MAKEFILE


-En la carpeta "src" se encuentra el repositorio "img" donde se cargan las imágenes. por tanto es allí donde ha de incluir otras nuevas.

*Los resultados son guardados en la carpeta principal donde reside el Makefile.

-Tambien los archivos fuentes de cada proceso se encuentra al acceder a "src"
-Las funciones que utilizan los procesos se encuentran en la carpeta "fun" y el archivo .h se encuentra en el repositorio head.

*en la terminal del repositorio al llamar el Makefile con escribir "make" es suficiente para el funcionamiento del programa por completo.
-para limpiar existe el comando "make clean"
-para otros casos "make clean1" para limpiar los objetos creados por gcc y "make clean2" para eliminar las imágenes resultantes.











