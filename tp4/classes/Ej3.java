import jade.core.*;
import java.io.*;
import java.util.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;

public class Ej3 extends Agent {
  private static int BUFFER_SIZE = 1024;

  String nombreArchivo = "";
  String nombreArchivoLocal = "";
  String nombreCopiaServer = "";
  String origen = "";
  String destino = "";
  int estadoCopia = 0;

  DataBuffer dataBuffer = new DataBuffer(BUFFER_SIZE);
  int totalLeido = 0;

  public void setup() {
    Object[] args = getArguments();

    if (args == null || (args != null && args.length != 3)) {
      println("Este agente requiere de 3 parametros: container_destino archivo_servidor archivo_local");
      try {
        this.getContainerController().kill();
      } catch (Exception e) {}
      return;
    }
    destino = args[0].toString();
    nombreArchivo = args[1].toString();
    nombreArchivoLocal = args[2].toString();
    nombreCopiaServer = nombreArchivo + "_copy";
    origen = this.here().getName();
    moveTo(destino);
  }


  protected void afterMove() {

    if (estadoCopia == 1) {
      this.prepararCopiado();
      moveTo(destino);
    }

    try {
      if (this.here().getName().equals(destino)) { //Si esta en el destino (source)
        dataBuffer = leer(nombreArchivo, totalLeido, BUFFER_SIZE);
        totalLeido += dataBuffer.buffer_len;
        moveTo(origen);
      } else { //Si esta en el origen
        FileOutputStream fos = new FileOutputStream(nombreArchivoLocal, true);

        if (dataBuffer.buffer_len > 0) {
          fos.write(dataBuffer.buffer, 0, dataBuffer.buffer_len);
        }
        else {
          println("Finalizo el copiado del archivo al origen");
          //Cuando finaliza esto, hay que pasar al copiado
          estadoCopia++;
          if(estadoCopia > 1) {
            return;
          }
          
          moveTo(destino);
        }

        fos.close();
        moveTo(destino);
      }
    } catch (Exception e) {
        System.out.println(e);
    }
  }


  void prepararCopiado() {
    nombreArchivo = nombreArchivoLocal;
    nombreArchivoLocal = nombreCopiaServer;
    String temp = origen;
    origen = destino;
    destino = temp;
    dataBuffer = new DataBuffer(BUFFER_SIZE);
    totalLeido = 0;
    println(nombreArchivo + nombreArchivoLocal + origen + destino);
  }


  void moveTo(String container) {
    try {
      this.doMove(new ContainerID(container, null));
    } catch (Exception e) {}
  }


  DataBuffer leer(String filename, int offset, int bytes_to_read) {
    DataBuffer db = new DataBuffer(bytes_to_read);
    
    try {
      RandomAccessFile raf = new RandomAccessFile(filename, "r"); //concatenar origen como carpeta
      raf.seek(offset);
      db.buffer_len = raf.read(db.buffer);
      raf.close();
    } catch(Exception e) {}
    
    return db;
  }
  

  int escribir(String filename, int bytes_to_write, byte[] buffer) {
    try {
      FileOutputStream output = new FileOutputStream(filename, true); //concatenar origen como carpeta
      output.write(buffer, 0, bytes_to_write);
      output.close();
    } catch (Exception e) {
        return 0;
    }
        
    return bytes_to_write;
  }



  private void println(String s) {
    System.out.println("\n=======================================================================");
    System.out.println(s);
    System.out.println("==========================================================================\n");
  }

}
