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
