# IA-VRPB-FC

UTFSM - Inteligencia Artificial 2020-2  
Francisco Abarca 201673552-6  

Implementación de VRPB mediante algoritmo FC+GBJ

# Instrucciones compilación

Usar el comando ```make```

# Instrucciones de uso

Para ejecutar el programa debe compilarse con el comando del punto anterior, la instancia a ocupar debe tener el formato ```<NOMBRE>.txt``` y estar dentro del directorio ```Instancias``` que se encunetra en la raiz del proyecto.  

Para ejecutar el programa realizamos ```make run file='<NOMBRE>'```, con la instancia debidamente ubicada en la carpeta mencionada. Ejemplo ```make run file='GF4'``` . 

Para obtener el tiempo de ejecución utilizar ```time make run file='instancia'```.

Para limpiar el directorio de los ejecutables se utiliza ```make clean```.  

Los resultados del programa se encuentran en el directorio ```Resultados``` ubicado en la raíz del proyecto.  

Para salir del programa en cualquier momento se realiza ```ctrl + c``` durante su ejecución.
