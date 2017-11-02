import jade.core.*;

public class Ej1 extends Agent {
  // Ejecutado por única vez en la creación
  String[] containers = new String[4];
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
    println("Entrando a "+this.here().getName());
    if (this.here().getName() != containers[3]) {
      moveNext();
    } 
    Long elapsed = System.currentTimeMillis() - startTime;
    println("Tarde " + elapsed.toString());
  }

  void moveNext() {
    try {
      ContainerID destino = new ContainerID(containers[idx], null);
      idx++;
      System.out.println("Migrando el agente a " + destino.getID());
      this.doMove(destino);
    } catch (Exception e) {
      
    }
  }

}