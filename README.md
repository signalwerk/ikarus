## Ikarus Spline

### Implementation of the Ikarus Spline algorithm in C and TypeScript

This repository contains the original C implementation of the Ikarus Spline algorithm from the book «Digital Typefaces» by Peter Karow and a direct translation to TypeScript of the same algorithm.

## C · original code

The C implementation (`/src/C/lib/`) was taken from the [2012 edition](https://link.springer.com/book/10.1007/978-3-642-78105-6) of the book «Digital Typefaces» by Peter Karow. The reprint of the hardcover 1st edition from 1994.

```plaintext
Book Title: Digital Typefaces
Book Subtitle: Description and Formats
Authors: Peter Karow
Publisher: Springer-Verlag Berlin Heidelberg
Softcover ISBN: 978-3-642-78107-0 (19 January 2012)
eBook ISBN: 978-3-642-78105-6 (06 December 2012)
Number of Pages: XIII, 448
```

In the Apendix W «Programs for converting IK → DI» (page 403) the author provides the code for the Ikarus Spline algorithm. The code is divided into several files (page 406 – 420)

The [1994 version is available on archive.org](https://archive.org/details/digitaltypefaces0000karo/).

In the comments there are some note about versions and edit-history. According to this the code was last edited on 22. 01. 1992 (`BuchM.c`). The first edit was on 20. 07. 1987 (`SplinM.c`).

### Additions/Corrections to the original C code

The original C `/src/C/lib/` was transcribed from the book. The following corrections and additions were made:

- Added header-files for all `.c` files and included where necessary
- Added type deifnitions for `Short`, `Float` and `Boolean` in `IKBasic.h`
- `SaurM` calls in the original listening a function called `Draik4`, which is not present in the code. Was replaced it with the function `DraikM` that is present.
- `BuchM`, `DraikM` and `SplineM` contained most likely logical OR operators (`||`). But the char (`|`) was probably not present in the font used to print the listing and was presumably replaced by a space in the original publication. Spaces were replaced with the char (`|`).
- Light code and comment formatting without changing the original code-logics

## TypeScript · direct translation

The TypeScript implementation (`/src/TypeScript/lib/`) is based on the C implementation. The TypeScript implementation is a direct translation of the C code to TypeScript. The TypeScript implementation is not optimized for performance or memory usage. The following points are important to note about the TypeScript implementation:

- Follows exact the same structure as in C (same function names)
- The C pointers were convertet to pass objects in TypeScript
- The input knots are mutated (like in C)
- Output matches the C output

## Example and usage

There are examples in `src/C/main.c` and `src/TypeScript/main.ts` reading a CSV with IK points and converts them to a JSON with curve- and line-segments.

There are four types of points in the IK format:

| Point type | Internal identifier |
| ---------- | ------------------- |
| Start      | 12                  |
| Corner     | 13                  |
| Curve      | 14                  |
| Tangent    | 15                  |

## License

### C code

Code in `/src/C/lib/*.c`

In the book «Digital Typefaces» by Peter Karow, the author states the following in the appendix (page 403) of the book:

> Everyone is free to copy the
> C-listings in this chapter.
> We waive our copyright.

### The rest of the repository

[MIT License](https://opensource.org/license/mit)

### Example

The Ikarus-Points for the lowercase letter a in `data/IkarusNimbusSansRegular.csv` are taken from `Ikarus M 2.5 Demo` that comes with two example fonts. The CSV was created by exporting the IK points from the file `NIMBUS Sans regular.IK` and then extracted by hand (see `data/IkarusNimbusSansRegular.png`).
