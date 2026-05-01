libulz - A collection of functions and data structures for C development
========================================================================

'tis my collection of useful/re-usable C code i developed over the
years mostly for personal use, but sharing it with the world.

my main focus was always "small and simple" - especially when it comes to
binary size.
that's why most functions live in their own .c file, so even with a dumb
linker one gets beautifully tiny binaries.
(a smart compiler has -ffunction-sections, -fdata-sections and the smart
linker -Wl,-gc-sections to make good use of those sections).

most code is very well tested in countless of real-life apps that are
in use since years, so it can be considered quite stable - especially
files that have been in this repo for over a decade.

as it grew organically, some of the stuff is not the most beautiful,
and especially naming conventions aren't consistent at all.
in that regard it's similar to ncurses.

the scope of the collection is similar to the python standard library:
almost everything one often needs.
it contains hash functions like md5, sha512, base64 en/decoding, crc32,
code to deal with filesystems and filenames, time, processes, etc.

to learn more about the contents just take a look at the headers in include/.
they also serve as documentation, plus the respective examples/ and tests/.

personal favorites:
-------------------

- sblist.h is by far my most often used piece of code.
  it's an efficient implementation of a dynamic array, or what C++ people
  would call a vector. even though extremely simple, it's also extremely
  useful, especially since it can save stuff by value, circumventing the need
  to manually allocate and free every single item.
  it's also a lot faster than linked lists due to how cpu caches work.
  it comes with a lot of useful functions such as sblist_push() and pop()
  which turns it into a stack, insert, delete, binary search, sort function,
  and sorted insertion.

- tlist.h is a treap list, similar to sblist it can be used as dynamic
  array but has perfect O(log N) performance when deletion and insertion
  is one of the main usecases. currently offers only basic functionality.

- proclib.h features functionality to spawn a process while being able
  to control stdin, stdout, and stderr. basically what popen() should've been.

- ht.h is a hashtable for generic numeric keys with superb performance and
  even better storage properties. overhead can be as low as 34bit per slot.
  ships as a "single-header library".

- logger.h is a different approach to printing mixed content that's
  extremely tiny, unlike a full-blown printf engine. stdio-replacement.h
  builds on the same concept and can give you most of what printf offers
  in a few hundred bytes of object code vs tens of kilobytes.

there's also some things here that i'm not that proud of, like my early
command line option parser and my early hashmap implementations which
aren't that ergonomic to use (looking at you, hbmap).
also i made the typical beginner mistake to make my own string type
(pointer + length) because muh performance. turns out that kernighan and
ritchie weren't stupid when they designed the C string.

recommended usage
-----------------
for quick prototyping, i use my custom rcb2[0] build tool, which automatically
finds necessary .c files by parsing pragmas in headers.
all libulz components provide these pragmas.
`rcb2` main.c - and it automatically collects everything needed and links it
together. however the tool is written in python2 and many distros have removed
support for that. so i'd just recommend to copy/paste the desired bits and
pieces into your project.
APIs are mostly stable, but not entirely. so this is not a library to encounter
in your distro's /usr/lib.

LICENSE
-------
unless otherwise noted, LGPL 2.1+. (see LICENSE).
some files have even more liberal licenses like MIT, if so it's noted
in the respective .h or .c files.

[0]: https://github.com/rofl0r/rcb2
