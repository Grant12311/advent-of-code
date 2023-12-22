use std::env;
use std::fs;

#[derive(Debug)]
enum ParseSeedsError {
    EmptyLine,
    WrongStart,
}

// Parses seeds from string in format of "seeds: 1 2 3"
fn parse_seeds(string: &str) -> Result<Vec<i64>, ParseSeedsError> {
    let mut tokens = string.split(" ");

    // Make sure at least one token is present
    if let Some(start) = tokens.next() {
        // Verify start of line
        if start == "seeds:" {
            let mut seeds = Vec::new();

            // Parse all found seeds and insert them into seeds
            while let Some(seed) = tokens.next() {
                let seed = seed
                    .trim()
                    .parse()
                    .expect("Unexpcted non-number in seeds list");
                seeds.push(seed);
            }

            Ok(seeds)
        } else {
            Err(ParseSeedsError::WrongStart)
        }
    } else {
        Err(ParseSeedsError::EmptyLine)
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        panic!("Usage: {} <INPUT_FILE>", args[0]);
    }

    // Read file into lines
    let contents = fs::read_to_string(&args[1]).expect("Failed to read input file");
    let lines: Vec<&str> = contents.lines().collect();

    let mut seeds = parse_seeds(lines[0]).expect("Failed to parse seeds");

    // Flag to check if a seed at a given index has been converted yet by the current map. All set to false on start of every map
    let mut seeds_converted: Vec<bool> = vec![false; seeds.len()];

    // Iterate over each line, skipping seed line, and removing empty lines
    for (line_num, line) in lines
        .iter()
        .enumerate()
        .skip(2)
        .filter(|(_line_num, line)| !line.is_empty())
    {
        // If line starts with digit it's a mapping. Otherwise it's the start of a new map
        if line.chars().nth(0).unwrap().is_ascii_digit() {
            // Parse numbers on line
            let tokens: Vec<i64> = line
                .split(" ")
                .map(|string| {
                    string
                        .trim()
                        .parse::<i64>()
                        .expect("Failed to parse mapping value")
                })
                .collect();

            // There should only be three numbers per mapping
            if tokens.len() != 3 {
                panic!("Invalid number on line {line_num}");
            }

            // Extract three numbers by name
            let dest_start = tokens[0];
            let src_start = tokens[1];
            let size = tokens[2];

            // Try to convert each seed if it hasn't been converted yet this map
            for (seed, converted) in seeds
                .iter_mut()
                .zip(&mut seeds_converted)
                .filter(|(_seed, converted)| !**converted)
            {
                if *seed >= src_start && *seed < src_start + size {
                    *converted = true;
                    *seed = dest_start + *seed - src_start;
                }
            }
        } else {
            // Reset conversion flags for start of new map
            seeds_converted.fill(false);
        }
    }

    println!(
        "Result: {}",
        seeds.iter().min().expect("Must contain at least one seed")
    );
}
