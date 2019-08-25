open ReMap;
open ReMap.Operators;
open Jest;
open Expect;

let currencies: t(int) = [|("USD", 100), ("PLN", 393)|];
type person = {
  petsName: string,
  age: int,
};
let people: t(person) = [|
  ("Bob", {petsName: "Fluffykins", age: 23}),
  ("James", {petsName: "Bary", age: 26}),
|];

describe("(@:) Accessor", () => {
  test("Gets a prmitive value", () =>
    expect(currencies @: "USD") |> toEqual(Some(100))
  );

  test("Gets a complex value", () =>
    switch (people @: "Bob") {
    | Some(p) => expect(p.petsName) |> toEqual("Fluffykins")
    | None => expect(true) |> toEqual(false)
    }
  );

  test("Returns `None` when didn't find an entry", () =>
    expect(people @: "Jon") |> toEqual(None)
  );
});

describe("(@!:) Unsafe Accessor", () => {
  test("Gets a prmitive value", () =>
    expect(currencies @:! "USD") |> toEqual(100)
  );

  test("Gets a complex value", () =>
    expect((people @:! "Bob").petsName) |> toEqual("Fluffykins")
  );
  // Todo: test if throws if didn't find an entry
});

describe("(@-) Deleter", () => {
  test("Deletes a primitive value", () =>
    expect((currencies @- "PLN") @: "PLN") |> toBe(None)
  );
  test("Deletes a colplex value", () =>
    expect((people @- "Bob") @: "Bob") |> toBe(None)
  );
});

describe("(@+ | @++) Appender", () => {
  test("Adds a primitive", () =>
    expect(
      (currencies @+ ("GBP", 82))
      ->Belt.Array.some(((k, v)) => k == "GBP" && v == 82),
    )
    |> toBe(true)
  );

  test("Doesn't add a key copy", () =>
    expect([|("PLN", 393)|] @++ [|("PLN", 432), ("GBP", 82)|])
    |> toEqual([|("PLN", 393), ("GBP", 82)|])
  );
});

describe("(@#) Editor", () => {
  test("Edits a primitive", () =>
    expect(([|("PLN", 393), ("USD", 100)|] @# ("PLN", 401)) @: "PLN")
    |> toEqual(Some(401))
  );

  test("Edits a complex value", () =>
    expect(
      (people @# ("James", {petsName: "Bobby", age: 28}))
      @>>= ((_, p) => (p.petsName, p.age)),
    )
    |> toEqual([|("Fluffykins", 23), ("Bobby", 28)|])
  );
});

describe("(@#>) Editor Function", () => {
  test("Edits a primitive", () =>
    expect(
      ([|("PLN", 393), ("USD", 100)|] @#> ("PLN", c => c * 2)) @: "PLN",
    )
    |> toEqual(Some(786))
  );

  test("Edits a complex value", () =>
    expect(
      (people @#> ("James", p => {age: p.age * 2, petsName: p.petsName}))
      @>>= ((_, p) => (p.petsName, p.age)),
    )
    |> toEqual([|("Fluffykins", 23), ("Bary", 52)|])
  );
});

describe("(@>>:) Collect", () =>
  test("Collects entries", () =>
    expect(currencies @>>: [|"PLN", "USD"|])
    |> toEqual([|("USD", 100), ("PLN", 393)|])
  )
);