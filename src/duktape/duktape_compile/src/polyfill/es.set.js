if (globalThis.Set === undefined) {
    globalThis.Set = class Set {
        constructor(arr) {
            this.items = [];
            if (arr) {
                arr.forEach(v => {
                    this.add(v);
                });
            }
        }

        add(value) {
            if (!this.has(value)) {
                this.items.push(value);
            }
        }

        clear() {
            this.items = [];
        }

        delete(value) {
            let index = this.items.indexOf(value);
            if (index !== -1) {
                this.items.splice(index, 1);
            }
        }

        forEach(callbackFn, thisArg) {
            for (let i = 0; i < this.items.length; i++) {
                callbackFn.call(thisArg, this.items[i], this.items[i], this);
            }
        }

        has(value) {
            return this.items.indexOf(value) !== -1;
        }

        get size() {
            return this.items.length;
        }

        toJSON() {
            return this.items;
        }
    };
}
