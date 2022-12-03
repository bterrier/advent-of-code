use std::collections::HashSet;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn char2score(c: char) -> i32 {
    let cvalue: i32 = c as i32;

    if c >= 'A' && c <= 'Z' {
        return cvalue - 'A' as i32 + 27;
    }

    return cvalue - 'a' as i32 + 1;
}

fn main() {
    // File hosts must exist in current path before this produces output
    let mut score: i32 = 0;
    let mut sets: [HashSet<char>; 2] = [HashSet::new(), HashSet::new()];
    let mut index: usize = 0;
    if let Ok(lines) = read_lines("./input.txt") {
        // Consumes the iterator, returns an (Optional) String
        for line in lines {
            if let Ok(line) = line {
                if line.is_empty() {
                    continue;
                } else {
                    if index == 2 {
                        for c in line.chars() {
                            if sets[0].contains(&c) && sets[1].contains(&c) {
                                
                                score += char2score(c);
                                break;
                            }
                        }
                        sets = [HashSet::new(), HashSet::new()];
                        index = 0;
                    } else {
                        for c in line.chars() {
                            sets[index].insert(c);
                        }
                        index += 1;
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
