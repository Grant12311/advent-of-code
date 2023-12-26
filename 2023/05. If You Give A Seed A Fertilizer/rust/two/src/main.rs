use std::env;
use std::fs;

#[derive(Debug)]
enum ParseSeedsError {
    EmptyLine,
    OddNumberCount,
    WrongStart,
}

struct SeedRange {
    start: i64,
    size: i64,
}

impl SeedRange {
    fn from(start: i64, size: i64) -> SeedRange {
        SeedRange { start, size }
    }
}

// Parses seed ranges from string in format of "seeds: <RANGE_START> <RANGE_END> <RANGE_START> <RANGE_END>"
fn parse_seeds(string: &str) -> Result<Vec<SeedRange>, ParseSeedsError> {
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

            // Make sure all seeds have both start and size
            if seeds.len() % 2 == 1 {
                Err(ParseSeedsError::OddNumberCount)
            } else {
                Ok(seeds
                    .chunks(2)
                    .map(|chunk| SeedRange::from(chunk[0], chunk[1]))
                    .collect())
            }
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

    // New breakaway ranges to insert into seeds after each seed is converted. Cleared after insertion
    let mut new_seeds = Vec::new();

    // Flag to check if a range at a given index has been converted yet by the current map. All set to false on start of every map
    let mut range_converted: Vec<bool> = vec![false; seeds.len()];

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

            // Iterate by index because we need to add ranges mid iteration
            for i in 0..seeds.len() {
                let converted = &mut range_converted[i];

                // Skip ranges already converted during current map
                if *converted {
                    continue;
                }

                let range = &mut seeds[i];

                // Skip ranges that don't overlap with the conversion area
                if range.start + range.size - 1 < src_start || range.start >= src_start + size {
                    continue;
                }

                // Break off portion before start of conversion area
                if range.start < src_start {
                    new_seeds.push(SeedRange::from(range.start, src_start - range.start));
                    range.size -= src_start - range.start;
                    range.start = src_start;
                }

                // Break off portion before after end of conversion area
                if range.start + range.size > src_start + size {
                    new_seeds.push(SeedRange::from(
                        src_start + size,
                        range.start + range.size - (src_start + size),
                    ));
                    range.size -= range.start + range.size - (src_start + size);
                }

                // Convert remaining portion
                range.start = dest_start + range.start - src_start;
                *converted = true;

                // Insert breakaway seeds
                seeds.append(&mut new_seeds);

                // Resize to match current seed count
                range_converted.resize(seeds.len(), false);
            }
        } else {
            // Reset conversion flags for start of new map
            range_converted.fill(false);
        }
    }

    println!(
        "Result: {}",
        seeds
            .iter()
            .map(|range| range.start)
            .min()
            .expect("No seed ranges found")
    );
}
