use md5;

fn five_zeros_in_hex(digest: &md5::Digest) -> bool {
    let bytes = digest.0;

    // 4 bits per hex digit so check the first two full bytes then the upper half of the third byte
    bytes[0] == 0 && bytes[1] == 0 && (bytes[2] & 0b11110000 == 0)
}

fn main() {
    // No puzzle input file this time
    let secret = String::from("abcdef");

    let mut nonce = 1;
    
    while !five_zeros_in_hex(&md5::compute(secret.clone() + &nonce.to_string())) {
        nonce += 1;
    }

    println!("Result: {nonce}");
}
