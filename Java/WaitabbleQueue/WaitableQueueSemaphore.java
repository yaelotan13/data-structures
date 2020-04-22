package waitabblequeue;

import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.locks.ReentrantLock;

public class WaitableQueueSemaphore<T> implements WaitableQueue<T> {
	private PriorityQueue<T> queue;
	private Semaphore itemsInQueue = new Semaphore(0);
	private ReentrantLock queueLock = new ReentrantLock();

	public WaitableQueueSemaphore() {
		queue = new PriorityQueue<>();
	}

	public WaitableQueueSemaphore(Comparator<? super T> comparator) {
		queue = new PriorityQueue<>(comparator);
	}

	@Override
	public void enqueue(T item) {
		queueLock.lock();
		try {
			queue.add(item);
			itemsInQueue.release();
		} finally {
			queueLock.unlock();
		}
	}

	@Override
	public T dequeue() throws InterruptedException {
		try {
			return dequeue(Long.MAX_VALUE, TimeUnit.DAYS);
		} catch (TimeoutException e) {
			throw new InterruptedException("Long.MAX_VALUE has elapsed");
		}
	}

	@Override
	public T dequeue(long timeout, TimeUnit timeUnit) throws TimeoutException, InterruptedException {
		T dequeueObject = null;
		
		if (!itemsInQueue.tryAcquire(timeout, timeUnit)) {
			throw new TimeoutException();
		}
		
		queueLock.lock();
		try {
			dequeueObject = queue.poll();
		} finally {
			queueLock.unlock();
		}
		
		return dequeueObject;
	}

	@Override
	public boolean remove(T item) {
		boolean hasRemovedObject = false;
		
		queueLock.lock();
		try {
			if (itemsInQueue.availablePermits() > 0) {
				try {
					itemsInQueue.acquire();
					if (!(hasRemovedObject = queue.remove(item))) {
						itemsInQueue.release();
					}
				} catch (InterruptedException e1) {
					e1.printStackTrace();
				} 
			} 
		} finally {
			queueLock.unlock();
		}

		return hasRemovedObject;
	}
}

