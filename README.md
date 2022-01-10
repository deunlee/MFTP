# MFTP :: Multiple File Transfer Program

The MFTP is a simple file transfer program for study.

MFTP는 공부를 위한 간단한 파일 전송 프로그램입니다.

The server sends multiple files to clients at the same time.
For comparison of multi-threading and multi-processing performance, the server supports two modes.
The client is multi-threaded and downloads multiple files at the same time.
During the download, progress and speed are displayed in progress bar.

서버는 클라이언트에게 동시에 여러 파일을 전송합니다.
멀티스레딩과 멀티프로세싱 성능 비교를 위해 서버는 두 가지 모드를 지원합니다.
클라이언트는 멀티스레드로 동작하며 서버로부터 다수의 파일을 동시에 다운로드합니다.
다운로드 중에는 진행률과 속도가 프로그래스바로 표시됩니다.

WARNING: This program has no security features, including authentication.

경고: 이 프로그램은 인증 기능을 비롯한 보안 기능이 없습니다.


## Build
```
make
```

## Usage
```
$ ./mftp -h
  __  __ ______ _______ _____  
 |  \/  |  ____|__   __|  __ \ 
 | \  / | |__     | |  | |__) |
 | |\/| |  __|    | |  |  ___/ 
 | |  | | |       | |  | |     
 |_|  |_|_|       |_|  |_|     

MFTP: Multiple File Transfer Program by Deun Lee

Usage: mftp -s    [-p port] [options]
       mftp -c ip [-p port] [options] files

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
      --thread              use multi-thread to accept clients (default)

Client Options: 
  -c, --client    <ip>      run in client mode, connecting to <ip>
  -f, --force               force overwriting of files when they exist
```

## Server Example
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

## Client Example
```
$ ./mftp -c 127.0.0.1 ~/128mb_1 ~/128mb_2 ~/128mb_3 ~/128mb_4
[Client] Start downloading 4 file(s).

 78% [======================================>           ] 128mb_1         (152.8 MB/s)
 56% [===========================>                      ] 128mb_2         (44.4 MB/s)
100% [=================================================>] 128mb_3         (FINISHED)
 98% [================================================> ] 128mb_4         (226.0 MB/s)

...

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

## License
[MIT License](./LICENSE)
