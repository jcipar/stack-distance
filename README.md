stack-distance
==============

Utility to simulate cache behavior with Mattson's Stack Algorithm.


Computing miss-ratio-curve for a large social network graph
----

    gunzip --stdout twitter_rv.tar.gz  | 
        pv -s 26172280241 -p -t -e -r -a | 
        stack-distance | 
        sort -n | 
        uniq -c > twitter-mrc.txt