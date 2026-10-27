(function () {
    'use strict';

    if (Array.prototype.at === undefined) {
      Array.prototype.at = function (index) {
        var len = this.length;
        if (index >= len || index < -len) {
          return undefined;
        }
        var k = index >= 0 ? index : index + len;
        return this[k];
      };
    }

    function _classCallCheck(a, n) { if (!(a instanceof n)) throw new TypeError("Cannot call a class as a function"); }
    function _defineProperties(e, r) { for (var t = 0; t < r.length; t++) { var o = r[t]; o.enumerable = o.enumerable || false, o.configurable = true, "value" in o && (o.writable = true), Object.defineProperty(e, _toPropertyKey$1(o.key), o); } }
    function _createClass(e, r, t) { return r && _defineProperties(e.prototype, r), Object.defineProperty(e, "prototype", { writable: false }), e; }
    function _toPropertyKey$1(t) { var i = _toPrimitive$1(t, "string"); return "symbol" == typeof i ? i : i + ""; }
    function _toPrimitive$1(t, e) { if ("object" != typeof t || !t) return t; var r; if ("undefined" != typeof Symbol && void 0 !== (r = t[Symbol.toPrimitive])) { var i = r.call(t, e); if ("object" != typeof i) return i; throw new TypeError("@@toPrimitive must return a primitive value."); } return (String )(t); }
    if (globalThis.Set === undefined) {
      globalThis.Set = /*#__PURE__*/function () {
        function Set(arr) {
          var _this = this;
          _classCallCheck(this, Set);
          this.items = [];
          if (arr) {
            arr.forEach(function (v) {
              _this.add(v);
            });
          }
        }
        return _createClass(Set, [{
          key: "add",
          value: function add(value) {
            if (!this.has(value)) {
              this.items.push(value);
            }
          }
        }, {
          key: "clear",
          value: function clear() {
            this.items = [];
          }
        }, {
          key: "delete",
          value: function _delete(value) {
            var index = this.items.indexOf(value);
            if (index !== -1) {
              this.items.splice(index, 1);
            }
          }
        }, {
          key: "forEach",
          value: function forEach(callbackFn, thisArg) {
            for (var i = 0; i < this.items.length; i++) {
              callbackFn.call(thisArg, this.items[i], this.items[i], this);
            }
          }
        }, {
          key: "has",
          value: function has(value) {
            return this.items.indexOf(value) !== -1;
          }
        }, {
          key: "size",
          get: function get() {
            return this.items.length;
          }
        }, {
          key: "toJSON",
          value: function toJSON() {
            return this.items;
          }
        }]);
      }();
    }

    if (Array.from === undefined) {
      Array.from = function (arr, callbackFn, thisArg) {
        var newArr = [];
        var k = [];
        if (globalThis.Set && arr instanceof Set) {
          arr.forEach(function (v) {
            k.push(v);
          });
          arr = k;
        }
        for (var i = 0; i < arr.length; i++) {
          newArr[i] = callbackFn ? callbackFn.call(thisArg, arr[i], i, arr) : arr[i];
        }
        return newArr;
      };
    }

    if (Array.prototype.includes === undefined) {
      Array.prototype.includes = function (searchElement, fromIndex) {
        if (this === undefined || this === null) {
          throw new TypeError('"this" is null or undefined');
        }
        var o = Object(this);
        var len = o.length >>> 0;
        if (len === 0) {
          return false;
        }
        var n = fromIndex | 0;
        var k = Math.max(n >= 0 ? n : len - Math.abs(n), 0);
        function isSameValue(a, b) {
          return a === b || typeof a === 'number' && typeof b === 'number' && isNaN(a) && isNaN(b);
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

    if (Array.of === undefined) {
      Array.of = function () {
        return Array.prototype.slice.call(arguments);
      };
    }

    function _toConsumableArray(r) { return _arrayWithoutHoles(r) || _iterableToArray(r) || _unsupportedIterableToArray(r) || _nonIterableSpread(); }
    function _nonIterableSpread() { throw new TypeError("Invalid attempt to spread non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method."); }
    function _unsupportedIterableToArray(r, a) { if (r) { if ("string" == typeof r) return _arrayLikeToArray(r, a); var t = {}.toString.call(r).slice(8, -1); return "Object" === t && r.constructor && (t = r.constructor.name), "Map" === t || "Set" === t ? Array.from(r) : "Arguments" === t || /^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(t) ? _arrayLikeToArray(r, a) : void 0; } }
    function _iterableToArray(r) { var e = "undefined" != typeof Symbol && r[Symbol.iterator] || r["@@iterator"]; if (Array.isArray(r) && (null == e || e === ("undefined" != typeof Symbol && [][Symbol.iterator]) || e === []["@@iterator"])) return r; if (null != e) { for (var t, o = [], a = e.call(r); !(t = a.next()).done;) o.push(t.value); return o; } }
    function _arrayWithoutHoles(r) { if (Array.isArray(r)) return _arrayLikeToArray(r); }
    function _arrayLikeToArray(r, a) { (null == a || a > r.length) && (a = r.length); for (var e = 0, n = Array(a); e < a; e++) n[e] = r[e]; return n; }
    if (Array.prototype.toSorted === undefined) {
      Array.prototype.toSorted = function (compareFn) {
        return _toConsumableArray(this).sort(compareFn);
      };
    }

    function ownKeys(e, r) { var t = Object.keys(e); if (Object.getOwnPropertySymbols) { var o = Object.getOwnPropertySymbols(e); r && (o = o.filter(function (r) { return Object.getOwnPropertyDescriptor(e, r).enumerable; })), t.push.apply(t, o); } return t; }
    function _objectSpread(e) { for (var r = 1; r < arguments.length; r++) { var t = null != arguments[r] ? arguments[r] : {}; r % 2 ? ownKeys(Object(t), true).forEach(function (r) { _defineProperty(e, r, t[r]); }) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(t)) : ownKeys(Object(t)).forEach(function (r) { Object.defineProperty(e, r, Object.getOwnPropertyDescriptor(t, r)); }); } return e; }
    function _defineProperty(e, r, t) { return (r = _toPropertyKey(r)) in e ? Object.defineProperty(e, r, { value: t, enumerable: true, configurable: true, writable: true }) : e[r] = t, e; }
    function _toPropertyKey(t) { var i = _toPrimitive(t, "string"); return "symbol" == typeof i ? i : i + ""; }
    function _toPrimitive(t, e) { if ("object" != typeof t || !t) return t; var r; if ("undefined" != typeof Symbol && void 0 !== (r = t[Symbol.toPrimitive])) { var i = r.call(t, e); if ("object" != typeof i) return i; throw new TypeError("@@toPrimitive must return a primitive value."); } return ("string" === e ? String : Number)(t); }
    var a = 1;
    var b = 2;
    var c = a + b;
    console.log("a + b = ".concat(c));
    console.log({
      name: 'xiaozhuai'
    });
    console.log([1, 2, 3]);
    console.assert(1 === 2, '1 !== 2', {
      aaa: 111
    });
    var obj1 = {
      a: 1
    };
    var obj2 = {
      b: 2
    };
    console.log(Object.assign(obj1, obj2));
    console.log(_objectSpread(_objectSpread({}, obj1), obj2));
    console.error('this is an error message');

    /** @var {Object} bridge **/
    /** @var {Object} bridge.native **/
    /** @var {Object} bridge.js **/
    if (globalThis.bridge !== undefined) {
      /** @var {function(number)} bridge.native.callbackValue **/
      bridge["native"].callbackValue(c);
      bridge.js.pow = function (a, b) {
        return Math.pow(a, b);
      };
    }
    var d = new Set([obj1, [111], 'a', 'b', 'c']);
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
    console.log([1, 2, 3].toSorted(function (a, b) {
      return b - a;
    }));
    console.log([1, 2, 3].reverse());

})();
