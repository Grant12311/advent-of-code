use std::collections::HashSet;
use std::fs;

fn main() {
    let input = fs::read_to_string("input.txt")
        .expect("Failed to open input.txt");

    let mut visited = HashSet::new();

    // One position for each santa
    let mut positions = [(0, 0), (0, 0)];
    // Counts up and is taken mod positions.len() to alternate between santas
    let mut driver: usize = 0;

    visited.insert(positions[0]);

    for dir in input.chars() {
        // Get current santa and swap to next santa for next iteration
        let current_position = &mut positions[driver % positions.len()];
        driver += 1;

        match dir {
            '^' => current_position.1 -= 1,
            '>' => current_position.0 += 1,
            'v' => current_position.1 += 1,
            '<' => current_position.0 -= 1,
            _   => panic!("Unknown direction: {dir}")
        }

        visited.insert(*current_position);
    }

    println!("Result: {}", visited.len());
}
