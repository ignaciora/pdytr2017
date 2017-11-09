import java.io.Serializable;

public class DataBuffer implements Serializable {

  private static final long serialVersionUID = 7526471155622776147L;

  public DataBuffer(int buffer_size) {
    this.buffer = new byte[buffer_size];
  }

  public byte[] buffer;
  public int buffer_len;

}