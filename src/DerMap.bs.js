// Generated by BUCKLESCRIPT VERSION 5.0.4, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");

function $at$colon(inst, key) {
  var match = Belt_Array.getBy(inst, (function (param) {
          return param[0] === key;
        }));
  if (match !== undefined) {
    return Caml_option.some(match[1]);
  }
  
}

function $at$pipe(inst, param) {
  var key = param[0];
  var match = Belt_Array.getBy(inst, (function (param) {
          return param[0] === key;
        }));
  if (match !== undefined) {
    return match[1];
  } else {
    return param[1];
  }
}

function $at$colon$bang(inst, key) {
  return Belt_Option.getExn(Belt_Array.getBy(inst, (function (param) {
                      return param[0] === key;
                    })))[1];
}

function $at$neg(inst, key) {
  return Belt_Array.keep(inst, (function (param) {
                return key !== param[0];
              }));
}

function $at$plus$plus(inst, entries) {
  return Belt_Array.concat(inst, Belt_Array.keep(entries, (function (param) {
                    var k = param[0];
                    return !Belt_Array.some(inst, (function (param) {
                                  return k === param[0];
                                }));
                  })));
}

function $at$plus(inst, entry) {
  return $at$plus$plus(inst, /* array */[entry]);
}

function $at$hash(inst, param) {
  var newVal = param[1];
  var key = param[0];
  return Belt_Array.map(inst, (function (param) {
                var k = param[0];
                var match = k === key;
                if (match) {
                  return /* tuple */[
                          k,
                          newVal
                        ];
                } else {
                  return /* tuple */[
                          k,
                          param[1]
                        ];
                }
              }));
}

function $at$hash$great(inst, param) {
  var foo = param[1];
  var key = param[0];
  return Belt_Array.map(inst, (function (param) {
                var v = param[1];
                var k = param[0];
                var match = k === key;
                if (match) {
                  return /* tuple */[
                          k,
                          Curry._1(foo, v)
                        ];
                } else {
                  return /* tuple */[
                          k,
                          v
                        ];
                }
              }));
}

function $at$great$great$eq(inst, foo) {
  return Belt_Array.map(inst, (function (param) {
                return Curry._2(foo, param[0], param[1]);
              }));
}

function $$colon(inst, key) {
  var match = Belt_Array.getBy(inst, (function (param) {
          return key.test(param[0]);
        }));
  if (match !== undefined) {
    return match[1];
  }
  
}

function $$colon$colon(inst, key) {
  return Belt_Array.keep(inst, (function (param) {
                return key.test(param[0]);
              }));
}

exports.$at$colon = $at$colon;
exports.$at$pipe = $at$pipe;
exports.$at$colon$bang = $at$colon$bang;
exports.$at$neg = $at$neg;
exports.$at$plus$plus = $at$plus$plus;
exports.$at$plus = $at$plus;
exports.$at$hash = $at$hash;
exports.$at$hash$great = $at$hash$great;
exports.$at$great$great$eq = $at$great$great$eq;
exports.$$colon = $$colon;
exports.$$colon$colon = $$colon$colon;
/* No side effect */
