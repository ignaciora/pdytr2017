import jade.core.*;
import java.lang.management.*;

public class Ej1 extends Agent {
  String[] containers = new String[4];
  String[] listaStrings = {"","","",""};
  int idx = 0;
  long timeTick = System.currentTimeMillis();

  private void println(Object s){ System.out.println(s); }

  public void setup() {
    containers[0] = "Container-1";
    containers[1] = "Container-2";
    containers[2] = this.here().getName();
    containers[3] = "Main-Container";
    
    println("\n\nLista de containers");
    for(String s : containers) {
      println(s);
    }
    this.moveNext();
  }

  // Ejecutado al llegar a un contenedor como resultado de una migración
  protected void afterMove() {
    //Concatenar los datos en un string y meterlo en un array segun el idx
    String carga = Double.toString(ManagementFactory.getOperatingSystemMXBean().getSystemLoadAverage());
    String memoria = Double.toString(java.lang.Runtime.getRuntime().totalMemory());
    String cnt = this.here().getName();
    listaStrings[idx] = cnt+"\t"+carga+"\t"+memoria;
    
    idx++;
    if (this.here().getName().equals(containers[3])) {
      println("Container\tCarga\tMemoria disponible\t");
      for(String s : listaStrings) {
        println(s);
      }
      Long elapsed = System.currentTimeMillis() - timeTick;
      println("Tiempo transcurrido: " + elapsed.toString());
      timeTick = System.currentTimeMillis();
      idx = 0;
    }
    moveNext();
  }

  void moveNext() {
    try {
      this.doMove(new ContainerID(containers[idx], null));
    } catch (Exception e) {}
  }
}
