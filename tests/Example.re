open ReMap.Operators;
open Jest;
open Expect;

// -------- Data -------- //
let fetchedKittens = [|
  ("Fluffykins#001", [|("Age", "3"), ("Breed", "Ragdoll")|]),
  ("Fluffykins#002", [|("Age", "1"), ("Breed", "Ragdoll")|]),
  ("Toby#001", [|("Age", "3"), ("Breed", "Birman")|]),
|];

describe("README Example", () => {
  test("Get Fluffykins (@:)", () =>
    expect(fetchedKittens @: "Fluffykins#001")
    |> toEqual(Some([|("Age", "3"), ("Breed", "Ragdoll")|]))
  );

  test("Get Fluffykins (@:!)", () =>
    expect(fetchedKittens @:! "Fluffykins#001")
    |> toEqual([|("Age", "3"), ("Breed", "Ragdoll")|])
  );

  test("Get age (@:!)", () => {
    let fk = fetchedKittens @:! "Fluffykins#001";
    expect(fk @:! "Age") |> toEqual("3");
  });

  test("Update age (@#)", () => {
    let fk = fetchedKittens @:! "Fluffykins#001";
    let newKittens =
      fetchedKittens @# ("Fluffykins#001", fk @# ("Age", "4"));

    expect(newKittens)
    |> toEqual([|
         ("Fluffykins#001", [|("Age", "4"), ("Breed", "Ragdoll")|]),
         ("Fluffykins#002", [|("Age", "1"), ("Breed", "Ragdoll")|]),
         ("Toby#001", [|("Age", "3"), ("Breed", "Birman")|]),
       |]);
  });

  test("Update dynamic age (@#>)", () => {
    let newKittens =
      fetchedKittens
      @#> (
        "Fluffykins#001",
        fluffykins =>
          fluffykins
          @# (
            "Age",
            int_of_string(fluffykins @:! "Age") + 1 |> string_of_int,
          ),
      );

    expect(newKittens)
    |> toEqual([|
         ("Fluffykins#001", [|("Age", "4"), ("Breed", "Ragdoll")|]),
         ("Fluffykins#002", [|("Age", "1"), ("Breed", "Ragdoll")|]),
         ("Toby#001", [|("Age", "3"), ("Breed", "Birman")|]),
       |]);
  });
});