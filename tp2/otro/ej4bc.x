typedef opaque archivo<>;
typedef string nombre<30>;

program EJ4 {
   version EJ4_VERSION {
     archivo leer(nombre, int, int) = 1;
     int escribir(nombre, archivo, int) = 2;
     int cantbytes(nombre) = 3;
   } = 1;
} = 777777777;

