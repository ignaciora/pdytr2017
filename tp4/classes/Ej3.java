import jade.core.*;
import java.io.*;
import java.util.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;

public class Ej3 extends Agent {
  private static int BUFFER_SIZE = 1024;

  String nombreArchivo = "";
  String origen = "";
  String destino = "";
  String operacion = "";

  DataBuffer dataBuffer = new DataBuffer(BUFFER_SIZE);
  int totalLeido = 0;


  public void setup() {
    Object[] args = getArguments();

    if (args == null || (args != null && args.length != 3)) {
      println("Este agente requiere de 3 parametros: container_destino (read|write) archivo");
      try {
        this.getContainerController().kill();
      } catch (Exception e) {}
      return;
    }
    destino = args[0].toString();
    operacion = args[1].toString();
    nombreArchivo = args[2].toString();
    origen = this.here().getName();

    moveTo(destino);
  }


  protected void afterMove() {
    switch (operacion) {
      case "read":
        try {
          if (this.here().getName().equals(destino)) { //Si esta en el destino (source)
            dataBuffer = leer(nombreArchivo, totalLeido, BUFFER_SIZE);
            totalLeido += dataBuffer.buffer_len;
            moveTo(origen);
          }
          else { //Si esta en el origen
            FileOutputStream fos = new FileOutputStream(nombreArchivo, true);

            if (dataBuffer.buffer_len > 0) {
              fos.write(dataBuffer.buffer, 0, dataBuffer.buffer_len);
            }
            else {
              println("Finalizo la lectura del archivo");
              return;
            }

            fos.close();
            moveTo(destino);
          }
        } catch (Exception e) {
            System.out.println(e);
        }
        break;

      case "write":
        //
        break;

      default: //Podria ser el COPY
        break;
    }
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


  private class DataBuffer {
    public byte[] buffer;
    public int buffer_len;

    public DataBuffer(int buffer_size) {
      this.buffer = new byte[buffer_size];
      this.buffer_len = 0;
    }
  }
}
