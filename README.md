# ReMap
> Operators for working with JSON objects of unknown structure.

## Content
- [ReMap](#remap)
  - [Content](#content)
  - [Installation](#installation)
  - [Motivation](#motivation)
  - [Example](#example)
  - [Different forms](#different-forms)

## Installation
You can get yourself a brand new copy of RieMap for **FREE**! (crazy, I know...)
Just use the following command:
```sh
npm i riemap
```
## Motivation
Long story short, I was working on a CMS system, which allowed users to edit db models, thus I didn't know data-types of fetched items ahead of time. As you may know, Reason doesn't allow for dynamic records. It made my use arrays of tuples, a.k.a. `(key => value)` pairs. Dealing with it was kinda tedious, so I decided to write a simple lib around it.

## Example
Let's pretend a user wants to access a collection of kittens. Needless to say you had to prepare the structure in advance on the backend.
```reason
let fetchedKittens = Fetch.kittensSync() |> Json.parseDynamic
/*
  => [|
    ("Fluffykins#001", [|
      ("Age", "3"), <-- They all have to be of the same type;
      ("Breed", "Ragdoll")
    |]),
    ("Fluffykins#002", [|
      ("Age", "1"),
      ("Breed", "Ragdoll")
    |]),
    ("Toby#001", [|
      ("Age", "3"),
      ("Breed", "Birman")
    |])
  |]
*/
```
So now you've got this ugly mess of a structure, just because the user may want to change the fields anytime. Good for you!(*unsarcastic cough*)</br>
Alright, time to deal with it! Let's start with simply getting the `Fluffykins#001`.
```reason
let fluffykins001 = fetchedKittens @: "Fluffykins#001"
/*
  => Some([|
    ("Age", "3"),
    ("Breed", "Ragdoll")
  |])
*/
```
Oh... forgot to tell you that `@:` (or if you prefer the function form `get()`) returns an `option('a)`, as what would happen if you tried accessing let's say, `Bobby#001`? As you can see, such kitten doesn't exist, so instead of throwing an error, you would just get `None` and let the user know.</br>
But if you're feeling adventures, you may just use `@:!` (or `getUnsafe()`). Let's stick with it just for simplicity's sake.
```reason
let fluffykins001 = fetchedKittens @:! "Fluffykins#001"

// Now let's say that we want to get it's breed.
let breed = fluffykins001 @:! "Breed" 
// => "Ragdoll"
```
Cool things, right? Ok, let's say it's just been Fluffykins's birthday (Happy birthday Fluffy :tada:), so now it's 4, not 3. Time to update that!
```reason
let newKittens = fetchedKittens 
  @# (
    "Fluffykins#001", 
    fluffykins001 @# ("Age", "4")
  )

/*
  => [|
    ("Fluffykins#001", [|
      ("Age", "4"), <-- Now it's 4 years old!
      ("Breed", "Ragdoll")
    |]),
    ("Fluffykins#002", [|
      ("Age", "1"),
      ("Breed", "Ragdoll")
    |]),
    ("Toby#001", [|
      ("Age", "3"),
      ("Breed", "Birman")
    |])
  |]
*/
```
Let me quickly walk you through. `@#` takes a key (`Fluffykins#001`) and a new value for the entry. The value is our `fluffykins001` with the changed age.
Ok, that was pretty awesome, but we hard-coded the age of 4. Let's rewrite the code such that it uses a dynamic age.
```reason
let newKittens = fetchedKittens
  @#> (
    "Fluffykins#001", 
    fluffykins => fluffykins 
      @# (
        "Age",
        (int_of_string(fluffykins @:! "Age") + 1)
          |> string_of_int
      )
  )
```
Here's how the `@#>` operator works. Instead of taking a new value, it takes a function `('a) => 'a`.

## Different forms

| Function                                   | Operator |
| ------------------------------------------ | -------- |
| get(t('a), string) => option('a)           | @:       |
| getOr(t('a), string, 'a) => 'a             | @\|      |
| getUnsafe(t('a), string) => 'a             | @:!      |
| remove(t('a), string) => t('a)             | @-       |
| concat(t('a), t('a)) => t('a)              | @++      |
| push(t('a), 'a) => t('a)                   | @+       |
| replace(t('a), string, 'a) => t('a)        | @#       |
| freplace(t('a), string, 'a => 'a) => t('a) | @#>      |
| map(t('a), string, 'a => 'b) => array('b)  | @>>=     |
| getRe(t('a), Js.Re.t) => array('a)         | $:       |
| aggregate(t('a), Js.Re.t) => t('a)         | $::      |