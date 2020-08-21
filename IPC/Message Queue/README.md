# Message Queue
A message queue can be thought of as a linked list of messages. Threads with adequate permission can put messages onto the queue, and threads with adequate permission can remove
messages from the queue. No requirement exists that someone be waiting for a message to arrive on a  queue before some process writes a message to that queue. This is in contrast
to both pipes and FIFOs, for which it having a writer makes no sense unless a reader also exists.

### This section shows example of client-server implementation using Message Queue.

The `mq_open` function creates a new message queue or opens an existing message queue. The return value from `mq_open` is `message queue descriptor` which is used as the first
argument to the remaining message queue functions. <br />

The `mq_close` function closes an open queue.

The `mq_unlink` function remove a `name` that was used as an argument to `mq_open` from the system.

The `mq_getattr` function used to get attributes of queue.
> The attributes of queue are: 
>
> -> mq_flags: message queue flag: 0, O_NONBLOCK <br />
> -> mq_maxmsg:  max number of messages allowed on queue <br />
> -> mq_msgsize: max size of a message (in bytes) <br />
> -> mq_curmsgs: numeber of messages currently on queue <br />

The `mq_setattr` function sets the attributes for the queue.
> But only mq_flag can be set or cleared with this function. The maximum number of message per queue and the maximum number of bytes per message can be set only
when queue is created, and the number of messages currently on the queue can be fetched but not set.

The `mq_send` and `mq_receive` functions:
> These two functions are used to place a message onto a queue and take a message off a queue respectively. Every message has a priority. `mq_receive` always returns the oldest message of the highest priority from the specified queue.

The `mq_notify` function:
> Posix message queues allow for an `asynchronous event notification` when a message is placed onto an empty message queue. This notification can be either: the generation of a signal, or the creation  of a thread to execute a specified function. <br />
>
> This function establishes or removes the asynchronous event notification for the specified queue.
