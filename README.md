stack-distance
==============

Utility to simulate cache behavior with Mattson's Stack Algorithm.

Mattson's Stack Algorithm is used to estimate the cache miss rate for a given
workload, with *any* sized cache. Given a trace of memory/disk/object
accesses, it can be used to find a cache size that would be appropriate for
that workload.

It uses the assumption that most real-world caches behave similarly- enough to
an LRU that the LRU miss rate is "good enough". The algorithm simulates an LRU
cache. For each memory access, it gives the LRU position of that access. By
giving it a stream of object IDs, it will print out a stream of LRU positions.
A cumulative histogram of these LRU positions is a "miss ratio curve": a plot
showing, for each cache size, what the cache hit rate would have been for that
cache size.


Using the utility
----

The `stack-distance` program takes object ids as arbitrary strings on standard
input. For each line of input, it prints an integer LRU position. Here's an
example. Object IDs are letters, LRU positions are numbers, and comments begin
with #.

    a
    9223372036854775807  # INT64_MAX is used for new objects.
    b
    9223372036854775807
    c
    9223372036854775807
    a
    2  # Two unique references since the last reference to a.
    a
    0  # A was the last reference, so it now has a distance of 0.
    b
    2  # Even though a has appeared twice, there are only 2 *unique* references
       # since the last time we saw b.
    a
    1
    c
    2


The output of this utility can be passed to `sort -n | uniq -c` to generate a
histogram.


Computing miss-ratio-curve for a large social network graph
----

    gunzip --stdout twitter_rv.tar.gz  | 
        pv -s 26172280241 -p -t -e -r -a | 
        stack-distance | 
        sort -n | 
        uniq -c > twitter-mrc.txt