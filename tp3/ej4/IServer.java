/* Needed for defining remote method/s */
import java.rmi.Remote;
import java.rmi.RemoteException;

/* This interface will need an implementing class */
public interface IServer extends Remote 
{
  public DataBuffer read(String filename, int offset, int bytes_to_read) throws RemoteException;

  public int write(String filename, int bytes_to_write, byte[] buffer) throws RemoteException;
  
}