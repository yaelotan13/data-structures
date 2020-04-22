const LinkedList = require('../linked-list-prototype').LinkedList;

const list = new LinkedList();
let sizeOnEmptyOnEmptyList = list.size();
let sizeOnANonEmptyList;
let sizeAfterPop;

list.pushFront(30);
list.pushFront(40);
list.pushFront(50);
sizeOnANonEmptyList = list.size();

list.popFront();
list.popFront();
sizeAfterPop = list.size();

describe('size', () => {
    test('size on an empty list', () => {
        expect(sizeOnEmptyOnEmptyList).toBe(0);
    });

    test('size on a non empty list', () => {
        expect(sizeOnANonEmptyList).toBe(3);
    });

    test('size after push and pop', () => {
        expect(sizeAfterPop).toBe(1);
    });
})
