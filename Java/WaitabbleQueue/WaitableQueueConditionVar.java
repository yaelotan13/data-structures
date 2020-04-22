package waitabblequeue;

import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class WaitableQueueConditionVar<T> implements WaitableQueue<T> {
	private Queue<T> queue;
	private Lock queueLock = new ReentrantLock();
	private Condition queueIsNotEmpty = queueLock.newCondition();
	
	public WaitableQueueConditionVar() {
		queue = new PriorityQueue<>();
	}
	
	public WaitableQueueConditionVar(Comparator<? super T> comparator) {
		queue = new PriorityQueue<>(comparator);
	}

	@Override
	public void enqueue(T item) {
		queueLock.lock();
		try {
			queue.add(item);
			queueIsNotEmpty.signal();
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
		long timeoutInNonos = timeUnit.toMillis(timeout);
		
		queueLock.lock();
		try {
			while (queue.isEmpty()) {
				timeoutInNonos = queueIsNotEmpty.awaitNanos(timeoutInNonos);
				if (timeoutInNonos <= 0) {
					throw new TimeoutException();
				}
			}
			dequeueObject = queue.poll();
		} finally {
			queueLock.unlock();
		}
		
		return dequeueObject;
	}
	
	@Override
	public boolean remove(T item) {
		try {
			queueLock.lock();
			
			return queue.remove(item);
		} finally {
			queueLock.unlock();
		}
	}
}