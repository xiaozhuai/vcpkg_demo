if (Array.of === undefined) {
    Array.of = function () {
        return Array.prototype.slice.call(arguments);
    };
}
