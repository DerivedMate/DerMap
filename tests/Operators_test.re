open DerMap;
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
  ("James", {petsName: "Poppy", age: 27}),
|];

describe("(@:) Accessor", () => {
  open! Expect.Operators;

  test("Gets a prmitive value", () =>
    expect(currencies @: "USD") === Some(100)
  );

  test("Gets a complex value", () =>
    switch (people @: "Bob") {
    | Some(p) => expect(p.petsName) === "Fluffykins"
    | None => expect(true) === false
    }
  );

  test("Returns `None` when didn't find an entry", () =>
    expect(people @: "Jon") === None
  );
});

describe("(@!:) Unsafe Accessor", () => {
  open! Expect.Operators;

  test("Gets a prmitive value", () =>
    expect(currencies @:! "USD") === 100
  );

  test("Gets a complex value", () =>
    expect((people @:! "Bob").petsName) === "Fluffykins"
  );
  // Todo: test if throws if didn't find an entry
});

describe("(@-) Deleter", () => {
  test("Deletes a primitive value", () =>
    expect((currencies @- "PLN")->Belt.Array.every(((k, _)) => k != "PLN"))
    |> toBe(true)
  );
  test("Deletes a colplex value", () =>
    expect((people @- "Bob")->Belt.Array.every(((k, _)) => k != "Bob"))
    |> toBe(true)
  );
  test("Deletes a single entry", () =>
    expect(
      (people @- "James")
      ->Belt.Array.every(((k, v)) =>
          !(k === "James" && v.petsName == "Bary")
        ),
    )
    |> toBe(true)
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
  test("Edits a single primitive", () =>
    expect(
      ([|("PLN", 393), ("PLN", 400), ("USD", 100)|] @# ("PLN", 401))
      @:: "PLN",
    )
    |> toEqual([|401, 400|])
  );

  test("Edits a single complex value", () =>
    expect(
      (people @# ("James", {petsName: "Bobby", age: 28}))
      @<|> ((_, p) => (p.petsName, p.age)),
    )
    |> toEqual([|("Fluffykins", 23), ("Bobby", 28), ("Poppy", 27)|])
  );
});

describe("(@#>) Editor", () => {
  test("Edits a single primitive", () =>
    expect(
      ([|("PLN", 393), ("PLN", 400)|] @#> ("PLN", c => c * 2)) @:: "PLN",
    )
    |> toEqual([|786, 400|])
  );

  test("Edits a single complex value", () =>
    expect(
      (people @#> ("James", p => {age: p.age * 2, petsName: p.petsName}))
      @<|> ((_, p) => (p.petsName, p.age)),
    )
    |> toEqual([|("Fluffykins", 23), ("Bary", 52), ("Poppy", 27)|])
  );
});

describe("(@##) Multi Editor", () => {
  test("Edits many primitives", () =>
    expect(([|("PLN", 393), ("PLN", 400)|] @## ("PLN", 401)) @:: "PLN")
    |> toEqual([|401, 401|])
  );

  test("Edits many complex values", () =>
    expect(
      ((people @## ("James", {age: 2, petsName: "Jake"})) @:: "James")
      ->Belt.Array.map(p => p.age),
    )
    |> toEqual([|2, 2|])
  );
});