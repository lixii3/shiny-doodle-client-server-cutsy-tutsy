# TCP client and server

These programs send a message from a terminal client to a TCP server. The
server listens on all of its machine's network interfaces. The default port is
`12123`.

## Requirements

- Linux or another POSIX-compatible system
- GCC

## Build

Run these commands from this directory:

```sh
gcc -Wall -Wextra -I.. server1_INET.c -o server
gcc -Wall -Wextra client1_INET.c -o client
```

To use a different port, define `SERVERPORT` when compiling **both** programs.
Choose an available TCP port from `1` to `65535`; for example:

```sh
gcc -Wall -Wextra -I.. -DSERVERPORT=23456 server1_INET.c -o server
gcc -Wall -Wextra -DSERVERPORT=23456 client1_INET.c -o client
```

The port is set at build time, so rebuild both binaries if you change it.

## Run on different machines on the same network

1. On the machine that will host the server, build the server with the chosen
   port and start it:

   ```sh
   ./server
   ```

2. Find the server machine's IPv4 address on the local network (for example,
   `192.168.1.20`). Allow inbound TCP traffic on the chosen port in the server
   machine's firewall, if its firewall is enabled.

3. On each client machine, build `client` using the **same**
   `SERVERPORT` value used to build the server. Send a message from the
   terminal, replacing the address with the server's IPv4 address:

   ```sh
   ./client 192.168.1.20 "Hello from another machine"
   ```

   The client prints the server's reply and exits. Quote messages containing
   spaces. For a client running on the same machine as the server, the server
   address can be omitted:

   ```sh
   ./client "Hello"
   ```

Stop the server with `Ctrl+C`.

## Important note

The current server also attempts to execute each received message as a
program. Run it only on a network you trust, and do not send untrusted input.
Messages are read in chunks of up to 31 bytes by the server.

![Diagram](.ico)
