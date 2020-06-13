Mutexes are for locking and cannot be used for waiting. We must synchronize the consumer with the producer to make certian
the consumer processes only data items that have already been stored by the producer. Our consume_wait function must wait 
until the producer have generated the ith item.

The fundamental problem is: what can we do when the desired item is not ready?
All we do is loop around again, unblocking and locking the mutex each time. This is called spinning or polling and is waste
of CPU time.

We could also sleep for a short amount of time, but we do not know how long to sleep. What is needed is another type of
synchronization that lets a thread (or process) sleep until some event occurs.