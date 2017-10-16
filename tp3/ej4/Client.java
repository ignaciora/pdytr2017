import java.rmi.Naming; /* lookup */
import java.rmi.registry.Registry; /* REGISTRY_PORT */
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.Arrays;

public class Client 
{
  private static int BUFFER_SIZE = 1024;

  public static void main(String[] args) {
    
    if (args.length < 3) {
      System.out.println("Se necesitan 3 argumentos.");
      System.out.println("Modo de uso:");
      System.out.println("hostname archivo_origen archivo_destino");
      System.exit(1);
    }

    String origin_file = args[1];
    String dest_file = args[2];
    
    try {
      String rname = "//" + args[0] + ":" + Registry.REGISTRY_PORT + "/remote";
      IServer remote = (IServer) Naming.lookup(rname);
      copy(origin_file, dest_file, remote);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  private static void copy(String origin_file, String dest_file, IServer remote) {
    readFromServer(origin_file, dest_file, remote);
    writeToServer(dest_file, origin_file + "__copy", remote);
  }

  private static void writeToServer(String origin_file, String dest_file, IServer remote) {
    try {
      Path path = Paths.get(origin_file);
      FileInputStream fis = new FileInputStream(path.toFile());
      byte[] buffer = new byte[BUFFER_SIZE];

      int read = 0;

      while ((read = fis.read(buffer)) > 0) {
        int written = remote.write(dest_file, read, buffer);
        if (written == 0) {
          throw new Exception("No se pudieron escribir datos en el server");
        }
      }

      fis.close();
    } catch (Exception e) {
      System.out.println(e);
    }
  }

  private static void readFromServer(String origin_file, String dest_file, IServer remote) {
    try {
      FileOutputStream fos = new FileOutputStream(dest_file, true);
      DataBuffer db = new DataBuffer(BUFFER_SIZE);

      boolean read = true;
      int total_read = 0;

      while (read) {
        db = remote.read(origin_file, total_read, BUFFER_SIZE);
        total_read += db.buffer_len;
        if (db.buffer_len > 0) {
          fos.write(db.buffer, 0, db.buffer_len);
        } else {
          read = false;
        }
      }

      fos.close();
    } catch (Exception e) {
      System.out.println(e);
    }
  }

}