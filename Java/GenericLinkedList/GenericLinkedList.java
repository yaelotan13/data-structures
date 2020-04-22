package genericlinkedlist;

import java.util.*;

public class GenericLinkedList<T> implements Iterable<T> {
	private Node<T> head;
	private ModCount modCount = new ModCount();
	
	public void pushFront(T data) {
		head = new Node<>(data, head);
		modCount.incrementModCounter();
	}
	
	public T popFront() {
		T data = head.data;
		
		head = head.next;
		modCount.incrementModCounter();
		
		return data;
	}
	
	public int size() {
		Iterator<T> iter = iterator();
		int counter = 0;
		                                            
		while (iter.hasNext()) {
			++counter;
			iter.next();
		}
		
		return counter;
	}
	
	public boolean isEmpty() {
		return (null == head);
	}
	
	public Iterator<T> find(T key) {
		Iterator<T> iter = iterator();
		Iterator<T> prevIter = iterator();
		
		while (iter.hasNext() && !prevIter.next().equals(key)) {
			iter.next();
		}
	
		return prevIter;
	}
	
	@Override
	public Iterator<T> iterator() {
		return new ImplIterator<>(head, modCount);
	}
	
	public static <T> GenericLinkedList<T> newReverse(GenericLinkedList<T> list) {
		GenericLinkedList<T> newGenericList = new GenericLinkedList<>();
		
		for (T t : list) {
			newGenericList.pushFront(t);
		}
	
		return newGenericList;
		
	}
	
	public static <T> GenericLinkedList<T> newMerge(GenericLinkedList<T> list1, GenericLinkedList<T> list2) {
		GenericLinkedList<T> newMergedList = new GenericLinkedList<>();
		
		for (T t : list2) {
			newMergedList.pushFront(t);
		}
		
		for (T t : list1) {
			newMergedList.pushFront(t);
		}

		return newMergedList;
	}
	
	private static class ImplIterator<E> implements Iterator<E> {
		private Node<E> current;
		private ModCount modCount;
		private ModCount copyModCount;
		
		public ImplIterator(Node<E> current, ModCount modCount) {
			this.current = current;
			copyModCount = new ModCount(modCount);
			this.modCount = modCount;
		}
		
		@Override
		public boolean hasNext() {			
			return (null != current);
		}

		@Override
		public E next() {
			E data = current.data;
			current = current.next;
			
			if (!copyModCount.equals(modCount)) {
				throw new ConcurrentModificationException();
			}
			
			return data;
		}
	}
	
	private static class Node<E> {
		private E data;
		private Node<E> next;
		
		public Node(E data, Node<E> next) {
			this.data = data;
			this.next = next;
		}
	}
	
	private static class ModCount {
		private int modCount;
		
		public ModCount() {}
		
		public ModCount(ModCount other) {
			this.modCount = other.modCount;
		}
		
		public void incrementModCounter() {
			++modCount;
		}
		
		//@Override 
		public boolean equals(Object modCount) {
			return (((ModCount)modCount).modCount == this.modCount);
		}
	}
}
