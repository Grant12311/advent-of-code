use std::fs;
use itertools::Itertools;

// Check if string contains at least the given number of vowels
fn enough_vowls(string: &str, enough: i32) -> bool {
    let vowls = "aeiou";

    let mut count = 0;

    for letter in string.chars() {
        if vowls.contains(letter) {
            count += 1;
            
            if count == enough {
                return true;
            }
        }
    }

    false
}

// Check if string contains the same character twice in a row
fn two_in_a_row(string: &str) -> bool {
    for (x, y) in string.chars().tuple_windows() {
        if x == y {
            return true;
        }
    }

    false
}

// Check if string contains any of a list of "bad" substrings
fn contains_bad_strings(string: &str) -> bool {
    let bad_strings = ["ab", "cd", "pq", "xy"];

    for bad_string in bad_strings {
        if string.contains(bad_string) {
            return true;
        }
    }

    false
}

// Check if string meets all of the requirements to be "nice"
fn is_nice(string: &str) -> bool {
    enough_vowls(string, 3) && two_in_a_row(string) && !contains_bad_strings(string)
}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Failed to open input.txt");

    let mut nice_count = 0;

    for line in input.split("\n") {
        if is_nice(line) {
            nice_count += 1;
        }
    }

    println!("Result: {nice_count}");
}
