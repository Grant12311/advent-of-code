use std::fs;
use itertools::Itertools;

// Check if string contains any pair of characters "times" seperate times with no overlap
fn recurring_pairs(string: &str, times: i32) -> bool {
    const LETTER_COUNT: usize = 26;

    // LETTER_COUNT x LETTER_COUNT array containing the counts of each pair
    let mut pairs = [[0; LETTER_COUNT]; LETTER_COUNT];

    let mut last = (b'\0', b'\0');

    for (x, y) in string.bytes().tuple_windows() {
        // This prevents "aaa" from counting as two pairs
        if (x, y) == last {
            last = (b'\0', b'\0');
            continue;
        };
        last = (x, y);

        let count = &mut pairs[usize::from(x - b'a')][usize::from(y - b'a')];
        *count += 1;

        if *count == times {
            return true;
        }
    }

    false
}

// Check if string contains the same character twice in a row but with one character in between
fn two_in_a_row_one_between(string: &str) -> bool {
    for (x, _y, z) in string.chars().tuple_windows() {
        if x == z {
            return true;
        }
    }

    false
}

// Check if string meets all of the requirements to be "nice"
fn is_nice(string: &str) -> bool {
    recurring_pairs(string, 2) && two_in_a_row_one_between(string)
}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Failed to open input.txt");

    let mut nice_count = 0;

    for line in input.split("\n") {
        if is_nice(line) {
            nice_count += 1;
        }
    }

    println!("Result: {}", nice_count);
}
