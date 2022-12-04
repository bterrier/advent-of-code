use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

struct Range {
    min: i32,
    max: i32,
}

fn str2range(input: String) -> Range {
    let mut split = input.split('-');
    let range: Range = Range { min: split.nth(0).unwrap().to_string().parse::<i32>().unwrap(), max: split.nth(0).unwrap().to_string().parse::<i32>().unwrap() };
    return range;
}

fn str2ranges(input: String) -> (Range, Range) {
    let mut split = input.split(',');
    return (
        str2range(split.nth(0).unwrap().to_string()),
        str2range(split.nth(0).unwrap().to_string()),
    );
}

fn overlap(a: &Range, b: &Range) -> bool {
    return (a.max <= b.max && a.max >= b.min) || (a.min <= b.max && b.max <= a.max);
}

fn main() {
    // File hosts must exist in current path before this produces output
    let mut score: i32 = 0;
    if let Ok(lines) = read_lines("./input.txt") {
        // Consumes the iterator, returns an (Optional) String
        for line in lines {
            if let Ok(line) = line {
                if line.is_empty() {
                    continue;
                } else {
                    let (r1, r2) = str2ranges(line);
                    if overlap(&r1, &r2) {
                        score += 1;
                    }
                }
            }
        }
    }

    println!("{}", score);
}

// The output is wrapped in a Result to allow matching on errors
// Returns an Iterator to the Reader of the lines of the file.
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}
