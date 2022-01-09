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
38fadac5b5c8f47043100885b8ef0d51f9480b00  128mb_1
451d455932f4fe19191322ba0c6a87958f1a54b8  128mb_2
caeaebe47ac5eac99d1040ebaec9143eb13d3189  128mb_3
9e60d73325615a67c4a38a9edc3e92a9b8c0423b  128mb_4
$ ./mftp -s
[Server] Use multi-thread to accept clients.
[Server] Server is running at 127.0.0.1:55555
[Thread] Client is connected. (127.0.0.1:37824)
[Thread] Client is connected. (127.0.0.1:37826)
[Thread] Client is connected. (127.0.0.1:37822)
[Thread] Client is connected. (127.0.0.1:37828)
[Thread] Requested file: /home/user/128mb_2
[Thread] Requested file: /home/user/128mb_1
[Thread] Requested file: /home/user/128mb_3
[Thread] Requested file: /home/user/128mb_4
[Thread] File transfer complete!
[Thread] File transfer complete!
[Thread] File transfer complete!
[Thread] File transfer complete!
```

```
$ ./mftp -c 127.0.0.1 ~/128mb_1 ~/128mb_2 ~/128mb_3 ~/128mb_4
[Client] Start downloading 4 file(s).

100% [=================================================>] 128mb_1         (FINISHED)
100% [=================================================>] 128mb_2         (FINISHED)
100% [=================================================>] 128mb_3         (FINISHED)
100% [=================================================>] 128mb_4         (FINISHED)

[Client] Finished!
$ sha1sum 128mb_*
38fadac5b5c8f47043100885b8ef0d51f9480b00  128mb_1
451d455932f4fe19191322ba0c6a87958f1a54b8  128mb_2
caeaebe47ac5eac99d1040ebaec9143eb13d3189  128mb_3
9e60d73325615a67c4a38a9edc3e92a9b8c0423b  128mb_4
```
