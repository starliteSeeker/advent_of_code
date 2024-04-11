use std::fs;
use std::num::Wrapping;
use std::str;

pub fn part1() {
    let content = fs::read("./inputs/day15.txt").unwrap();
    let steps = content
        .strip_suffix(&[b'\n'])
        .unwrap()
        .split(|&c| c == b',');

    let mut count: u32 = 0;
    for step in steps {
        count += hash(step) as u32;
    }
    println!("{count}");
}

pub fn part2() {
    let content = fs::read("./inputs/day15.txt").unwrap();
    let steps = content
        .strip_suffix(&[b'\n'])
        .unwrap()
        .split(|&c| c == b',');

    const NEW_VEC: Vec<(&[u8], u8)> = Vec::new();
    let mut boxes = [NEW_VEC; 256];
    for step in steps {
        let type_idx = step.iter().position(|&c| c == b'-' || c == b'=').unwrap();
        let name = &step[..type_idx];
        let box_idx = hash(name) as usize;

        let lens_idx = boxes[box_idx].iter().position(|&l| l.0 == name);

        match step[type_idx] {
            b'=' => {
                let focal: u8 = str::from_utf8(&step[type_idx + 1..])
                    .unwrap()
                    .parse()
                    .unwrap();
                // println!("= {box_idx} {focal}");
                if let Some(i) = lens_idx {
                    boxes[box_idx][i].1 = focal;
                } else {
                    boxes[box_idx].push((name, focal));
                }
            }
            b'-' => {
                // println!("- {box_idx}");
                if let Some(i) = lens_idx {
                    boxes[box_idx].remove(i);
                }
            }
            _ => unreachable!(),
        }
    }

    let mut count = 0;
    for (bi, b) in boxes.iter().enumerate() {
        for (li, l) in b.iter().enumerate() {
            count += (bi + 1) * (li + 1) * l.1 as usize;
        }
    }

    println!("{count}");
}

fn hash(input: &[u8]) -> u8 {
    let mut count: Wrapping<u8> = Wrapping(0);
    for c in input {
        count += c;
        count *= 17;
    }
    count.0
}
