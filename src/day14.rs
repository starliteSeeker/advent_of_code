use itertools::Itertools;
use std::{collections::BTreeMap, fs};

pub fn part1() {
    let contents = fs::read("inputs/day14.txt").unwrap();
    let arr: Vec<_> = contents
        .split(|&c| c == b'\n')
        .filter(|&s| !s.is_empty())
        .collect();

    let w = arr[0].len();
    let h = arr.len();
    let mut count = 0;

    for i in 0..w {
        let mut here = 0; // stone will fall to here
        for j in 0..h {
            match arr[j][i] {
                b'.' => (),
                b'#' => here = j + 1,
                b'O' => {
                    count += h - here;
                    here += 1;
                }
                _ => unreachable!(),
            }
        }
    }
    println!("{w} {h} {count}");
}

pub fn part2() {
    let contents = fs::read("inputs/day14.txt").unwrap();
    let arr: Vec<_> = contents
        .split(|&c| c == b'\n')
        .filter(|&s| !s.is_empty())
        .collect();

    let w: i32 = arr[0].len().try_into().unwrap();
    let h: i32 = arr.len().try_into().unwrap();

    let mut square: Vec<(i32, i32)> = Vec::new();
    let mut circle: Vec<(i32, i32)> = Vec::new();
    for (i, row) in arr.iter().enumerate() {
        for (j, c) in row.iter().enumerate() {
            match c {
                b'#' => square.push((i.try_into().unwrap(), j.try_into().unwrap())),
                b'O' => circle.push((i.try_into().unwrap(), j.try_into().unwrap())),
                _ => (),
            }
        }
    }
    println!("#: {}, O:{}", square.len(), circle.len());

    square.sort_by(|(ai, aj), (bi, bj)| Ord::cmp(&(aj, ai), &(bj, bi)));
    let square_up = (&mut square[..])
        .chunk_by_mut(|a, b| a.1 == b.1)
        .map(|arr| (arr[0].1, arr.iter().map(|a| a.0).collect_vec()))
        .collect::<BTreeMap<_, _>>();
    square.sort();
    let square_left = (&mut square[..])
        .chunk_by_mut(|a, b| a.0 == b.0)
        .map(|arr| (arr[0].0, arr.iter().map(|a| a.1).collect_vec()))
        .collect::<BTreeMap<_, _>>();
    square.sort_by(|(ai, aj), (bi, bj)| Ord::cmp(&(aj, ai), &(bj, bi)).reverse());
    let square_down = (&mut square[..])
        .chunk_by_mut(|a, b| a.1 == b.1)
        .map(|arr| (arr[0].1, arr.iter().map(|a| a.0).collect_vec()))
        .collect::<BTreeMap<_, _>>();
    square.sort_by(|a, b| Ord::cmp(&a, &b).reverse());
    let square_right = (&mut square[..])
        .chunk_by_mut(|a, b| a.0 == b.0)
        .map(|arr| (arr[0].0, arr.iter().map(|a| a.1).collect_vec()))
        .collect::<BTreeMap<_, _>>();

    let mut loads = Vec::new();
    let mut repeat = -1;

    for loop_count in 0.. {
        // tilt up
        circle.sort_by(|(ai, aj), (bi, bj)| Ord::cmp(&(aj, ai), &(bj, bi)));
        for group in circle.chunk_by_mut(|a, b| a.1 == b.1) {
            // current column
            let i = group[0].1;
            // println!("{} {:?} {:?}", i, group, square_up.get(&i));

            let mut bottom = 0; // where rock will fall to
            let square_vec = square_up.get(&i);
            let mut next_square = match square_vec {
                Some(v) => v.iter().peekable(),
                None => [].iter().peekable(),
            };
            for x in &mut *group {
                // set bottom to correct value
                while let Some(&&n) = next_square.peek() {
                    if n < x.0 {
                        bottom = n + 1;
                        next_square.next();
                    } else {
                        break;
                    }
                }

                // drop stone
                x.0 = bottom;
                bottom += 1;
            }
        }

        // tilt left
        circle.sort();
        for group in circle.chunk_by_mut(|a, b| a.0 == b.0) {
            // current row
            let i = group[0].0;
            // println!("{} {:?} {:?}", i, group, square_left.get(&i));

            let mut bottom = 0; // where rock will fall to
            let square_vec = square_left.get(&i);
            let mut next_square = match square_vec {
                Some(v) => v.iter().peekable(),
                None => [].iter().peekable(),
            };
            for x in &mut *group {
                // set bottom to correct value
                while let Some(&&n) = next_square.peek() {
                    if n < x.1 {
                        bottom = n + 1;
                        next_square.next();
                    } else {
                        break;
                    }
                }

                // drop stone
                x.1 = bottom;
                bottom += 1;
            }
        }

        // tilt down
        circle.sort_by(|(ai, aj), (bi, bj)| Ord::cmp(&(aj, ai), &(bj, bi)).reverse());
        for group in circle.chunk_by_mut(|a, b| a.1 == b.1) {
            // current column
            let i = group[0].1;
            // println!("{} {:?} {:?}", i, group, square_down.get(&i));

            let mut bottom = h - 1; // where rock will fall to
            let square_vec = square_down.get(&i);
            let mut next_square = match square_vec {
                Some(v) => v.iter().peekable(),
                None => [].iter().peekable(),
            };
            for x in &mut *group {
                // set bottom to correct value
                while let Some(&&n) = next_square.peek() {
                    if n > x.0 {
                        bottom = n - 1;
                        next_square.next();
                    } else {
                        break;
                    }
                }

                // drop stone
                x.0 = bottom;
                bottom -= 1;
            }
        }

        // tilt right
        circle.sort_by(|a, b| Ord::cmp(&a, &b).reverse());
        for group in circle.chunk_by_mut(|a, b| a.0 == b.0) {
            // current row
            let i = group[0].0;
            // println!("{} {:?} {:?}", i, group, square_right.get(&i));

            let mut bottom = w - 1; // where rock will fall to
            let square_vec = square_right.get(&i);
            let mut next_square = match square_vec {
                Some(v) => v.iter().peekable(),
                None => [].iter().peekable(),
            };
            for x in &mut *group {
                // set bottom to correct value
                while let Some(&&n) = next_square.peek() {
                    if n > x.1 {
                        bottom = n - 1;
                        next_square.next();
                    } else {
                        break;
                    }
                }

                // drop stone
                x.1 = bottom;
                bottom -= 1;
            }
        }

        // calc load
        let mut count = 0;
        for (i, _) in &circle {
            count += h - i;
        }
        loads.push((count, circle.clone()));
        // println!("{}, {}, {:?}", loop_count, count, circle);

        // cycle detection
        if loop_count % 2 == 0 && loads[loop_count] == loads[loop_count / 2] && loop_count != 0 {
            repeat = loads[loop_count].0;
            break;
        }
    }

    let first_repeat = loads.iter().position(|x| x.0 == repeat).unwrap();
    let period = loads
        .iter()
        .skip(first_repeat + 1)
        .position(|x| x.0 == repeat)
        .unwrap()
        + 1;
    println!("break, {repeat}, {first_repeat}, {period}");
    println!(
        "{}",
        loads[(1000000000 - first_repeat) % period - 1 + first_repeat].0
    )
}
