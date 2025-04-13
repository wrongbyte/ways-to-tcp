/// This code is still single-threaded, but with a critical difference: the I/O operations are not blocking, because we are using epoll now.

fn main() {
    println!("Hello, world!");
}
