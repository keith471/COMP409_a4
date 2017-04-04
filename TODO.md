# TODO

- MODIFY BUILD script for trottier computers

## Question 1
- write up document

## Question 2
- we want every legal, non-overlapping floating point value
- we want the longest matches possible
    - what about 9.011111.12? Do we take 9.01111 and 1.12 or 9.0 and 11111.12?
- each thread should convert every character that would not be part of the longest match in a sequential search to the space character
- so we want output of `39.0x..06.x9.3.4.66` to be `39.0      9.3 4.66`?



## Questions
- "of course when all five possibilities converge, the thread may start normal, sequential recognition"
    - this implies that each thread is actually five threads.
    - I think what this means is we should simulate all five possibilities one step at a time (sequentially), and if all end up in the same state, then we can just simulate one of them from there on out
