import jade.core.*;

public class FirstAgent extends Agent
{
  // Ejecutado por única vez en la creación
  public void setup()
  {
    Location origen = here();
    System.out.println("\n\nHola, agente con nombre local " + getLocalName());
    System.out.println("Y nombre completo... " + getName());
    System.out.println("Y en location " + origen.getID() + "\n\n");
  }
}