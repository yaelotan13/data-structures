package hashmap;

import static org.junit.jupiter.api.Assertions.*;
import java.util.Map;
import org.junit.jupiter.api.Test;

import java.util.ConcurrentModificationException;
import java.util.Iterator;

class TestHashMap {

	@Test
	void testIsEmpty() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		assertEquals(true, hashMap.isEmpty());
	}

	@Test
	void testSizeOnEmptyHashMap() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		assertEquals(0, hashMap.size());
	}
	
	@Test
	void testputOneItem() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		
		assertEquals(1, hashMap.size());
	}
	
	@Test
	void testSizeSeveraleItems() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		hashMap.put("Asher", 125623011);
		hashMap.put("Tal", 206169626);
		
		assertEquals(5, hashMap.size());
	}
	
	@Test
	void testIsEmptyOnSeveraleItems() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		hashMap.put("Asher", 125623011);
		hashMap.put("Tal", 206169626);
		
		assertEquals(false, hashMap.isEmpty());
	}
	
	@Test
	void testContainsKeyWhereKeyIsFound() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		hashMap.put("Asher", 125623011);
		hashMap.put("Tal", 206169626);
		
		assertEquals(true, hashMap.containsKey("Asher"));
	}
	
	@Test
	void testContainsKeyWhereKeyIsNotFound() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		hashMap.put("Asher", 125623011);
		hashMap.put("Tal", 206169626);
		
		assertEquals(false, hashMap.containsKey("asher"));
	}
	
	@Test
	void testContainsValueWhereValueIsFound() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		hashMap.put("Asher", 125623011);
		hashMap.put("Tal", 206169626);
		
		assertEquals(true, hashMap.containsValue(308338243));
	}
	
	@Test
	void testContainsValueWhereValueIsNotFound() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		hashMap.put("Asher", 125623011);
		hashMap.put("Tal", 206169626);
		
		assertEquals(false, hashMap.containsValue(308338245));
	}
	
	@Test
	void testGetKeyeWhereKeyIsFound() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		hashMap.put("Asher", 125623011);
		hashMap.put("Tal", 206169626);
		Integer ret = hashMap.get("Asher");
		
		assertEquals(125623011, ret);
	}
	
	@Test
	void testGetKeyeWhereKeyIsNotFound() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		hashMap.put("Asher", 125623011);
		hashMap.put("Tal", 206169626);
		Integer ret = hashMap.get("Ofir");
		
		assertEquals(null, ret);
	}
	
	@Test
	void testPutAll() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		hashMap.put("Asher", 125623011);
		hashMap.put("Tal", 206169626);
		HashMap<String, Integer> newHashMap = new HashMap<>();
		newHashMap.putAll(hashMap);
		
		assertEquals(false, newHashMap.isEmpty());
		assertEquals(5, newHashMap.size());
		assertEquals(308338243, newHashMap.get("Yael"));
	}
	
	@Test
	void testRemoveKeyIsValid() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		hashMap.put("Asher", 125623011);
		hashMap.put("Tal", 206169626);
		hashMap.remove("Yael");
		
		assertEquals(4, hashMap.size());
		assertEquals(null, hashMap.get("Yael"));
	}
	
	@Test
	void testRemoveKeyIsNotValid() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		hashMap.put("Asher", 125623011);
		hashMap.put("Tal", 206169626);
		
		assertEquals(null, hashMap.remove("Yaeli"));
	}
	
	@Test
	void testClear() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		
		assertEquals(308338243, hashMap.get("Yael"));
		assertEquals(209654562, hashMap.get("Rony"));
		
		hashMap.clear();
		
		assertEquals(null, hashMap.get("Yael"));
		assertEquals(null, hashMap.get("Rony"));
	}
	
	@Test
	void testEntrySet() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		
		int i = 0;
		for (Map.Entry<String, Integer> cur : hashMap.entrySet()) {
			if (i == 2) {
				assertEquals("Yael", cur.getKey());
				assertEquals(308338243, cur.getValue());
			}
			if (i == 1) {
				assertEquals("Rony", cur.getKey());
				assertEquals(209654562, cur.getValue());
			}
			if (i == 0) {
				assertEquals("Lior", cur.getKey());
				assertEquals(290971223, cur.getValue());
			}
			++i;
		}
	}
	
	@Test
	void testValues() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		
		int i = 0;
		for (Integer cur : hashMap.values()) {
			if (i == 2) {
				assertEquals(308338243, cur);
			}
			if (i == 1) {
				assertEquals(209654562, cur);
			}
			if (i == 0) {
				assertEquals(290971223, cur);
			}
			++i;
		}
	}
	
	@Test
	void testKeySet() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		
		int i = 0;
		for (String cur : hashMap.keySet()) {
			if (i == 2) {
				assertEquals("Yael", cur);
			}
			if (i == 1) {
				assertEquals("Rony", cur);
			}
			if (i == 0) {
				assertEquals("Lior", cur);
			}
			++i;
		}
	}
	
	@Test
	void testExceptionThrowen() {
		HashMap<String, Integer> hashMap = new HashMap<>();
		hashMap.put("Yael", 308338243);
		hashMap.put("Rony", 209654562);
		hashMap.put("Lior", 290971223);
		
		int i = 0;
		Iterator<Map.Entry<String, Integer>> iter =  hashMap.entrySet().iterator();
		Map.Entry<String, Integer> cur;
		
		while (iter.hasNext()) {
			cur = iter.next();
			if (i == 2) {
				assertEquals("Yael", cur.getKey());
				assertEquals(308338243, cur.getValue());
				hashMap.put("Asher", 12345678);
				try {
					boolean hasNext = iter.hasNext();
					fail(); //if it reached it there is a bug with the fail-fast iterator
				} catch (ConcurrentModificationException e) {
					return;
				}
			}
			if (i == 1) {
				assertEquals("Rony", cur.getKey());
				assertEquals(209654562, cur.getValue());
			}
			if (i == 0) {
				assertEquals("Lior", cur.getKey());
				assertEquals(290971223, cur.getValue());
			}
			++i;
		}
	}
}
