package genericlinkedlist;

import static org.junit.Assert.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.ConcurrentModificationException;
import java.util.Iterator;

import org.junit.Test;

public class GenericListTest {

	@Test
	public void isEmptyOnEmptyList() {
		GenericLinkedList<String> genericList = new GenericLinkedList<>();
		boolean isEmpty = genericList.isEmpty();
		assertEquals(isEmpty, true);
	}
	
	@Test
	public void sizeOnEmptyList() {
		GenericLinkedList<String> genericList = new GenericLinkedList<>();
		int size = genericList.size();
		assertEquals(size, 0);
	}
	
	@Test
	public void isEmptyOnNonEmptyList() {
		GenericLinkedList<String> genericList = new GenericLinkedList<>();
		genericList.pushFront("yael");
		genericList.pushFront("rony");
		genericList.pushFront("daniel");
		genericList.pushFront("inbar");
		genericList.pushFront("misheloli");
		genericList.pushFront("asher");
		genericList.pushFront("tal");
		boolean isEmpty = genericList.isEmpty();
		assertEquals(isEmpty, false);
	}

	@Test
	public void sizeOnNonEmptyList() {
		GenericLinkedList<String> genericList = new GenericLinkedList<>();
		genericList.pushFront("yael");
		genericList.pushFront("rony");
		genericList.pushFront("daniel");
		genericList.pushFront("inbar");
		genericList.pushFront("misheloli");
		genericList.pushFront("asher");
		genericList.pushFront("tal");
		int size = genericList.size();
		assertEquals(size, 7);
	}
	
	@Test
	public void iteratorNonEmptyList() {
		GenericLinkedList<String> genericList = new GenericLinkedList<>();
		genericList.pushFront("yael");
		genericList.pushFront("rony");

		
		Iterator<String> iter = genericList.iterator();
		int i = 0;
		while (iter.hasNext()) {
			if (i == 0) {
				assertEquals("rony", iter.next());
			}
			if (i == 1) {
				assertEquals("yael", iter.next());
			}
			++i;
		}
	}
	
	@Test
	public void popNonEmptyList() {
		GenericLinkedList<String> genericList = new GenericLinkedList<>();
		genericList.pushFront("yael");
		genericList.pushFront("rony");
		genericList.popFront();
		int size = genericList.size();
		assertEquals(size, 1);
	}
	
	@Test
	public void findNonEmptyList() {
		GenericLinkedList<String> genericList = new GenericLinkedList<>();
		genericList.pushFront("yael");
		genericList.pushFront("rony");
		genericList.pushFront("daniel");
		genericList.pushFront("inbar");
		genericList.pushFront("misheloli");
		genericList.pushFront("asher");
		genericList.pushFront("tal");
		
		Iterator<String> found = genericList.find("misheloli");
		
		assertEquals("misheloli", found.next());
	}
	
	@Test
	public void reverseNonEmptyList() {
		GenericLinkedList<String> genericList = new GenericLinkedList<>();
		genericList.pushFront("yael");
		genericList.pushFront("rony");
		genericList.pushFront("daniel");

		GenericLinkedList<String> reveresedGenericList = GenericLinkedList.newReverse(genericList);
		Iterator<String> reversIter = reveresedGenericList.iterator();
		
		int i = 0;
		while (reversIter.hasNext()) {
			if (i == 0) {
				assertEquals("yael", reversIter.next());
			}
			if (i == 1) {
				assertEquals("rony", reversIter.next());
			}
			if (i == 2) {
				assertEquals("daniel", reversIter.next());
			}
			++i;
		}
	}
	
	@Test 
	public void merge() {
		GenericLinkedList<Integer> g1 = new GenericLinkedList<>();
		g1.pushFront(3);
		g1.pushFront(2);
		
		GenericLinkedList<Integer> g2 = new GenericLinkedList<>();
		g2.pushFront(1);
		g2.pushFront(0);
		
		GenericLinkedList<Integer> merged = GenericLinkedList.NewMerge(g1, g2);
		Iterator<Integer> mergedIter = merged.iterator();
		
		int i = 0;
		while (mergedIter.hasNext()) {
			if (i == 0) {
				assertEquals(3, (int)mergedIter.next());
			}
			if (i == 1) {
				assertEquals(2, (int)mergedIter.next());
			}
			if (i == 2) {
				assertEquals(1, (int)mergedIter.next());
			}
			if (i == 3) {
				assertEquals(0, (int)mergedIter.next());
			}
			++i;
		}
	}
	
	@Test
	public void throwsException() {
		GenericLinkedList<Integer> g1 = new GenericLinkedList<>();
		g1.pushFront(3);
		g1.pushFront(2);
		
		GenericLinkedList<Integer> g2 = new GenericLinkedList<>();
		g2.pushFront(1);
		g2.pushFront(0);
		
		GenericLinkedList<Integer> merged = GenericLinkedList.NewMerge(g1, g2);
		Iterator<Integer> mergedIter = merged.iterator();
		
		int i = 0;
		while (mergedIter.hasNext()) {
			if (i == 0) {
				assertEquals(3, (int)mergedIter.next());
			}
			if (i == 1) {
				assertEquals(2, (int)mergedIter.next());
				merged.popFront();
			}
			if (i == 2) {
				assertThrows(ConcurrentModificationException.class, () -> {
					mergedIter.next();
				    });
			}
			if (i == 3) {
				assertEquals(0, (int)mergedIter.next());
			}
			++i;
		}
	}
}
