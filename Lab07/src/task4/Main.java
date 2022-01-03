package task4;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {

	private static final int THREADS = 4;

	private static AtomicInteger taskCount = new AtomicInteger(0);
	private static ForkJoinPool fjp = new ForkJoinPool(THREADS);

    private static int[][] graph = { { 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 }, { 1, 6 }, { 2, 1 }, { 2, 3 }, { 2, 7 },
            { 3, 2 }, { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 }, { 5, 0 }, { 5, 7 }, { 5, 8 }, { 6, 1 },
            { 6, 8 }, { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 }, { 8, 3 }, { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 },
            { 9, 7 } };

    private static int destination = 3;

    private static Set<List<Integer>> sequentialSolution = ConcurrentHashMap.newKeySet();
    private static Set<List<Integer>> parallelSolution = ConcurrentHashMap.newKeySet();

    private static void getPath(ArrayList<Integer> partialPath) {
        if (partialPath.get(partialPath.size() - 1) == destination) {
        	sequentialSolution.add(partialPath);
            return;
        }

        // don't enter cycles
        int lastNodeInPath = partialPath.get(partialPath.size() - 1);
        for (int[] ints : graph) {
            if (ints[0] == lastNodeInPath) {
                if (partialPath.contains(ints[1]))
                    continue;
                ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
                newPartialPath.add(ints[1]);
                getPath(newPartialPath);
            }
        }
    }

    public static void main(String[] args) {
    	System.out.println("Starting sequential implementation...");

        ArrayList<Integer> partialPath = new ArrayList<>();
        partialPath.add(0);
        getPath(partialPath);

        System.out.println("Starting parallel implementation...");

        fjp.invoke(new MyTask(Arrays.asList(0)));
        fjp.shutdown();

        System.out.println(sequentialSolution.equals(parallelSolution) ? "Correct" : "Wrong");
    }

    private static class MyTask extends RecursiveTask<Void> {

		private static final long serialVersionUID = 1L;

		private List<Integer> partialPath;

    	public MyTask(List<Integer> partialPath) {
			this.partialPath = partialPath;
			taskCount.incrementAndGet();
		}

    	@Override
    	protected Void compute() {
    		int lastNode = partialPath.get(partialPath.size() - 1);

    		List<MyTask> childTasks = new ArrayList<>();

    		if (lastNode == destination) {
    			parallelSolution.add(partialPath);
    		} else {
    			// don't enter cycles
    			for (int[] edge : graph) {
    				int source = edge[0];
    				int dest = edge[1];

    				if (source != lastNode || partialPath.contains(dest))
    					continue;

    				List<Integer> newPartialPath = new ArrayList<>(partialPath);
    				newPartialPath.add(dest);

    				MyTask newTask = new MyTask(newPartialPath);
    				childTasks.add(newTask);
    				newTask.fork();
    			}
    		}

    		for (MyTask childTask : childTasks)
    			childTask.join();

    		return null;
    	}

    }
}
