/// This code is single-threaded and **blocking** - which is, it can only handle
/// one request at a time.
/// - `listener.incoming()` enters a loop that blocks on the accept() syscall,
///   waiting for new connections
/// - when a connection arrives, the program processes it completely before
///   returning to accept the next connection
/// - each connection is handled sequentially - if one client is slow, all other
///   clients must wait
/// Think of it as a process that uses the port 5000 to read information sent to
/// it.
///
/// The accept() syscall is called by the program to return a completed connection from the front of the queue of completed connections. If this queue is empty, the process is put to sleep and waits until a connection arrives at the queue. In this sense, accept() is blocking the execution of the program (assuming that the socket was set as "blocking"). https://stackoverflow.com/a/78253560/16886135
/// You can refer to `tcp_server.c` to see what syscalls are called in order to run
/// a TCP server.
use std::{
    io::{BufRead, BufReader, Write},
    net::{TcpListener, TcpStream},
};

fn main() {
    let listener = TcpListener::bind("127.0.0.1:5000").unwrap();
    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                handle_connection(stream);
            }
            Err(e) => {
                dbg!(e);
            }
        }
    }
}

fn handle_connection(mut stream: TcpStream) {
    let reader = BufReader::new(&stream);
    let http_request: Vec<_> = reader
        .lines()
        .map(|r| r.unwrap())
        // terminates when we find an empty line.
        .take_while(|line| !line.is_empty())
        .collect();
    dbg!(http_request);

    let response = "HTTP/1.1 200 OK\r\n\r\nHello, World!";
    stream.write_all(response.as_bytes()).unwrap();
}
