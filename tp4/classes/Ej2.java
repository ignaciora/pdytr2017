import jade.core.*;
import java.lang.management.*;
import java.io.*;
import java.util.*;

public class Ej2 extends Agent {
  // Ejecutado por única vez en la creación
  String archivo = "";
  String containerOrigen = "";
  int sumaTotal = 0;

  private void println(String s) {
    System.out.println("\n=======================================================================");
    System.out.println(s);
    System.out.println("==========================================================================\n");
  }

  public void setup() {
    Object[] args = getArguments();
    if (args == null || (args != null && args.length != 2)) {
      println("Este agente requiere de 2 parametros: Container destino y nombre de archivo");
      try {
        this.getContainerController().kill();
      } catch (Exception e) {

      }
      return;
    }
    String targetContainer = args[0].toString();
    archivo = args[1].toString();
    containerOrigen = this.here().getName();

    moveTo(targetContainer);
  }

  // Ejecutado al llegar a un contenedor como resultado de una migración
  protected void afterMove() {
    if (this.here().getName().equals(containerOrigen)) {
      //Si estoy en el container de origen, muestro la suma
      println("La suma total del archivo fue de: " + Integer.toString(sumaTotal));
    } else {
      //Si estoy en el container destino, sumo del archivo
      try {
        Scanner scanner = new Scanner(new File(this.archivo));
        ArrayList<Integer> list = new ArrayList<Integer>();
        while(scanner.hasNextInt()) {
          list.add(scanner.nextInt());
        }
        for(Integer v : list)
          sumaTotal += v;
        
      } catch (Exception e) {
        e.printStackTrace();
      }
      //Al terminar de sumar, vuelvo al origen para reportar
      moveTo(this.containerOrigen);
    }
  }

  void moveTo(String container) {
    try {
      ContainerID destino = new ContainerID(container, null);
      this.doMove(destino);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

}