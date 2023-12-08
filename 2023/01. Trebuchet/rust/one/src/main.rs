use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("Usage: {}", args[0]);
        return;
    }

    let mut total = 0;

    // for each line in file
    let contents = fs::read_to_string(&args[1]).expect("Could not read input file");
    for line in contents.lines() {
        //  Find first digit
        let first_digit_index = line
            .find(|c: char| c.is_ascii_digit())
            .expect(&format!("No digit found in line \"{line}\""));

        // Convert to numeric
        let first_digit = line
            .chars()
            .nth(first_digit_index)
            .unwrap()
            .to_digit(10)
            .unwrap();

        // Find last digit
        let last_digit_index = line
            .rfind(|c: char| c.is_ascii_digit())
            .expect(&format!("No digit found in line \"{line}\""));
        
        // Convert to numeric
        let last_digit = line
            .chars()
            .nth(last_digit_index)
            .unwrap()
            .to_digit(10)
            .unwrap();

        // Assemble and to total
        total += (first_digit * 10) + last_digit;
    }

    println!("Result {total}");
}
