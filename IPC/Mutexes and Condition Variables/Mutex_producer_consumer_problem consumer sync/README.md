### Producer-consumer problem
In this example 'mutex' is used to synchronize the multiple producers threads who are writing data to shared buffer simulataneously. Consumer then varify the data in the buffer. 
(Here consumer thread also initiated when all the threads are created. Consumer thread is synchronized with producer threads with wait function implementation to wait for data available to read from producers.)
