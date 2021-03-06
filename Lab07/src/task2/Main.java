package task2;

import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;

public class Main {

	private static final int THREADS = 4;

	private static AtomicInteger taskCount = new AtomicInteger(0);
	private static ExecutorService tpe = Executors.newFixedThreadPool(THREADS);

	private static int N = 10;
	private static int COLORS = 3;
	private static int[][] graph = { { 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 }, { 1, 6 }, { 2, 1 }, { 2, 3 }, { 2, 7 },
            { 3, 2 }, { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 }, { 5, 0 }, { 5, 7 }, { 5, 8 }, { 6, 1 },
            { 6, 8 }, { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 }, { 8, 3 }, { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 },
            { 9, 7 } };

	private static Set<List<Integer>> sequentialSolution = ConcurrentHashMap.newKeySet();
	private static Set<List<Integer>> parallelSolution = ConcurrentHashMap.newKeySet();

	private static void colorGraph(int[] colors, int step) {
        if (step == N) {
            sequentialSolution.add(Arrays.stream(colors).boxed().collect(Collectors.toList()));
            return;
        }

        // for the node at position step try all possible colors
        for (int i = 0; i < COLORS; i++) {
            int[] newColors = colors.clone();
            newColors[step] = i;
            if (verifyColors(newColors, step))
                colorGraph(newColors, step + 1);
        }
    }

    private static boolean verifyColors(int[] colors, int step) {
        for (int i = 0; i < step; i++) {
            if (colors[i] == colors[step] && isEdge(i, step))
                return false;
        }
        return true;
    }

    private static boolean isEdge(int a, int b) {
        for (int[] ints : graph) {
            if (ints[0] == a && ints[1] == b)
                return true;
        }
        return false;
    }

    public static void main(String[] args) {
    	System.out.println("Starting sequential implementation...");

        int[] colors = new int[N];
        colorGraph(colors, 0);

        System.out.println("Starting parallel implementation...");

        tpe.submit(new MyRunnable(new int[N], 0));
    }

    private static class MyRunnable implements Runnable {

    	private int[] colors;
    	private int step;

    	public MyRunnable(int[] colors, int step) {
			this.colors = colors;
			this.step = step;
			taskCount.incrementAndGet();
		}

    	@Override
    	public void run() {
    		if (step == N) {
    			parallelSolution.add(Arrays.stream(colors).boxed().collect(Collectors.toList()));
    		} else {
    			for (int i = 0; i < COLORS; i++) {
    				int[] newColors = colors.clone();
    				newColors[step] = i;

    				if (verifyColors(newColors, step))
    					tpe.submit(new MyRunnable(newColors, step + 1));

    			}
    		}

    		if (taskCount.decrementAndGet() == 0) {
    			tpe.shutdown();

    			System.out.println(sequentialSolution.equals(parallelSolution) ? "Correct" : "Wrong");
    		}

    	}

    }
}
