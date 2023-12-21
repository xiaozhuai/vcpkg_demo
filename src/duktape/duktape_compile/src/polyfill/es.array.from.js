require('./es.set');

if (Array.from === undefined) {
    Array.from = function (arr, callbackFn, thisArg) {
        let newArr = [];
        let k = [];
        if (globalThis.Set && arr instanceof Set) {
            arr.forEach(v => {
                k.push(v);
            });
            arr = k;
        }
        for (let i = 0; i < arr.length; i++) {
            newArr[i] = callbackFn
                ? callbackFn.call(thisArg, arr[i], i, arr)
                : arr[i];
        }
        return newArr;
    };
}
