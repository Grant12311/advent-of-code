use std::collections::HashMap;
use std::env;
use std::fs;

// Add the part number containg x, y in grid to part_numbers of present
fn process_neighbor(
    grid: &Vec<&str>,
    part_numbers: &mut HashMap<(usize, usize), i32>,
    x: usize,
    y: usize,
) {
    // No need to check below zero because usize is unsigned
    if y >= grid.len() || x >= grid[y].len() {
        return;
    }

    let row = grid[y];
    let c = row.chars().nth(x).expect("Failed to index gird");

    if c.is_ascii_digit() {
        // Find first digit of part number
        let begin = match row[..x].rfind(|c: char| !c.is_ascii_digit()) {
            Some(index) => index + 1,
            None => 0,
        };

        // Find past-the-last digit of part number
        let end = match row[x..].find(|c: char| !c.is_ascii_digit()) {
            Some(index) => x + index,
            None => row.len(),
        };

        // Convert part number to i32 and insert into map
        let part_number: i32 = row[begin..end].parse().expect(&format!(
            "Failed to parse part number \"{}\"",
            &row[begin..end]
        ));

        // Key is coordinate of the start of part number string to allow duplicates of same string in different location
        part_numbers.insert((begin, y), part_number);
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        eprintln!("Usage: {} <INPUT_FILE>", &args[0]);
    }

    let mut part_numbers = HashMap::new();

    let contents = fs::read_to_string(&args[1]).expect("Failed to read input file");
    let grid: Vec<&str> = contents.lines().collect();

    for (y, row) in grid.iter().enumerate() {
        for (x, _c) in row
            .char_indices()
            .filter(|(_x, c)| c.is_ascii_punctuation() && *c != '.') // Only iterate over symbols
        {
            process_neighbor(&grid, &mut part_numbers, x, y.wrapping_sub(1)); // North
            process_neighbor(&grid, &mut part_numbers, x, y.wrapping_add(1)); // South
            process_neighbor(&grid, &mut part_numbers, x.wrapping_add(1), y); // East
            process_neighbor(&grid, &mut part_numbers, x.wrapping_sub(1), y); // West

            // North-East
            process_neighbor(
                &grid,
                &mut part_numbers,
                x.wrapping_add(1),
                y.wrapping_sub(1),
            );
            // South-East
            process_neighbor(
                &grid,
                &mut part_numbers,
                x.wrapping_add(1),
                y.wrapping_add(1),
            );
            // South-West
            process_neighbor(
                &grid,
                &mut part_numbers,
                x.wrapping_sub(1),
                y.wrapping_add(1),
            );
            // North-West
            process_neighbor(
                &grid,
                &mut part_numbers,
                x.wrapping_sub(1),
                y.wrapping_sub(1),
            );
        }
    }

    let part_number_sum: i32 = part_numbers.values().sum();
    println!("Result: {part_number_sum}");
}
