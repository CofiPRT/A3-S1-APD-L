package doubleVectorElements;

import java.util.ArrayList;
import java.util.List;

/**
 * @author cristian.chilipirea
 *
 */
public class Main {

	static class MyThread extends Thread {

		private int startIndex;
		private int endIndex;
		private int[] v;

		public MyThread(int startIndex, int endIndex, int[] v) {
			this.startIndex = startIndex;
			this.endIndex = endIndex;
			this.v = v;
		}

		@Override
		public void run() {
			for (int i = startIndex; i < endIndex; i++)
				v[i] *= 2;
		}

	}

	public static void main(String[] args) {
		int N = 100000013;
		int v[] = new int[N];

		for(int i=0;i<N;i++)
			v[i]=i;

		int cores = Runtime.getRuntime().availableProcessors();

		List<Thread> threads = new ArrayList<>();

		for (int i = 0; i < cores; i++) {

			int startIndex = i * N / cores;
			int endIndex = Math.min((i + 1) * N / cores, N);

			Thread thread = new MyThread(startIndex, endIndex, v);

			threads.add(thread);
			thread.start();

		}

		try {
			for (Thread thread: threads)
				thread.join();
		} catch (InterruptedException e) {
			System.err.println("Could not join threads");
			e.printStackTrace();
		}

		for (int i = 0; i < N; i++) {
			if(v[i]!= i*2) {
				System.out.println("Wrong answer");
				System.exit(1);
			}
		}
		System.out.println("Correct");
	}

}
