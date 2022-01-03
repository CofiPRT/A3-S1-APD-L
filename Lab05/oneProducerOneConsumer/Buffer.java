package oneProducerOneConsumer;
/**
 * @author cristian.chilipirea
 *
 */
public class Buffer {
	int a = -1;

	synchronized void put(int value) {

		if (a != -1) {
			try {
				this.wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		// update
		a = value;
		notify();

	}

	synchronized int get() {

		if (a == -1) {
			try {
				this.wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		notify();

		// save before resetting
		int backup = a;
		a = -1;

		return backup;

	}
}
