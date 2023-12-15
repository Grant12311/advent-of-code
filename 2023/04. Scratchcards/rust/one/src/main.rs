use std::process::ExitCode;
use std::{env, fs};

fn main() -> ExitCode {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        eprintln!("Usage: {} <INPUT_FILE>", args[0]);
        return ExitCode::from(1);
    }

    // Puzzle result
    let mut score_sum = 0;

    // Numbers found and winning numbers respectively. Both cleared after each iteration
    let mut present_numbers = Vec::new();
    let mut winning_numbers = Vec::new();

    // Read each line
    let contents = fs::read_to_string(&args[1]).expect("Failed to load input file");
    for (line_num, line) in contents.lines().enumerate() {
        present_numbers.clear();
        winning_numbers.clear();

        let mut tokens = line.split(' ').filter(|token| !token.is_empty());

        // Read and verify "Card"
        if tokens.next() != Some("Card") {
            eprintln!("ERROR! Expected \"Card\" on line {line_num}");
            return ExitCode::from(1);
        }

        // Read and verify card number
        if tokens.next() != Some(&format!("{}:", line_num + 1)) {
            eprintln!("ERROR! Wrong card number on line {line_num}");
            return ExitCode::from(1);
        }

        // Reference to Vec to store numbers in
        let mut target = &mut present_numbers;

        for token in tokens {
            if token == "|" {
                // Switch from reading present numbers to winning numbers
                target = &mut winning_numbers;
            } else {
                // Parse number
                let number: i32 = token.parse().expect(&format!(
                    "Invlaid number on line {}: \"{token}\"",
                    line_num + 1
                ));

                target.push(number);
            }
        }

        // Sort for binary search
        winning_numbers.sort();

        let mut score = 0;

        // Calculate card score
        for present_number in present_numbers.iter() {
            if let Ok(_) = winning_numbers.binary_search(present_number) {
                score = if score == 0 { 1 } else { score * 2 };
            }
        }

        score_sum += score;
    }

    println!("Result: {score_sum}");

    ExitCode::from(0)
}
