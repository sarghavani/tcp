# TCP CLIENT
This is a simple program to connect the client to the server in the socket programing with epoll use edge-triggerd in C languge.
# How to clone
```sh
git clone git@github.com:sarghavani/tcp-client.git
```
# Make
```sh
cd tcp-client

make build
```
# How to use
```sh
open two tabs in the terminal
in two tab:
cd tcp-client

tab one :
nc -l localhost 8888

tab two :
./client
send massage for server.
```
