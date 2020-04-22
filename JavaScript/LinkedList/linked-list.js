/**
 * @class
 * @summary
 * Linked list class.
 * Instantiating this class would return a new list
 */
class LinkedList {
  /**
   * @constructor
   * @summary
   * Constructs an empty list
   */
  constructor() {
    this._head = null;
  }

  /**
   * @function isEmpty
   * @returns {boolean}
   * Returns true if this list contains no elements
   */
  isEmpty() {
    return this._head === null;
  }

  /**
   * @function size
   * @summary
   * Returns the number of elements of the list
   * @returns {number}
   */
  size() {
    let size = 0;
    for (let elem of this) {
      size += 1;
    }

    return size;
  }

  /**
   * @function popFront
   * @summary
   * Removes the first element of the list and returns it
   * @returns {*}
   */
  popFront() {
    if (this._head === null) {
      throw new ReferenceError('pop from an empty list');
    }

    const curHead = this._head;
    const newHead = curHead.next;
    this._head = newHead;
    return curHead.data;
  }

  /**
   * @function pushFront
   * @summary
   * Get a new element to push to the front of the list
   * @param {*} element - element to push to the front of the list
   */
  pushFront(data) {
    this._head = new Node(data, this._head);
  }

  /**
   * @function find
   * @summary
   * Get an element to find in the list and a compare function to search by.
   * Returns an iterator from the desired location or null if not found
   * @param {*} key - element to find in the list
   * @param {function} isMatch - function to search by in the list
   * @returns {*} data - the desired data or null if not found
   */
  find(key, isMatch) {
    for (const elem of this) {
      if (isMatch(key, elem)) {
        return elem;
      }
    }

    return undefined;
  }

  /**
   * @property iterator
   * @summary
   * Get an iterator starting from the head of the list, or null if the list is empty
   * @returns {Object} iterator - iterator starting from the head of the list, or null if the list is empty
   */
  [Symbol.iterator]() {
    let curHead = this._head;

    return {
      next: function () {
        if (curHead === null) {
          return { done: true };
        }

        let result = { value: curHead.data, done: false };
        curHead = curHead.next;
        return result;
      },
    };
  }
}

/**
 * @class
 * @summary
 * node class which the list is build out of
 */
class Node {
  /**
   * @constructor
   * @param {*} data - data that the node holds in it's value
   * @param {Node} next - the next node this node points to
   */
  constructor(data, next) {
    this.data = data;
    this.next = next;
  }
}

module.exports = { LinkedList };
