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
    let contents = fs::read("inputs/day14_2.txt").unwrap();
    let arr: Vec<_> = contents
        .split(|&c| c == b'\n')
        .filter(|&s| !s.is_empty())
        .collect();

    // let w = arr[0].len();
    // let h = arr.len();

    let mut square: Vec<(u32, u32)> = Vec::new();
    let mut circle: Vec<(u32, u32)> = Vec::new();
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

    // let square_up_group = &square
    //     .iter()
    //     .sorted_by(|(ai, aj), (bi, bj)| Ord::cmp(&(aj, ai), &(bj, bi)))
    //     .group_by(|(_, b)| b);
    // let square_up = (square_up_group)
    //     .into_iter()
    //     .map(|(key, l)| (key, l.map(|(a, _)| a).collect_vec()))
    //     .collect::<BTreeMap<_, _>>();
    square.sort_by(|(ai, aj), (bi, bj)| Ord::cmp(&(aj, ai), &(bj, bi)));
    let square_up = (&mut square[..])
        .chunk_by_mut(|a, b| a.1 == b.1)
        .map(|arr| (arr[0].1, arr.iter().map(|a| a.0).collect_vec()))
        .collect::<BTreeMap<_, _>>();

    // tilt up
    circle.sort_by(|(ai, aj), (bi, bj)| Ord::cmp(&(aj, ai), &(bj, bi)));
    for group in circle.chunk_by_mut(|a, b| a.1 == b.1) {
        // current column
        let i = group[0].1;
        println!("{} {:?} {:?}", i, group, square_up.get(&i));
        for x in &mut *group {
            x.1 = 100;
        }
        println!("{} {:?} {:?}", i, group, square_up.get(&i));
    }
}
