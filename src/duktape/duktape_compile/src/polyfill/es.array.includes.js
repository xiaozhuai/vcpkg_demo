if (Array.prototype.includes === undefined) {
    Array.prototype.includes = function (searchElement, fromIndex) {
        if (this === undefined || this === null) {
            throw new TypeError('"this" is null or undefined');
        }
        let o = Object(this);
        let len = o.length >>> 0;
        if (len === 0) {
            return false;
        }
        let n = fromIndex | 0;
        let k = Math.max(n >= 0 ? n : len - Math.abs(n), 0);

        function isSameValue(a, b) {
            return a === b || (typeof a === 'number' && typeof b === 'number' && isNaN(a) && isNaN(b));
        }

        while (k < len) {
            if (isSameValue(o[k], searchElement)) {
                return true;
            }
            k++;
        }
        return false;
    };
}
