use std::env;
use std::fs;

const NUM_COLORS: usize = 3;
const COLOR_STRINGS: [&str; NUM_COLORS] = ["red", "green", "blue"];

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        eprintln!("Usage: {} <INPUT_FILE>", args[0]);
        return;
    }

    // Puzzle result
    let mut power_sum = 0;

    // Read line by line
    let contents = fs::read_to_string(&args[1]).expect("Could not read input file");
    for (line_num, line) in contents.lines().enumerate() {
        // Min number of cubes of each color needed for game to be possible
        let mut min_needed = [0; NUM_COLORS];

        // Sperate line into tokens, skip the "Game" and game id tokens, and remove emtpy spaces
        let mut tokens = line
            .split(|c| " :;,".contains(c))
            .skip(2)
            .filter(|part| !part.is_empty());

        // Iterate over each ammount, color pair
        while let Some(ammount) = tokens.next() {
            // Parse the ammount
            let ammount: i32 = ammount
                .parse()
                .expect(&format!("Could not convert \"{ammount}\" to integer"));

            // Parse the color
            let color = tokens
                .next()
                .expect(&format!("Expected color on line {line_num}"));

            // Get the 0-2 index for the color
            let color = COLOR_STRINGS
                .iter()
                .enumerate()
                .find(|(_, color_string)| **color_string == color)
                .map(|(index, _)| index)
                .expect(&format!("Unknown color \"{color}\" on line {line_num}"));

                // Update min needed of ammount is greater
                min_needed[color] = min_needed[color].max(ammount);
        }

        power_sum += min_needed[0] * min_needed[1] * min_needed[2];
    }

    println!("Result: {power_sum}");
}
