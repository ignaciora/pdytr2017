/* Needed for implementing remote method/s */
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.io.FileOutputStream;
import java.io.RandomAccessFile;

/* This class implements the interface with remote methods */
public class ServerImpl extends UnicastRemoteObject implements IServer 
{

  protected ServerImpl() throws RemoteException {
    super();
  }

  public DataBuffer read(String filename, int offset, int bytes_to_read) throws RemoteException {

    DataBuffer db = new DataBuffer(bytes_to_read);

    try {
      RandomAccessFile raf = new RandomAccessFile(filename, "r");
  
      raf.seek(offset);
      db.buffer_len = raf.read(db.buffer);
  
      raf.close();
    } catch(Exception e) {
      System.out.println(e);
    }

    return db;
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
    
    return bytes_to_write;
  }
}