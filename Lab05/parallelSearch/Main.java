package parallelSearch;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CyclicBarrier;

public class Main {
    public final static int P = 8;
    public final static int SIZE = 100;
    public static int[] v = new int[SIZE];

    public final static int targetElement = 198;
    public static int positionElement = -1;
    public static volatile boolean foundElement = false;

    public static int left = 0;
    public static int right = SIZE - 1;

    public static CyclicBarrier barrier = new CyclicBarrier(P);

    public static void main(String[] args) {
        for (int i = 0; i < SIZE; i++) {
            v[i] = i * 2;
        }

        List<Thread> threads = new ArrayList<>();

        for (int id = 0; id < P; id++) {
			Thread thread = new ParallelSearch(id);
			threads.add(thread);
			thread.start();
        }

		for (Thread thread : threads) {
			try {
				thread.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

        if (foundElement) {
            System.out.println("Found position = " + Main.positionElement);
        } else {
            System.out.println("Element not found");
        }
    }
}
