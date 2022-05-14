if (Array.prototype.toSorted === undefined) {
    Array.prototype.toSorted = function (compareFn) {
        return [...this].sort(compareFn);
    };
}
