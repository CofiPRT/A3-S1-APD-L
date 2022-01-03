package parallelSearch;

import java.util.concurrent.BrokenBarrierException;

public class ParallelSearch extends Thread {
    private final int id;
    private int prevStart;
    private int prevEnd;


    public ParallelSearch(int id) {
        super();
        this.id = id;
        prevStart = 0;
        prevEnd = 0;
    }

    @Override
    public void run() {

    	while (!Main.foundElement) {

    		int start = Main.left + id * (int) Math.ceil((double) (1 + Main.right - Main.left) / Main.P);
    		int end = Math.min(Main.SIZE - 1, Main.left + (1 + id) * (int) Math.ceil((double) (1 + Main.right - Main.left) / Main.P));

    		try {
				Main.barrier.await();
			} catch (InterruptedException | BrokenBarrierException e) {
				e.printStackTrace();
			}

    		if (start > end || (start == prevStart && end == prevEnd))
    			return;

    		prevStart = start;
    		prevEnd = end;

    		if (Main.v[start] == Main.targetElement) {
    			Main.foundElement = true;
    			Main.positionElement = start;
    		} else if (Main.v[end] == Main.targetElement) {
    			Main.foundElement = true;
    			Main.positionElement = end;
    		} else if (Main.v[start] <= Main.targetElement && Main.v[end] >= Main.targetElement) {
    			Main.left = start;
    			Main.right = end;
    		}

    		try {
				Main.barrier.await();
			} catch (InterruptedException | BrokenBarrierException e) {
				e.printStackTrace();
			}

    	}

    }
}
