use md5;

fn six_zeros_in_hex(digest: &md5::Digest) -> bool {
    let bytes = digest.0;

    // 4 bits per hex digit
    bytes[0] == 0 && bytes[1] == 0 && bytes[2] == 0
}

fn main() {
    // No puzzle input file this time
    let secret = String::from("yzbqklnj");

    let mut nonce = 1;
    
    while !six_zeros_in_hex(&md5::compute(secret.clone() + &nonce.to_string())) {
        nonce += 1;
    }

    println!("Result: {nonce}");
}
