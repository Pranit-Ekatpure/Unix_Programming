# FIFO
  FIFO stands for first in, first out, and a Unix FIFO is similar to a pipe. It is a one-way (half-duplex) flow of data. But unlike pipes, a FIFO has a pathname associated with it 
allowing unrelated processes to access a single FIFO. FIFOs are also called `named pipes`. <br />
  A FIFO is created by the `mkfifo` function. To create and open a pipe requires one call to `pipe`. To create and open a FIFO requires one call to mkfifo followed by a call to `open`.
  
### This section shows example of client-server implementation using two FIFOs.
In this example client reads the name of file whose contents are to be read from standard input and send it to the server through first FIFO. 
Server reads the content of file whose name received 
from first FIFO and write it on second FIFO. 
Client reads the content of file from second FIFO and output it on standard output. <br />
Two FIFOs, FIFO1 and FIFO2, are created. The client opens the first FIFO1 in write mode and second FIFO2 in read mode while the server opens first FIFO1 in read mode and second FIFO2 in write mode.
The first FIFO1 is used to send the pathname from the client to the server, and the second FIFO2 is used to send the contents of that file from the server to the client.
