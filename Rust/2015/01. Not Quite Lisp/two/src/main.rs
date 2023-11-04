use std::fs;

fn main() {
    let input = fs::read_to_string("input.txt")
        .expect("Failed to load input.txt");

    let position = 'position_loop: loop {
        let mut floor = 0;
        let mut position = 1;

        for instruction in input.chars() {
            if instruction == '(' {
                floor += 1;
            } else if instruction == ')' {
                floor -= 1;
            }

            if floor == -1 {
                break 'position_loop position;
            }
    
            position += 1;
        }
    };

    println!("Result: {position}");
}
