use std::fs;

// Parse a string in the format of lxwxh (e.g. 5x16x32) into an array of the three numbers
fn parse_dims(line: &str) -> [i32; 3] {
    let mut dims = [0; 3];

    for (i, dim) in line.split("x").enumerate() {
        if i == dims.len() {
            panic!("Invalid line {line}");
        }

        dims[i] = dim.parse().expect(&format!("Invalid line: {line}"));
    }

    dims
}

fn main() {
    let input = fs::read_to_string("input.txt")
        .expect("Could not open input.txt");

    let mut area_needed = 0;

    for line in input.split("\n") {
        if line.is_empty() {
            continue;
        }

        let dims = parse_dims(line);
        let sides = [dims[0] * dims[1], dims[1] * dims[2], dims[2] * dims[0]];
        let area = sides[0] * 2 + sides[1] * 2 + sides[2] * 2;

        area_needed += area;
        area_needed += sides.iter().min().expect("parse_dims returned an emtpy array");
    }

    println!("Result: {area_needed}");
}
