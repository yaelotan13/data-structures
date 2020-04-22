const LinkedList = require('../linked-list-prototype').LinkedList;

let list = new LinkedList();
let sizeListOfNumbers;
let sizeListOfStrings;
let sizeListOfArrays;
let sizeListOfObjects;

list.pushFront(30);
list.pushFront(40);

sizeListOfNumbers = list.size();

list = new LinkedList();
list.pushFront('boo');
list.pushFront('foo');
list.pushFront('goo');

sizeListOfStrings = list.size();

list = new LinkedList();
list.pushFront([1,2,3]);
list.pushFront([20,30]);
list.pushFront([100,200]);

sizeListOfArrays = list.size();

list = new LinkedList();
list.pushFront({name: 'john'});
list.pushFront({age: 26});
list.pushFront({name: 'tal'});
list.pushFront({hobbies: 'sports'});

sizeListOfObjects = list.size();

describe('pushFront', () => {
    test('size - after push for numbers', () => {
        expect(sizeListOfNumbers).toBe(2);
    });

    test('size - after push for strings', () => {
        expect(sizeListOfStrings).toBe(3);
    });

    test('size - after push of arrays', () => {
        expect(sizeListOfArrays).toBe(3);
    });

    test('size - after push of objects', () => {
        expect(sizeListOfObjects).toBe(4);
    });
});
