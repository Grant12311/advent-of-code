use std::env;
use std::fs;

const DIGIT_STRINGS: [&str; 9] = [
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
];

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
        // u32::MAX represents not yet found
        let mut first_digit = u32::MAX;
        let mut last_digit = u32::MAX;

        // Look for first numeric digit
        let mut first_digit_index = line
            .find(|c: char| c.is_ascii_digit())
            .unwrap_or(usize::MAX);

        // If numeric digit was found convert from char to numeric
        if first_digit_index != usize::MAX {
            first_digit = line
                .chars()
                .nth(first_digit_index)
                .unwrap()
                .to_digit(10)
                .unwrap();
        }
        
        // Look for digit words
        for (i, digit_string) in DIGIT_STRINGS.iter().enumerate() {
            let found = line.find(digit_string);
            if let Some(found) = found {
                if found < first_digit_index {
                    first_digit = i as u32 + 1;
                    first_digit_index = found;
                }
            }
        }

        // Repeat above steps using line.rfind to find last digit/digit word

        let mut last_digit_index = line
            .rfind(|c: char| c.is_ascii_digit())
            .unwrap_or(usize::MAX);
        if last_digit_index != usize::MAX {
            last_digit = line
                .chars()
                .nth(last_digit_index)
                .unwrap()
                .to_digit(10)
                .unwrap();
        }
        
        for (i, digit_string) in DIGIT_STRINGS.iter().enumerate() {
            let found = line.rfind(digit_string);
            if let Some(found) = found {
                if last_digit_index == usize::MAX || found > last_digit_index {
                    last_digit = i as u32 + 1;
                    last_digit_index = found;
                }
            }
        }

        // Add to total
        total += (first_digit * 10) + last_digit;
    }

    println!("Result {total}");
}
