/**
 * @class
 * @summary
 * Linked list class.
 * Instantiating this class would return a new list
 */

function LinkedList() {
  this._head = null;
};

LinkedList.prototype.isEmpty = function () {
  return this._head === null;
};

LinkedList.prototype.size = function () {
  let size = 0;
  for (let elem of this) {
    size += 1;
  }

  return size;
};

LinkedList.prototype.popFront = function () {
  if (this._head === null) {
    throw new ReferenceError('pop from an empty list');
  }

  const curHead = this._head;
  const newHead = curHead.next;
  this._head = newHead;
  return curHead.data;
};

LinkedList.prototype.pushFront = function (data) {
  this._head = new LinkedList.Node(data, this._head);
};

LinkedList.prototype.find = function (key, isMatch) {
  for (const elem of this) {
    if (isMatch(key, elem)) {
      return elem;
    }
  }

  return undefined;
};

LinkedList.prototype[Symbol.iterator] = function () {
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
};

LinkedList.Node = function (data, next) {
  this.data = data;
  this.next = next;
};

module.exports = { LinkedList };
