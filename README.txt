gcc pipeline.c -o pipeline -lpng
gcc pconvolucion.c -o pconvolucion
gcc prectificacion.c -o prectificacion
gcc ppooling.c -o ppooling
gcc pclasificador.c -o pclasificador
gcc p_writing.c -o p_writing -lpng
./pipeline -c 2 -m filtro.txt -n 0.25 -p 25 -b

*NOTA: PARA HACER PRUEBA DE ESTE TIPO ./pipeline -c 2 -m filtro.txt -n 0.25 -p 25 -b
EN LO IDEAL MODIFICAR EL VALOR CONTENIDO EN EL MAKEFILE


-En la carpeta "src" se encuentra el repositorio "img" donde se cargan las imágenes. por tanto es allí donde ha de incluir otras nuevas.
-Los resultados son guardados en la carpeta principal donde reside el Makefile.
-Tambien los archivos fuentes de cada proceso se encuentra al acceder a "src"
-Las funciones que utilizan los procesos se encuentran en la carpeta "fun" y el archivo .h se encuentra en el repositorio head.

-en la terminal del repositorio al llamar el Makefile con escribir "make" es suficiente para el funcionamiento del programa por completo.
-para limpiar existe el comando "make clean"
-para otros casos "make clean1" para limpiar los objetos creados por gcc y "make clean2" para eliminar las imágenes resultantes.











