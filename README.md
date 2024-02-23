## Additions/Corrections done:

- Added header-files for all `.c` files and used them where necessary
- in `IKBasic.h` added type deifnitions for `Short`, `Float` and `Boolean`
- `SaurM` calls in the original listening a function called `Draik4`, which is not present in the code. I replaced it with the function name `DraikM` that is present.
- `BuchM`, `DraikM` and `SplineM` contained most likely logical OR operators (`||`). But the char (`|`) was probably not present in the font used to print the listing and was presumably replaced by a space in the original publication. I replaced the spaces with the char (`|`).
- Light code and comment formatting without changing the original code-logics

## Source

```plaintext
Book Title: Digital Typefaces
Book Subtitle: Description and Formats
Authors: Peter Karow
Publisher: Springer Berlin, Heidelberg
Softcover ISBN: 978-3-642-78107-0 (19 January 2012)
eBook ISBN: 978-3-642-78105-6 (06 December 2012)
Number of Pages: XIII, 448
```
