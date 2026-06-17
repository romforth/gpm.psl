# A reimplementation of Christopher Strachey's GPM in C

This is a "bug compatible" conversion into C of Patty S. Loughney's GPM
implementation in Rust which is available at: https://github.com/psl8/gpm

That code in turn is a reimplementation of Christoper J. Strachey's
GPM converted to Rust from CPL (which I've now translated to C).

It is pretty much a one-to-one match of the Rust code except for the
places where I made the syntax sugar changes to GPM to match the syntax
that I had used earlier for the implementation that I had tried earlier
at https://github.com/romforth/gpm

For the Rust to C conversion, I used a helper Perl script called convert
(which is part of this repo) to handle about ~80% of the work and then
resorted to manually fixing up the rest of the code which could not be
handled by a simple minded line level regexp replacement.
