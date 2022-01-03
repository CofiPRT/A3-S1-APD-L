import java.util.Arrays;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class Main {

	private static final int N = 100000;
	private static final int P = 4;

	private static final int[] values = new int[N];
	private static final int[] seqValues = new int[N];

	private static final CyclicBarrier barrier = new CyclicBarrier(P);

	public static void main(String[] args) {

		Thread[] threads = new Thread[P];

        for (int i = 0; i < N; i++) {
        	values[i] = (int) (Math.random() * N);
        	seqValues[i] = values[i];
        }

        // start timestamp
        long start = System.nanoTime();

        for (int i = 0; i < P; i++) {
            threads[i] = new MyThread(i);
            threads[i].start();
        }

        for (int i = 0; i < P; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        // end timestamp
        long end = System.nanoTime();
        System.out.println("Execution time (ns) = " + (end - start));

        Arrays.sort(seqValues);

        boolean correct = true;
        for (int i = 0; i < N; i++) {
        	if (values[i] != seqValues[i]) {
        		System.out.println("Wrong value at index " + i +
        				": expected " + seqValues[i] + ", found " + values[i]);
        		correct = false;
        	}
        }

        System.out.println(correct ? "Correct answer" : "Wrong answer");
	}

	static class MyThread extends Thread {
		private int id;

		public MyThread(int id) {
			this.id = id;
		}

		@Override
		public void run() {
			try {

				int start = id * (int) Math.ceil((double) N / P);
				int end = Math.min(N - 1, (id + 1) * (int) Math.ceil((double) N / P));

				int start_even, start_odd;
				if (start % 2 != 0) {
					start_even = start + 1;
					start_odd = start;
				} else {
					start_even = start;
					start_odd = start + 1;
				}

				for (int k = 0; k < N; k++) {

					// even phase
					for (int i = start_even; i < end; i += 2) {
						if (values[i] > values[i + 1]) {
							int temp = values[i];
							values[i] = values[i + 1];
							values[i + 1] = temp;
						}
					}

					// wait for all threads to finish the odd phase
					barrier.await();

					// odd phase
					for (int i = start_odd; i < end; i += 2) {
						if (values[i] > values[i + 1]) {
							int temp = values[i];
							values[i] = values[i + 1];
							values[i + 1] = temp;
						}
					}

					// wait for all threads to finish the odd phase
					barrier.await();

				}
			} catch (InterruptedException | BrokenBarrierException e) {
				e.printStackTrace();
			}

		}
	}

}
