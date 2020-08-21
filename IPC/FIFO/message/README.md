The example shown for pipes and FIFOs have used the stream I/O. Reads and writes do not examine the data at all. The data is a `byte stream` with no interpretation done by the 
system. If any interpretation is desired, the writing process and the reading process must agree to it a prior and do it themselves.<br />
Sometimes an application wants to impose some structure on the data being transferred. This can happen when the data consists of variable-lenght messages and the reader must know
where the message boundaries are so that it knows when a single message has been  read.<br />

Here, one structure `struct mymesg` is added which stores length of message, type of message and data itself. This structure is used with FIFOs in client-server implementation 
example.
