import jade.core.*;
import java.lang.management.*;

public class Ej1 extends Agent {
  // Ejecutado por única vez en la creación
  String[] containers = new String[4];
  String[] listaStrings = new String[4];
  double[] loads = new double[4];
  int idx = 0;
  long startTime = System.currentTimeMillis();

  private void println(Object s){ System.out.println(s); }

  public void setup() {
    //{"Container-1", "Container-2", "Main-Container"}
    containers[0] = "Container-1";
    containers[1] = "Container-2";
    containers[2] = "Main-Container";
    containers[3] = this.here().getName();
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
    if (!this.here().getName().equals(containers[3])) {
      idx++;
      moveNext();
      return;
    }
    println("Container\tCarga\tMemoria disponible\t");
    for(String s : listaStrings) {
      println(s);
    }
    Long elapsed = System.currentTimeMillis() - startTime;
    println("Tarde " + elapsed.toString());
  }

  void moveNext() {
    try {
      ContainerID destino = new ContainerID(containers[idx], null);
      this.doMove(destino);
    } catch (Exception e) {
      
    }
  }

}