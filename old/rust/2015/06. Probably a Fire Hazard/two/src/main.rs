use std::fs;

const GRID_SIDE_LEN: usize = 1_000;

struct Point {
    x: usize,
    y: usize,
}

impl Point {
    // Parse Point from a string in the format of x,y
    fn parse(string: &str) -> Point {
        let string = string.trim();

        // Comma marks the end of x
        let comma_pos = string
            .find(',')
            .expect(&format!("Invalid line \"{string}\", expected a comma"));

        // y ends with either a space or the end of the string
        let space_pos = match string.find(' ') {
            Some(pos) => pos,
            None => string.len(),
        };

        let x = string[..comma_pos]
            .parse()
            .expect(&format!("Invalid line \"{string}\", x is not a number"));
        let y = string[comma_pos + 1..space_pos]
            .parse()
            .expect(&format!("Invalid line \"{string}\", y is not a number"));

        Point { x, y }
    }
}

enum InstructionKind {
    On,
    Off,
    Toggle,
}

impl InstructionKind {
    // Return length of string representation of InstructionKind
    fn to_string_len(&self) -> usize {
        match self {
            Self::On => Instruction::ON_STR.len(),
            Self::Off => Instruction::OFF_STR.len(),
            Self::Toggle => Instruction::TOGGLE_STR.len(),
        }
    }
}

struct Instruction {
    kind: InstructionKind,
    from: Point,
    to: Point,
}

// Take characters from the start until first numeric digit. Returns None of no digit is found
fn until_first_digit(string: &str) -> Option<&str> {
    for (i, character) in string.char_indices() {
        if character.is_numeric() {
            return Some(&string[..i]);
        }
    }

    None
}

impl Instruction {
    const ON_STR: &str = "turn on";
    const OFF_STR: &str = "turn off";
    const TOGGLE_STR: &str = "toggle";

    // Parse Instruction from string
    fn parse(string: &str) -> Instruction {
        // Parse kind
        let kind = match until_first_digit(string)
            .expect(&format!("Invalid line \"{string}\""))
            .trim_end()
        {
            Self::ON_STR => InstructionKind::On,
            Self::OFF_STR => InstructionKind::Off,
            Self::TOGGLE_STR => InstructionKind::Toggle,
            &_ => panic!("Unknown instruction kind string \"{string}\""),
        };

        let from = Point::parse(&string[kind.to_string_len()..]);

        // Find "through" to find start of second point
        let through_pos = string
            .find("through")
            .expect(&format!("Invalid line \"{string}\", expected \"through\""));

        let to = Point::parse(&string[through_pos + "through".len()..]);

        Instruction { kind, from, to }
    }

    // Perform action on grid of lights
    fn process(&self, grid: &mut [[i32; GRID_SIDE_LEN]; GRID_SIDE_LEN]) {
        {
            match self.kind {
                // Increase brightness in range
                InstructionKind::On => {
                    for row in &mut grid[self.from.y..self.to.y + 1] {
                        for cell in &mut row[self.from.x..self.to.x + 1] {
                            *cell += 1;
                        }
                    }
                }
                // Decrease brightness in range, minimum zero
                InstructionKind::Off => {
                    for row in &mut grid[self.from.y..self.to.y + 1] {
                        for cell in &mut row[self.from.x..self.to.x + 1] {
                            if *cell > 0 {
                                *cell -= 1;
                            }
                        }
                    }
                }
                // Increase brightness in range by two (toggle makes no sense but is from instructions)
                InstructionKind::Toggle => {
                    for row in &mut grid[self.from.y..self.to.y + 1] {
                        for cell in &mut row[self.from.x..self.to.x + 1] {
                            *cell += 2;
                        }
                    }
                }
            }
        }
    }
}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Could not open input.txt");

    // GRID_SIDE_LEN x GRID_SIDE_LEN 2D array of lights brightness values
    let mut grid = [[0; GRID_SIDE_LEN]; GRID_SIDE_LEN];

    // Read each line of input
    for line in input.split("\n") {
        if line.is_empty() {
            continue;
        }

        // Parse an process instruction
        let int = Instruction::parse(line);
        int.process(&mut grid);
    }

    let mut sum = 0;

    // Sum light brightnesses
    for row in &grid {
        for cell in row {
            sum += cell;
        }
    }

    println!("Result: {sum}");
}
