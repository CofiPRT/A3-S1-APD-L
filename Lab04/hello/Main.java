package hello;

import java.util.ArrayList;
import java.util.List;

public class Main {

	enum THREAD_IDENTIFIER {
		NAME,
		ID
	}

	// instantiation ID (NAME) or the unique auto-generated long-int ID (ID)?
	public static final THREAD_IDENTIFIER threadIdentifier = THREAD_IDENTIFIER.NAME;

	public static void main(String[] args) {

		int cores = Runtime.getRuntime().availableProcessors();

		if (cores < 1) {
			System.err.println("The number of cores is less than one! Possible causes:\n" +
								"\t1. Your processor is about to die\n" +
								"\t2. Your JVM has a serious bug in it\n" +
								"\t3. The universe is about to blow up\n"); // https://stackoverflow.com/a/4759606
			System.exit(-1);
		}

		Runnable task = new Runnable() {
			@Override
			public void run() {
				switch(threadIdentifier) {
				case NAME:
					System.out.println("Hello from thread " + Thread.currentThread().getName());
					break;
				case ID:
					System.out.println("Hello from thread #" + Thread.currentThread().getId());
					break;
				default:
					// how
				}

			}
		};

		List<Thread> threads = new ArrayList<>();

		for (int i = 0; i < cores; i++) {
			Thread thread = new Thread(task, "" + i);

			threads.add(thread);
			thread.start();
		}

		try {
			for (Thread thread : threads)
				thread.join();
		} catch (InterruptedException ex) {
			System.err.println("Could not join threads");
		}

	}

}
