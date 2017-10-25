/*
 * AskRemote.java
 * a) Looks up for the remote object
 * b) "Makes" the RMI
 */
import java.rmi.Naming;
/* lookup */
import java.rmi.registry.Registry;
/* REGISTRY_PORT */
public class AskRemote {
	public static void main(String[] args) {
		/* Look for hostname and msg length in the command line */
		if (args.length != 1) {
			System.out.println("1 argument needed: (remote) hostname");
			System.exit(1);
		}
		try {
			String rname = "//" + args[0] + ":" + Registry.REGISTRY_PORT + "/remote";
			IfaceRemoteClass remote = (IfaceRemoteClass) Naming.lookup(rname);
			int bufferlength = 1;
			byte[] buffer = new byte[bufferlength];
			long startTime = System.currentTimeMillis();
			remote.sendThisBack(buffer);
			long elapsed = System.currentTimeMillis() - startTime;
			System.out.println(elapsed);
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
}