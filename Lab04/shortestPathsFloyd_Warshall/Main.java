package shortestPathsFloyd_Warshall;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

/**
 * @author cristian.chilipirea
 *
 */
public class Main {

	static class MyThread extends Thread {

		private CyclicBarrier barrier;
		private int startIndex;
		private int endIndex;
		private int[][] graph;

		public MyThread(CyclicBarrier barrier, int startIndex, int endIndex, int[][] graph) {
			this.barrier = barrier;
			this.startIndex = startIndex;
			this.endIndex = endIndex;
			this.graph = graph;
		}

		@Override
		public void run() {

			int vertices = graph.length;

			for (int k = 0; k < vertices; k++) {

				for (int i = 0; i < vertices; i++)
					for (int j = startIndex; j < endIndex; j++)
						graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);

				try {
					barrier.await();
				} catch (InterruptedException | BrokenBarrierException e) {
					e.printStackTrace();
				}

			}
		}

	}

	public static void main(String[] args) {
		int M = 9;

		int seqGraph[][] = { { 0, 1, M, M, M },
		          			 { 1, 0, 1, M, M },
		          			 { M, 1, 0, 1, 1 },
		          			 { M, M, 1, 0, M },
		          			 { M, M, 1, M, 0 } };

		int graph[][] = { { 0, 1, M, M, M },
				          { 1, 0, 1, M, M },
				          { M, 1, 0, 1, 1 },
				          { M, M, 1, 0, M },
				          { M, M, 1, M, 0 } };

		// Parallelize me (You might want to keep the original code in order to compare)
		for (int k = 0; k < 5; k++) {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					seqGraph[i][j] = Math.min(seqGraph[i][k] + seqGraph[k][j], seqGraph[i][j]);
				}
			}
		}

		int cores = Runtime.getRuntime().availableProcessors();

		List<Thread> threads = new ArrayList<>();

		CyclicBarrier barrier = new CyclicBarrier(cores);

		for (int i = 0; i < cores; i++) {

			int startIndex = i * 5 / cores;
			int endIndex = Math.min((i + 1) * 5 / cores, 5);

			Thread thread = new MyThread(barrier, startIndex, endIndex, graph);

			threads.add(thread);
			thread.start();

		}

		System.out.println("Sequential graph:");
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				System.out.print(seqGraph[i][j] + " ");
			}
			System.out.println();
		}

		System.out.println("Parallel graph:");
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				System.out.print(graph[i][j] + " ");
			}
			System.out.println();
		}

		boolean isCorrect = true;

		for (int i = 0; i < 5; i++) {

			for (int j = 0; j < 5; j++) {
				if (seqGraph[i][j] != graph[i][j]) {
					isCorrect = false;
					break;
				}
			}

			if (!isCorrect)
				break;

		}

		System.out.println(isCorrect ? "Correct" : "Wrong");
	}
}
