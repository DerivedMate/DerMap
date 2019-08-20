type t('a) = array((string, 'a));
/**
 * Operator Syntax
 * Operators should concist of 2-3 characters in the following format:  * `@[operator][modifier?]`   i.e. @:! <=> @[access][unsafe];
*/

/**
Access a value by the key in a safe way.
```reason
[|("PLN", 393)|] @: "PLN" = option(393)
```
*/
let (@:) = (inst: t('a), key: string) =>
  switch (inst->Belt.Array.getBy(((k, _)) => k == key)) {
  | Some((_, v)) => Some(v)
  | None => None
  };

/**
Access a value by the key in a safe way.
```reason
[|("PLN", 393), ("PLN", 400), ("USD", 100)|] @:: "PLN" = [|393, 400|]
```
*/
let (@::) = (inst: t('a), key: string) =>
  inst->Belt.Array.keepMap(((k, v)) => k == key ? Some(v) : None);

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
  let (_, v) =
    Belt.(inst->Array.getBy(((k, _)) => k == key)->Option.getExn);
  v;
};

/**
Delete an entry by key w/o mutation
```reason
[|("PLN", 393), ("USD", 100)|] @- "PLN" = [|("USD", 100)|]
[|("PLN", 393), ("PLN", 394), ("USD", 100)|] @- "PLN" = [|("PLN", 394), ("USD", 100)|]
```
  */
let (@-) = (inst: t('a), key: string) => {
  let deleted = ref(false);
  inst->Belt.Array.keep(((k, _)) =>
    if (deleted^) {
      true;
    } else {
      let p = key != k;
      if (!p) {
        deleted := true;
      };
      p;
    }
  );
};

/**
Delete entries by key w/o mutation. Deletes all matching entries.
```reason
[|("PLN", 393), ("USD", 100)|] @- "PLN" = [|("USD", 100)|]
[|("PLN", 393), ("PLN", 394), ("USD", 100)|] @- "PLN" = [|("USD", 100)|]
```
  */
let (@--) = (inst: t('a), key: string) =>
  inst->Belt.Array.keep(((k, _)) => key != k);

/**
 * Appends entries w/o entries
 * ```reasonml
 * [|("PLN", 393)|] @++ [|("GBP", 82), ("USD", 100)|] =  [|("PLN", 393), ("GBP", 82), ("USD", 100)|]
 *
 */
let (@++) = (inst: t('a), entries: array((string, 'a))) =>
  Belt.Array.(
    keep(entries, ((k, _)) => !some(inst, ((k2, _)) => k == k2))
    |> concat(inst)
  );

/**
Appends an entry w/o mutation.
```reasonml
[|("PLN", 393)|] @+ ("GBP", 82) = [|("PLN", 393), ("GBP", 82)|]
```
 */
let (@+) = (inst: t('a), entry: (string, 'a)) => inst @++ [|entry|];

let (@#) = (inst: t('a), (key, newVal): (string, 'a)) => {
  let edited = ref(false);
  Belt.Array.(
    inst->map(((k, v)) => {
      let p = k == key;
      if (edited^ || !p) {
        (k, v);
      } else {
        edited := true;
        (k, newVal);
      };
    })
  );
};

let (@#>) = (inst: t('a), (key, foo): (string, 'a => 'a)) => {
  let edited = ref(false);
  Belt.Array.(
    inst->map(((k, v)) => {
      let p = k == key;
      if (edited^ || !p) {
        (k, v);
      } else {
        edited := true;
        (k, foo(v));
      };
    })
  );
};

let (@##) = (inst: t('a), (key, newVal): (string, 'a)) =>
  Belt.Array.(inst->map(((k, v)) => k == key ? (k, newVal) : (k, v)));

let (@##>) = (inst: t('a), (key, foo): (string, 'a => 'a)) =>
  Belt.Array.(inst->map(((k, v)) => k == key ? (k, foo(v)) : (k, v)));

let (@<|>) = (inst: t('a), foo: (string, 'a) => 'b) =>
  inst->Belt.Array.map(((k, v)) => foo(k, v));