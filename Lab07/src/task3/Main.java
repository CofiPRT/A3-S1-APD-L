package task3;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

import javafx.util.Pair;

public class Main {

	private static int N = 4;
	private static int THREADS = 4;

	private static AtomicInteger taskCount = new AtomicInteger(0);
	private static ExecutorService tpe = Executors.newFixedThreadPool(THREADS);

	private static Set<List<Pair<Integer, Integer>>> sequentialSolution = ConcurrentHashMap.newKeySet();
	private static Set<List<Pair<Integer, Integer>>> parallelSolution = ConcurrentHashMap.newKeySet();

    private static boolean check(int[] arr, int step) {
        for (int i = 0; i <= step; i++) {
            for (int j = i + 1; j <= step; j++) {
                if (arr[i] == arr[j] || arr[i] + i == arr[j] + j || arr[i] + j == arr[j] + i)
                    return false;
            }
        }
        return true;
    }

    private static void saveSolution(int[] sol, Set<List<Pair<Integer, Integer>>> solutionSet) {
    	List<Pair<Integer, Integer>> newSolution = new ArrayList<>();

        for (int i = 0; i < sol.length; i++)
        	newSolution.add(new Pair<>(sol[i] + 1, i + 1));

        solutionSet.add(newSolution);
    }

    public static void queens(int[] graph, int step) {
        if (Main.N == step) {
        	saveSolution(graph, sequentialSolution);
            return;
        }
        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;

            if (check(newGraph, step)) {
                queens(newGraph, step + 1);
            }
        }
    }

    public static void main(String[] args) {
    	System.out.println("Starting sequential implementation...");

        int[] graph = new int[N];
        queens(graph, 0);

        System.out.println("Starting parallel implementation...");

        tpe.submit(new MyRunnable(new int[N], 0));
    }

    private static class MyRunnable implements Runnable {

    	private int[] graph;
    	private int step;

    	public MyRunnable(int[] graph, int step) {
			this.graph = graph;
			this.step = step;
			taskCount.incrementAndGet();
		}

    	@Override
    	public void run() {
    		if (step == N) {
    			saveSolution(graph, parallelSolution);
    		} else {
    			for (int i = 0; i < N; i++) {
    				int[] newGraph = graph.clone();
    				newGraph[step] = i;

    				if (check(newGraph, step))
    					tpe.submit(new MyRunnable(newGraph, step + 1));

    			}
    		}

    		if (taskCount.decrementAndGet() == 0) {
    			tpe.shutdown();

    			System.out.println(sequentialSolution.equals(parallelSolution) ? "Correct" : "Wrong");
    		}
    	}

    }
}
