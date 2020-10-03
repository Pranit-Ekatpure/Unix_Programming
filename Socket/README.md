# Socket Programming
This section contain examples on how to use the protocols in the TCP/IP suite: TCP, UDP and SCTP. These transport protocols use the network-layer protocol IP, either IPv4 or IPv6.

To perform network I/O, desired communication protocol **socket** is created first. It is a endpoint of a two way communication link between two programs running on the network. **Socket programming** is a way of connecting two nodes on a network to communicate with each other.

## TCP
Transmission Control Protocol.
### TCP Features
- Connection-oriented, reliable, full-duplex byte-stream protocol without any record boundary.
- It takes care of details such as acknowledgments, timeouts, retransmissions and the like.
- TCP provides flow control
- It can use either IPv4 or IPv6.
- TCP sockets are an example of *__stream sockets__*.

### TCP Connection Establishment and Termination
#### Three-Way Handshake
The following scenario occurs when a TCP connection is established:
1. The server must be prepared to **accept** an incoming connection. This is done by calling **socket**, **bind**, and **listen** and is 		called a *passive open*.
2. The client issues an *active open* by calling **connect**. This causes the client TCP to send a "synchronize" (SYN) segment, which tells the server the client's initials sequence number for the data that the client will send on the connection. Normally, there is no data sent with the SYN; it just contains an IP header, a TCP header, and possible TCP options.
3. The server must acknowledge (ACK) the client's SYN and the server must also send its own SYN containing the initial sequence	number for the data that the server will send on the connection. The server sends it SYN and the ACK of the client's SYN in a single segment.
4. The client must acknowledge the server's SYN.

The minimum number of packets required for this exchange is three; hence, this is called TCP's *three-way handshake*.
#### TCP Connection Termination
The following scenario occurs during TCP connection termination:
1. One application calls **close** first, and we say that this end performs the *active close*. This end's TCP sends a FIN segment,	which means it is finished sending data.
2. The other end that receives the FIN performs the *passive close*. The received FIN is acknowledged by TCP. The receipt of the FIN is also passed to the application as an end-of-file ( after any data that may have already been queued for the application to receive), since the receipt of the FIN means the application will not receive any additional data on the connection.
3. Sometime later, the application that received the end-of-file will close its socket. This causes its TCP to send a FIN.
4. The TCP on the system that receives this final FIN (the end that did the active close) acknowledges the FIN.

Since a FIN and an ACK are required in each direction, four segments are normally required.	The sending of each FIN occurs when a socket is closed. Here the appliacation calls **close** for this to happen, but when a process terminates, either voluntarily or involuntarily, all open descriptors are closed, which will also cause a FIN to be sent on any TCP connection that is still open. Either end --the client or the server -- can perform the active close.

## UDP
User Datagram Protocol.

### UDP Features
- Connectionless, unreliable, datagram protocol: datagram has a lenght and it is passed to the receiving application along with the data.
- No guarantee of datagram reaching the intended destination.
- It can use either IPv4 or IPv6.
- UDP sockets are an example of *__datagram sockets__*.
- UDP can be full-duplex.

### UDP communication
UDP provides a *connectionless* service, as there need not be any long-term relationship between a UDP client and server. For example, a UDP client can create a socket and send a datagram to a given server and then immediately send another datagram on the same socket to a different server. Similarly, UDP server can receive several datagram on a single UDP socket, each from a different client.

## SCTP
Stream Control Trasmission Protocol.
### SCTP Features
- Connection-oriented protocol that provides full-duplex association. The word "association" is used when referring to a connection in SCTP because SCTP is multihomed, involving a set of IP addresses and a single port for each side of an association.
- It also provide reliability, sequencing, flow control, like TCP.
- Unlike TCP, SCTP is message-oriented.
- SCTP provide multiple streams between connection endpoints, each with its own reliable sequenced delivery of messages. A lost message in one of these streams does not block delivery of messages in any of the other streams. This is in contrast to TCP, where a loss at any point in the single stream of bytes blocks delivery of all future data on the connection until the loss is repaired, which is called head-of-line blocking. SCTP minimizes head-of-line blocking.
- As SCTP provides a multihoming feature, which allows a single SCTP endpoint to support multiple IP addresses, it provide increased robustness against network failure. An endpoint can have multiple redundant network connections, where each of these networks has a different connection to the Internet infrastructure. It can work around a failure of one network or path across the Internet by switching to another address associated with the SCTP association.
- As with TCP and UDP, SCTP can use either IPv4 or IPv6, but is can also use both IPv4 and IPv6 simultaneously on the same association.

### SCTP Association Establishment and Termination
#### Four-Way Handshake
The following scenario occurs when an SCTP association is established:
1. The server must be prepared to accept an incoming association. This preparation is normally done by calling **socket**, **bind**, and **listen** and is called a *passive open*.
2. The client issues an *active open* by calling **connect** or by **sending a message**, which implicitly opens the association. This causes the client SCTP to send an INIT message to tell the server the client's list of IP addresses, initial sequence number, initiation tag to identify all packets in this association, number of outbound streams the client is requesting, and number of inbound streams the client can support.
3. The server acknowledges the client's INIT message with an INIT-ACK message, which contains the server's list of IP addresses, initial sequence number, initiation tag, number of outbound streams the server is requesting, number of inbound stream the server 
can support, and a state cookie. The state cookie contains all of the state that the server needs to ensure that the association is valid, and is digitally signed to ensure its validity.
4. The client echos the server's state cookie with a COOKIE-ECHO message. This message may also contain user data bundled within the same packet.
5. The server acknowledges that the cookie was correct and that the association was established with a COOKIE-ACK message. This message may also contain user data bundled within the same packet.

The minimum number of packets required for this exchange is four; hence, this process is called SCTP's *four-way handshake*.
#### Association Termination
- Unlike TCP, SCTP does not permit a "half-closed" association. When one end shuts down an association, the other end must stop sending new data. The receiver of the shutdown request sends the data that was queued, if any, and then completes the shutdown.