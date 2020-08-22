### Producer-consumer example
In this example `mutex` is used to synchronize the multiple producers threads who are writing data to shared buffer simulataneously. Consumer then varify the data in the buffer. (Here consumer thread is started when all the producer threads done the job and terminates.)
