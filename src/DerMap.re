open Belt;
open Belt.Array;

type t('a) = array((string, 'a));
type mapCallback('a, 'b) = (string, 'a) => 'b;
type filterCallback('a) = string => bool;

type key =
  | String(string)
  | Regex(Js.Re.t);

/**
 * Operator Syntax
 * Operators should concist of 2-3 characters in the following format:
 * `@[operator][modifier?]`   i.e. @:! <=> @[access][unsafe];
*/

/**
Access a value by the key in a safe way.
```reason
[|("PLN", 393)|] @: "PLN" = option(393)
```
*/
let (@:) = (inst: t('a), key: string) =>
  switch (inst->getBy(((k, _)) => k == key)) {
  | Some((_, v)) => Some(v)
  | None => None
  };

/**
Get the value or a default value.
```reason
[|("PLN", 393)|] @| ("GBP", 82) = 82
```
*/
let (@|) = (inst: t('a), (key, def): (string, 'a)) =>
  switch (inst->Belt.Array.getBy(((k, _)) => k == key)) {
  | Some((_, v)) => v
  | None => def
  };

/**
Get a value by the key in an unsafe way (omits `option('a)`).
```reason
[|("PLN", 393)|] @:! "PLN" = 393
[|("PLN", 393)|] @:! "USD" = raise
```
 */
let (@:!) = (inst: t('a), key: string) => {
  let (_, v) = inst->getBy(((k, _)) => k == key)->Option.getExn;
  v;
};

/**
Delete an entry by key w/o mutation
```reason
[|("PLN", 393), ("USD", 100)|] @- "PLN" = [|("USD", 100)|]
[|("PLN", 393), ("PLN", 394), ("USD", 100)|] @- "PLN" = [|("PLN", 394), ("USD", 100)|]
```
  */
let (@-) = (inst: t('a), key: string) => inst->keep(((k, _)) => key != k);

/**
 * Appends entries w/o entries
 * ```reason
 * [|("PLN", 393)|] @++ [|("GBP", 82), ("USD", 100)|] =  [|("PLN", 393), ("GBP", 82), ("USD", 100)|]
 *
 */
let (@++) = (inst: t('a), entries: array((string, 'a))) =>
  keep(entries, ((k, _)) => !some(inst, ((k2, _)) => k == k2))
  |> concat(inst);

/**
 * Appends an entry w/o mutation.
 * ```reason
 * [|("PLN", 393)|] @+ ("GBP", 82) = [|("PLN", 393), ("GBP", 82)|]
 * ```
 */
let (@+) = (inst: t('a), entry: (string, 'a)) => inst @++ [|entry|];

/**
 * Edits a matching entry.
 * ```reason
 * [|("PLN", 393), ("PLN", 400)|] @# ("PLN", 390)
 *  = [|("PLN", 390), ("PLN", 400)|]
 * ```
 */
let (@#) = (inst: t('a), (key, newVal): (string, 'a)) =>
  inst->map(((k, v)) => k == key ? (k, newVal) : (k, v));

/**
 * Edits a matching entry using a function
 * ```reason
 * [|("PLN", 100)|] @#> ("PLN", c => c * 2)
 *  = [|("PLN", 200)|]
 * ```
 */
let (@#>) = (inst: t('a), (key, foo): (string, 'a => 'a)) =>
  inst->map(((k, v)) => k == key ? (k, foo(v)) : (k, v));

let (@>>=) = (inst: t('a), foo: (string, 'a) => 'b) =>
  inst->map(((k, v)) => foo(k, v));

// -------- Regex operators -------- //

/**
 * Accesses an entry whose key matches the given regex.
 * ```reason
 * [|("Nil:01", 0), ("Pol:01", 1)|] $: [%re "/^Pol/"]
 *  = Some(1)
 * ```
 */
let ($:) = (inst: t('a), key: Js.Re.t) =>
  switch (inst->getBy(((k, _)) => Js.Re.test_(key, k))) {
  | Some((_, v)) => v
  | None => None
  };

/**
 * Aggregates entries whose keys match the given regex.
 * ```reason
 * [|("Nil:01", 0), ("Pol:01", 1), ("Pol:02", 2)|] $:: [%re]
 *  = [|("Pol:01", 1), ("Pol:02", 2)|]
 * ```
 */
let ($::) = (inst: t('a), key: Js.Re.t) =>
  inst->keep(((k, _)) => Js.Re.test_(key, k));