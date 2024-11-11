use std::cmp::{max, min};
use std::fs;

pub fn part1() -> Option<()> {
    let mut contents = fs::read("./inputs/day11.txt").unwrap();
    let arr: Vec<_> = contents
        .split_mut(|&c| c == b'\n')
        .filter(|s| !s.is_empty())
        .collect();
    let width = arr[0].len();
    let height = arr.len();
    println!("{width}x{height}");

    let mut empty_rows = Vec::new();
    for i in 0..height {
        if arr[i].iter().all(|&x| x != b'#') {
            empty_rows.push(i);
        }
    }

    let mut empty_cols = Vec::new();
    let mut galaxies = Vec::new();
    for j in 0..width {
        let mut flag = false;
        for i in 0..height {
            if arr[i][j] == b'#' {
                flag = true;
                galaxies.push((i, j));
            }
        }
        if !flag {
            empty_cols.push(j);
        }
    }
    // println!("{:?}", empty_rows);
    // println!("{:?}", empty_cols);
    // println!("{:?}", galaxies);

    let mut dist = 0;
    for (i, &(y1, x1)) in galaxies.iter().enumerate() {
        for j in i + 1..galaxies.len() {
            let (y2, x2) = galaxies[j];
            dist += y2.abs_diff(y1) + x2.abs_diff(x1);
            dist += empty_rows
                .iter()
                .skip_while(|&a| a < min(&y1, &y2))
                .take_while(|&a| a <= max(&y1, &y2))
                .count()
                * 999999;
            dist += empty_cols
                .iter()
                .skip_while(|&a| a < min(&x1, &x2))
                .take_while(|&a| a <= max(&x1, &x2))
                .count()
                * 999999;
        }
    }

    println!("{dist}");

    Some(())
}
