# MFTP

MFTP: Multiple File Transfer Program

```
$ ./mftp -h
  __  __ ______ _______ _____  
 |  \/  |  ____|__   __|  __ \ 
 | \  / | |__     | |  | |__) |
 | |\/| |  __|    | |  |  ___/ 
 | |  | | |       | |  | |     
 |_|  |_|_|       |_|  |_|     

MFTP: Multiple File Transfer Program by Deun Lee

Usage: mftp -s      [-p port] [options]
       mftp -c host [-p port] [options] files

Common Options: 
  -p, --port      #         server port to listen on/connect to
                            (default port: 55555)
  -v, --version             show version and quit
  -h, --help                show usage message and quit
  -d, --debug               print debug messages

Server Options: 
  -s, --server              run in server mode
  -b, --bind      <ip>      bind ip address (default: 0.0.0.0)
      --fork                use multi-process to accept clients
      --thread              use multo-thread to accept clients (default)

Client Options: 
  -c, --client    <host>    run in client mode, connecting to <host>
  -f, --force               force overwriting of files when they exist
```

```
$ dd if=/dev/urandom of=128mb_1 bs=1024 count=131072
$ dd if=/dev/urandom of=128mb_2 bs=1024 count=131072
$ dd if=/dev/urandom of=128mb_3 bs=1024 count=131072
$ dd if=/dev/urandom of=128mb_4 bs=1024 count=131072
$ sha1sum 128mb_*
$ ./mftp -s
```

```
./mftp -c 127.0.0.1 ~/128mb_1 ~/128mb_2 ~/128mb_3 ~/128mb_4
```