# TODO

## Question 1
- we'll need to partition the array into sections so that threads are not interfering with one another
- for each partition, iterate for the assigned number of non-skip nodes or until null (end of linked list is reached)

## Question 2
- write a function that determines if the DFA accepts or rejects an input. Will have to determine how to make the start state of the DFA accessible. Actually, since we'll need to start from any state, maybe the function should also take the state to start at.
