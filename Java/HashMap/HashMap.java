package hashmap;

import java.util.AbstractCollection;
import java.util.AbstractSet;
import java.util.ArrayList;
import java.util.Collection;
import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import pair.Pair;

public class HashMap<K,V> implements Map<K, V> {
	private static final int DEFAULT_CAPACITY = 16;
	private List<List<Entry<K, V>>> buckets;
	private Set<Entry<K, V>> entrySet;
	private Collection<V> values;
	private Set<K> keySet;
	private int capacity;
	private int modCount;
	
	public HashMap() {
		this(DEFAULT_CAPACITY);
	}
	
	public HashMap(int initialCapacity) {
		if (0 >= initialCapacity) {
			throw new IllegalArgumentException();
		}
		
		buckets = new ArrayList<>(initialCapacity); 
		capacity = initialCapacity;
		
		initilizeEntries();	
	}

	@Override
	public void clear() {
		incrementModCount();
		
		for (List<Entry<K, V>> bucket : buckets) {
			bucket.clear();
		}
	}
	
	@Override
	public boolean containsKey(Object key) {
		List<Entry<K, V>> targetBucket = buckets.get(getIndex(key));
		
		for (Entry<K,V> entry : targetBucket) {
			if (entry.getKey() == null) {
				if (null == key) {
					return true;
				}
			}
			if (entry.getKey().equals(key)) {
				return true;
			}
		}
		
		return false;
	}

	@Override
	public boolean containsValue(Object value) {
		for (List<Entry<K, V>> bucket : buckets) {
			for (Entry<K,V> entry : bucket) {
				if (entry.getValue() == null) {
					if (null == value) {
						return true;
					}
				}
				if (entry.getValue().equals(value)) {
					return true;
				}
			}
		}
		
		return false;
	}

	@Override
	public V get(Object key) {
		List<Entry<K, V>> targetBucket = buckets.get(getIndex(key));
		
		for (Entry<K,V> entry : targetBucket) {
			if (entry.getKey() == null) {
				if (null == key) {
					return null;
				}
			}
			if (entry.getKey().equals(key)) {
				return entry.getValue();
			}
		}
		
		return null;
	}
	
	@Override
	public boolean isEmpty() {		
		return !(entrySet().iterator().hasNext());
	}

	private V SetNewValueAndReturOldValue(Entry<K,V> entry, V value) {
		V oldValue = entry.getValue();
		entry.setValue(value);
		
		return oldValue;
	}
		
 	@Override
	public V put(K key, V value) {
		List<Entry<K, V>> targetBucket = buckets.get(getIndex(key));
		incrementModCount();
		
		for (Entry<K,V> entry : targetBucket) {
			if (entry.getKey() == null) {
				if (null == key) {
					return SetNewValueAndReturOldValue(entry, value);
				}
			}
			if (entry.getKey().equals(key)) {
				return SetNewValueAndReturOldValue(entry, value);
			}
		}
		
		targetBucket.add(Pair.of(key, value));
		
		return null;
	}

	@Override
	public void putAll(Map<? extends K, ? extends V> sourceMap) {		
		for (Map.Entry<? extends K, ? extends V> entry : sourceMap.entrySet()) {
			put(entry.getKey(), entry.getValue());
		}
	}

	@Override
	public V remove(Object key) {
		List<Entry<K, V>> targetBucket = buckets.get(getIndex(key));
		Iterator<Entry<K, V>> linkedListIter = targetBucket.iterator();
			
		while (linkedListIter.hasNext()) {
			Entry<K, V> currentEntry = linkedListIter.next();
			if (currentEntry.getKey() == null) {
				if (key == null) {
					linkedListIter.remove();
					incrementModCount();
					
					return oldValue(currentEntry);
				}
			}
			if (currentEntry.getKey().equals(key)) {
				linkedListIter.remove();
				incrementModCount();
				
				return oldValue(currentEntry);
			}
		}
		
		return null;
	}
	
	private V oldValue(Entry<K, V> currentEntry) {
		return currentEntry.getValue();
	}

	@Override
	public int size() {
		int count = 0;
		
		for (List<Entry<K, V>> bucket : buckets) {
			for (@SuppressWarnings("unused") Entry<K,V> entry : bucket) {
				++count;
			}
		}
		
		return count;
	}

	@Override
	public Set<Entry<K, V>> entrySet() {
		if (entrySet == null) {
			return new EntrySet();			
		}
		
		return entrySet;
	}
	
	@Override
	public Collection<V> values() {
		if (values == null) {
			return new Values();
		}
		
		return values;
	}
	
	@Override
	public Set<K> keySet() {
		if (keySet == null) {
			return new KeySet();
		}
		
		return keySet;
	}
	

	private void initilizeEntries() {
		for (int i = 0; i < capacity; ++i) {
			buckets.add(i, new LinkedList<Entry<K,V>>());
		}	
	}
	
	private int getIndex(Object key) {
		if (key == null) {
			return 0;
		}
		
		return Math.abs(key.hashCode() % capacity);
	}
	
	private void incrementModCount() {
		++modCount;
	}
	
	private class EntrySet extends AbstractSet<Entry<K, V>> { 

		@Override
		public Iterator<Entry<K, V>>  iterator() {
			return new EntryIterator();
		}

		@Override
		public int size() {
			return HashMap.this.size();
		}
		
		private class EntryIterator implements Iterator<Entry<K, V>> {
			private Iterator<List<Entry<K, V>>> bucketsIter = buckets.iterator();
			private Iterator<Entry<K,V>> entriesIter;
			private int currentBucket= 0;
			private int copyModCount;
			
			public EntryIterator() {
				copyModCount = modCount;
				if (bucketsIter.hasNext()) {
					entriesIter = bucketsIter.next().iterator();
				}
			}
			
			@Override
			public boolean hasNext() {
				if (copyModCount != modCount) {
					throw new ConcurrentModificationException();
				}
				
				while (currentBucket < buckets.size()) {
					if (entriesIter.hasNext()) {
						return true;
					} 
					if (bucketsIter.hasNext()) {
						entriesIter = bucketsIter.next().iterator();
						if (entriesIter.hasNext()) {		
							return true;
						}
					}
					
					++currentBucket;
				}
				
				return false;
			}

			@Override
			public Entry<K, V> next() {
				if (copyModCount != modCount) {
					throw new ConcurrentModificationException();
				}
				return entriesIter.next();
			}			
		}
	}
	
	private class KeySet extends AbstractSet<K> {
		
		@Override
		public Iterator<K> iterator() {
			return new KeyIterator();
		}

		@Override
		public int size() {
			return HashMap.this.size();
		}
		
		private class KeyIterator implements Iterator<K> {
			private Iterator<Map.Entry<K, V>> entries = entrySet().iterator();

			@Override
			public boolean hasNext() {
				return entries.hasNext();
			}

			@Override
			public K next() {
				return entries.next().getKey();
			}			
		}
	}
	
	private class Values extends AbstractCollection<V> {
		
		@Override
		public Iterator<V> iterator() {
			return new ValueIterator();
		}

		@Override
		public int size() {
			return HashMap.this.size();
		}
		
		class ValueIterator implements Iterator<V> {
			private Iterator<Map.Entry<K, V>> entries = entrySet().iterator();
			
			@Override
			public boolean hasNext() {
				return entries.hasNext();
			}

			@Override
			public V next() {
				return entries.next().getValue();
			}			
		}
	}
} 