open Jest;
open Expect;
open ReMap;
open Operators;

describe("Inline: Operators", () => {
  test("@:", () =>
    expect([|("PLN", 393)|] @: "PLN") |> toEqual(Some(393))
  );

  test("@|", () =>
    expect([|("PLN", 393)|] @| ("GBP", 82)) |> toEqual(82)
  );

  test("@:!", () =>
    expect([|("PLN", 393)|] @:! "PLN") |> toEqual(393)
  );

  test("@-", () =>
    expect([|("PLN", 393), ("USD", 100)|] @- "PLN")
    |> toEqual([|("USD", 100)|])
  );

  test("@++", () =>
    expect([|("PLN", 393)|] @++ [|("GBP", 82), ("USD", 100)|])
    |> toEqual([|("PLN", 393), ("GBP", 82), ("USD", 100)|])
  );

  test("@+", () =>
    expect([|("PLN", 393)|] @+ ("GBP", 82))
    |> toEqual([|("PLN", 393), ("GBP", 82)|])
  );

  test("@#", () =>
    expect([|("PLN", 393), ("USD", 100)|] @# ("PLN", 390))
    |> toEqual([|("PLN", 390), ("USD", 100)|])
  );

  test("@#>", () =>
    expect([|("PLN", 100)|] @#> ("PLN", c => c * 2))
    |> toEqual([|("PLN", 200)|])
  );

  test("@>>:", () =>
    expect(
      [|("PLN", 393), ("USD", 100), ("GBP", 82)|] @>>: [|"USD", "GBP"|],
    )
    |> toEqual([|("USD", 100), ("GBP", 82)|])
  );

  test("$:", () =>
    expect([|("Nil:01", 0), ("Pol:01", 1)|] $: [%re "/^Pol/"])
    |> toEqual(Some(1))
  );

  test("$::", () =>
    expect(
      [|("Nil:01", 0), ("Pol:01", 1), ("Pol:02", 2)|] $:: [%re "/^Pol/"],
    )
    |> toEqual([|("Pol:01", 1), ("Pol:02", 2)|])
  );
});