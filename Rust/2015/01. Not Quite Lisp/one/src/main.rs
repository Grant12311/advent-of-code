use std::fs;

fn main() {
    let input = fs::read_to_string("input.txt")
        .expect("Failed to load input.txt");

    let mut floor = 0;

    for instruction in input.chars() {
        if instruction == '(' {
            floor += 1;
        } else if instruction == ')' {
            floor -= 1;
        } else {
            println!("Other: {instruction}");
        }
    }

    println!("Result: {floor}");
}
