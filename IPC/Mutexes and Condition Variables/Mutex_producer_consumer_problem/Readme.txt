If we remove the mutex locking from this example, it fails, as expected. That is
the consumer detects many instances of buff[i] not equal to i. We can verify that 
the removal of the mutex locking has no effect if only one producer thread is run.