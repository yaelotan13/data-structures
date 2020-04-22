package waitabblequeue;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import org.junit.jupiter.api.Test;

class WaitabbleQueueConditionVarTest {

	@Test
	void testEnqueue() {
		WaitableQueueConditionVar<String> queue = new WaitableQueueConditionVar<>();
		Thread enqueueT = new Thread(() -> {
			queue.enqueue("Hello");
		});
		
		Thread dequeueT = new Thread(() -> {
			String ret = null;
			try {
				ret = queue.dequeue();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			assertEquals(ret, "Hello");
		});
		
		dequeueT.start();
		enqueueT.start();
	}
	
	@Test
	void testDeququBeforEnqueue() throws InterruptedException {
		WaitableQueueConditionVar<String> queue = new WaitableQueueConditionVar<>();
		
		Thread dequeueT = new Thread(() -> {
			String ret = null;
			try {
				ret = queue.dequeue();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			assertNotNull(ret);
		});
		
		Thread enqueueT1 = new Thread(() -> {
			queue.enqueue("Hello");
		});
		
		dequeueT.start();
		Thread.sleep(1000);
		
		enqueueT1.start();
	}
	
	@Test
	void testSeveralEnqueueAndDequeue() throws InterruptedException {
		WaitableQueueConditionVar<String> queue = new WaitableQueueConditionVar<>();
		
		Thread dequeueT1 = new Thread(() -> {
			String ret = null;
			try {
				ret = queue.dequeue();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			System.out.println("return value is: " + ret);
			assertNotNull(ret);
		});
		
		Thread dequeueT2 = new Thread(() -> {
			String ret = null;
			try {
				ret = queue.dequeue();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			System.out.println("return value is: " + ret);
			assertNotNull(ret);
		});
		
		Thread dequeueT3 = new Thread(() -> {
			String ret = null;
			try {
				ret = queue.dequeue();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			System.out.println("return value is: " + ret);
			assertNotNull(ret);
		});
		
		
		Thread enqueueT1 = new Thread(() -> {
			queue.enqueue("What's up?");
		});
		
		Thread enqueueT2 = new Thread(() -> {
			queue.enqueue("Como asstas?");
		});
		
		Thread enqueueT3 = new Thread(() -> {
			queue.enqueue("Kakdila?");
		});
		
		enqueueT1.start();
		enqueueT1.join();
		enqueueT2.start();
		enqueueT2.join();
		enqueueT3.start();
		enqueueT3.join();
		
		dequeueT1.start();
		dequeueT2.start();
		dequeueT3.start();
	}
	
	@Test
	void testRemoveBeforEnqueue() throws InterruptedException {
		WaitableQueueConditionVar<String> queue = new WaitableQueueConditionVar<>();
		
		Thread removeT = new Thread(() -> {
			boolean ret;
			ret = queue.remove("Hello");
			assertFalse(ret);
		});
		
		Thread enqueueT1 = new Thread(() -> {
			queue.enqueue("Hello");
		});
		
		removeT.start();
		Thread.sleep(1000);
		
		enqueueT1.start();
	}
	
	@Test
	void testRemoveValidObject() throws InterruptedException {
		WaitableQueueConditionVar<String> queue = new WaitableQueueConditionVar<>();
		Thread enqueueT = new Thread(() -> {
			queue.enqueue("Hello");
		});
		
		Thread removeT = new Thread(() -> {
			boolean ret = queue.remove("Hello");
			assertTrue(ret);
		});
		
		enqueueT.start();
		enqueueT.join();
		removeT.start();
	}

	@Test
	void testRemoveNonValidObject() throws InterruptedException {
		WaitableQueueConditionVar<String> queue = new WaitableQueueConditionVar<>();
		Thread enqueueT = new Thread(() -> {
			queue.enqueue("Hello");
		});
		
		Thread removeT = new Thread(() -> {
			boolean ret = queue.remove("Hola");
			assertFalse(ret);
		});
		
		enqueueT.start();
		enqueueT.join();
		removeT.start();
	}
	
	@Test
	void testdequeueTimeoutEmpty() throws InterruptedException {
		WaitableQueueConditionVar<String> queue = new WaitableQueueConditionVar<>();
		
		Thread dequeueT = new Thread(() -> {
			String ret;
			try {
				ret = queue.dequeue(2, TimeUnit.SECONDS);
				assertNull(ret);
			} catch (TimeoutException e) {
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		});
		
		dequeueT.start();
		dequeueT.join();
	}
	
	@Test
	void testdequeueTimeoutQueueNotEmptyTimeNotUp() throws InterruptedException {
		WaitableQueueConditionVar<String> queue = new WaitableQueueConditionVar<>();
		
		Thread enqueueT = new Thread(() -> {
			queue.enqueue("Hello");
		});
		
		
		Thread dequeueT = new Thread(() -> {
			String ret = null;
			try {
				ret = queue.dequeue(3, TimeUnit.SECONDS);
				assertEquals(ret, "Hello");
				System.out.println("return is: " + ret);
			} catch (TimeoutException e) {
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		});
		
		dequeueT.start();
		Thread.sleep(1000);
		
		enqueueT.start();
	}
	
	@Test
	void testdequeueTimeoutQueueNotEmptyTimeIsUp() throws InterruptedException {
		WaitableQueueConditionVar<String> queue = new WaitableQueueConditionVar<>();
		Thread enqueueT = new Thread(() -> {
			queue.enqueue("Hello");
		});
		
		
		Thread dequeueT = new Thread(() -> {
			String ret;
			try {
				ret = queue.dequeue(2, TimeUnit.SECONDS);
				assertNull(ret);
			} catch (TimeoutException e) {
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		});
		
		dequeueT.start();
		dequeueT.join();
		
		enqueueT.start();
	}
}
