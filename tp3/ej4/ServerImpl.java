/* Needed for implementing remote method/s */
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.io.FileOutputStream;

/* This class implements the interface with remote methods */
public class ServerImpl extends UnicastRemoteObject implements IServer 
{

  protected ServerImpl() throws RemoteException {
    super();
  }

  public DataBuffer read(String filename, int offset, int bytes_to_read) throws RemoteException {
    return new DataBuffer();
  }

  public int write(String filename, int bytes_to_write, byte[] buffer) throws RemoteException {

    //Ver acá por que no se puede saber cuanto se escribio realmente
    //https://stackoverflow.com/questions/33842174/get-number-of-bytes-a-dataoutputstream-wrapped-around-socket-getoutputstream-wro
    try {
      FileOutputStream output = new FileOutputStream(filename, true);
      output.write(buffer, 0, bytes_to_write);
      output.close();
    } catch (Exception e) {
      e.printStackTrace();
      return 0;
    }
    System.out.println(bytes_to_write);
    return bytes_to_write;
  }
}