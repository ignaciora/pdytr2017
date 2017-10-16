import java.rmi.Naming; /* lookup */
import java.rmi.registry.Registry; /* REGISTRY_PORT */
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
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
    //writeToServer(dest_file, origin_file + "__copy", remote);
    writeToServer(origin_file, origin_file + "__copy", remote);
  }

  private static void writeToServer(String origin_file, String dest_file, IServer remote) {
    try {
      Path path = Paths.get(origin_file);
      byte[] buffer = Files.readAllBytes(path);
      
      int i = 0;
      do {
        int length = (buffer.length <= i + BUFFER_SIZE) ? buffer.length - i : i + BUFFER_SIZE;
        
        byte[] subbuffer = Arrays.copyOfRange(buffer, i, i+length);
        int res = remote.write(dest_file, subbuffer.length, subbuffer);
        i += res;
        //System.out.format("Listo, escritos %d bytes\n", res);
        System.out.format("%d\n", i);
      } while (i < buffer.length);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  private static void readFromServer(String origin_file, String dest_file, IServer remote) {

  }

}