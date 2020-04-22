const LinkedList = require('../linked-list-prototype').LinkedList;

let list = new LinkedList();
let retNumber;
let retString;
let retArray;
let retObject;
let retNumberAfterSeveralPushes;

list.pushFront(10);
retNumber = list.popFront();

list.pushFront('foo');
retString = list.popFront();

list.pushFront([1,2,3]);
retArray = list.popFront();

list.pushFront({a:10});
retObject = list.popFront();

list.pushFront(10);
list.pushFront(20);
list.pushFront(30);
retNumberAfterSeveralPushes = list.popFront();

describe('popFront', ()=> {
    test('pop to a number', () => {
        expect(retNumber).toBe(10);
    });

    test('pop to a string', () => {
        expect(retString).toBe('foo');
    });

    test('pop to an array', () => {
        expect(retArray).toEqual([1,2,3]);
    });

    test('pop to an object', () => {
        expect(retObject).toEqual({a:10});
    });

    test('pop to a number after several pushes', () => {
        expect(retNumberAfterSeveralPushes).toBe(30);
    });
});
