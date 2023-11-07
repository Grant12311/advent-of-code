use std::collections::HashSet;
use std::fs;

fn main() {
    let input = fs::read_to_string("input.txt")
        .expect("Failed to open input.txt");

    let mut visited = HashSet::new();
    let mut current_position = (0, 0);

    visited.insert(current_position);

    for dir in input.chars() {
        match dir {
            '^' => current_position.1 -= 1,
            '>' => current_position.0 += 1,
            'v' => current_position.1 += 1,
            '<' => current_position.0 -= 1,
            _   => panic!("Unknown direction: {dir}")
        }

        visited.insert(current_position);
    }

    println!("Result: {}", visited.len());
}
