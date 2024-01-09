require('./polyfill');

let a = 1;
let b = 2;
let c = a + b;
console.log(`a + b = ${c}`);
console.log({name: 'xiaozhuai'});
console.log([1, 2, 3]);
console.assert(1 === 2, '1 !== 2', {aaa: 111});
let obj1 = {a: 1};
let obj2 = {b: 2};
console.log(Object.assign(obj1, obj2));
console.log({...obj1, ...obj2});

/** @var {Object} bridge **/
/** @var {Object} bridge.native **/
/** @var {Object} bridge.js **/
if (globalThis.bridge !== undefined) {
    /** @var {function(number)} bridge.native.callbackValue **/
    bridge.native.callbackValue(c);
    bridge.js.pow = function (a, b) {
        return Math.pow(a, b);
    };
}
let d = new Set([obj1, [111], 'a', 'b', 'c']);
d.add('c');
d.add('d');
console.log(d);
console.log([1, 2, 3].includes(1));
console.log([1, 2, 3].includes(4));
console.log([1, 2, NaN, 4].includes(NaN));
console.log(Array.of(1, 2, 'a', 'b', '100'));
console.log([1, 2, 3].concat([4, 5, 6]));
console.log([1, 2, 3].at(2));
console.log([1, 2, 3].at(-2));
console.log([1, 2, 3].at(-4));
console.log([1, 2, 3].toSorted((a, b) => b - a));
console.log([1, 2, 3].reverse());
