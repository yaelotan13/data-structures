const LinkedList = require('../linked-list-prototype').LinkedList;

const list = new LinkedList();
let isEmptyOnEmptyList = list.isEmpty();
let isEmptyOnANonEmptyList;
let isEmptyAfterPushAndPop;

list.pushFront(30);
isEmptyOnANonEmptyList = list.isEmpty();
list.popFront();
isEmptyAfterPushAndPop = list.isEmpty();

describe('isEmpty', () => {
    test('isEmpty on an empty list', () => {
        expect(isEmptyOnEmptyList).toBe(true);
    });

    test('isEmpty on a non empty list', () => {
        expect(isEmptyOnANonEmptyList).toBe(false);
    });

    test('isEmpty after push and pop', () => {
        expect(isEmptyAfterPushAndPop).toBe(true);
    });
});
