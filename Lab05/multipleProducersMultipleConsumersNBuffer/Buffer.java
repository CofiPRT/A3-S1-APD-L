package multipleProducersMultipleConsumersNBuffer;

public class Buffer {

    private final LimitedQueue<Integer> queue;

    public Buffer(int size) {
        queue = new LimitedQueue<>(size);
    }

	public synchronized void put(int value) {

		while (queue.isFull()) {
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

        queue.add(value);
        notifyAll();

	}

	public synchronized int get() {

		while (queue.isEmpty()) {
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		int result = queue.poll();
		notifyAll();

        return result;

	}
}
