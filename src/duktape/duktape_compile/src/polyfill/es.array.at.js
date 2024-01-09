if (Array.prototype.at === undefined) {
    Array.prototype.at = function (index) {
        let len = this.length;
        if (index >= len || index < -len) {
            return undefined;
        }
        let k = index >= 0 ? index : index + len;
        return this[k];
    };
}
