const LinkedList = require('../linked-list-prototype').LinkedList;

const numberTarget = 30;
const arrayTarget = ['boo', 'foo'];
const objectTarget = { name: 'goo' };
let numberFindVal;
let arrayFindVal;
let objectFindVal;
let numberNotExistingVal;
let arrayNotExistingVal;
let objectNotExistingVal;
let list = new LinkedList();

const isStrictMatch = (key, curElement) => {
    return key === curElement ? true : false;
}

const arrayMatch = (key, cuElement) => {
    return cuElement[0] === key[0] && cuElement[1] === key[1] ? true : false;
}

const objectMatch = (key, cuElement) => {
    return cuElement.name === key.name ? true : false;
}

list.pushFront(10);
list.pushFront(20);
list.pushFront(30);
list.pushFront(40);

numberFindVal = list.find(numberTarget, isStrictMatch);
numberNotExistingVal = list.find(numberTarget + 100, isStrictMatch);

list = new LinkedList();

list.pushFront(['jenny','bob', 'boo', 'foo']);
list.pushFront([55, 35]);
list.pushFront([10, 20]);
list.pushFront(['boo', 'foo', 'goo']);

arrayFindVal = list.find(arrayTarget, arrayMatch);
arrayNotExistingVal = list.find(arrayTarget.push('someValue'), arrayMatch);

list = new LinkedList();

list.pushFront({age: 25});
list.pushFront(55);
list.pushFront([10, 20]);
list.pushFront({name: 'goo'});

objectFindVal = list.find(objectTarget, objectMatch);
objectNotExistingVal = list.find({name: 'anotherName'}, objectMatch);

describe('find', () => {
    test('find an existing number', () => {
        expect(numberFindVal).toBe(numberTarget);
    });

    test('find a non existing number', () => {
        expect(numberNotExistingVal).toBeFalsy();
    });

    test('find an existing pattern in an array', () => {
        expect(arrayFindVal).not.toBeFalsy();
    });

    test('find a non existing pattern in an array', () => {
        expect(arrayNotExistingVal).toBeFalsy();
    });

    test('find an existing pattern in an abject', () => {
        expect(objectFindVal).not.toBeFalsy();
    });

    test('find a non existing pattern in an abject', () => {
        expect(objectNotExistingVal).toBeFalsy();
    });
});
