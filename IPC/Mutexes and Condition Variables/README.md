# Mutexes and Condition Variables
Synchronization is normally needed to allow the sharing of data between threads or processes. Mutexes and condition variables are the building blocks of synchronization.
## Mutexes
A mutex, which stands for `mutual exclusion`, is the most basic form of synchronization. A mutex is used to protect a `critical region`, to make certain that only one thread at a time executes the 
code within the region or that only one process at a time executes the code within the region.
## Condition Variables
A mutex is for `locking` and a condition variable is for `waiting`.
