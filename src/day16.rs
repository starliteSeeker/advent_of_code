use std::collections::{HashSet, VecDeque};
use std::fs;

#[derive(Debug, Hash, Eq, PartialEq, Copy, Clone)]
enum Dir {
    Right,
    Up,
    Left,
    Down,
}

pub fn part1() {
    let contents = fs::read("./inputs/day16.txt").unwrap();
    let arr = contents
        .split(|&c| c == b'\n')
        .filter(|&a| !a.is_empty())
        .collect::<Vec<_>>();

    println!("{}", part1_logic(&arr[..], ((0, 0), Dir::Left)));
}

fn part1_logic(arr: &[&[u8]], start: ((usize, usize), Dir)) -> usize {
    let mut queue = VecDeque::new();
    queue.push_back(start);

    let mut visited = HashSet::new();

    while let Some(item @ ((i, j), d)) = queue.pop_front() {
        if i >= arr.len() || j >= arr[0].len() {
            continue;
        }

        if !visited.insert(item) {
            continue;
        }

        // beam came in from direction d
        // println!("{i} {j} {d:?}");

        match (arr[i][j], d) {
            (b'|', Dir::Left | Dir::Right) => {
                // send up and down
                queue.push_back(((i.wrapping_sub(1), j), Dir::Down));
                queue.push_back(((i + 1, j), Dir::Up));
            }
            (b'-', Dir::Up | Dir::Down) => {
                // send left and right
                queue.push_back(((i, j.wrapping_sub(1)), Dir::Right));
                queue.push_back(((i, j + 1), Dir::Left));
            }
            (b'\\', Dir::Up) | (b'/', Dir::Down) => {
                // send right
                queue.push_back(((i, j + 1), Dir::Left));
            }
            (b'\\', Dir::Right) | (b'/', Dir::Left) => {
                // send up
                queue.push_back(((i.wrapping_sub(1), j), Dir::Down));
            }
            (b'\\', Dir::Down) | (b'/', Dir::Up) => {
                // send left
                queue.push_back(((i, j.wrapping_sub(1)), Dir::Right));
            }
            (b'\\', Dir::Left) | (b'/', Dir::Right) => {
                // send down
                queue.push_back(((i + 1, j), Dir::Up));
            }
            (_, Dir::Left) => {
                // send right
                queue.push_back(((i, j + 1), Dir::Left));
            }
            (_, Dir::Down) => {
                // send up
                queue.push_back(((i.wrapping_sub(1), j), Dir::Down));
            }
            (_, Dir::Right) => {
                // send left
                queue.push_back(((i, j.wrapping_sub(1)), Dir::Right));
            }
            (_, Dir::Up) => {
                // send down
                queue.push_back(((i + 1, j), Dir::Up));
            }
        }
    }

    visited.iter().map(|&x| x.0).collect::<HashSet<_>>().len()
}

pub fn part2() {
    let contents = fs::read("./inputs/day16.txt").unwrap();
    let arr = contents
        .split(|&c| c == b'\n')
        .filter(|&a| !a.is_empty())
        .collect::<Vec<_>>();

    let w = arr[0].len();
    let h = arr.len();
    let from_left = (0..h).map(|i| ((i, 0), Dir::Left));
    // println!("{:?}", from_left.collect::<Vec<_>>());
    let from_down = (0..w).map(|i| ((h - 1, i), Dir::Down));
    let from_right = (0..h).map(|i| ((i, w - 1), Dir::Right));
    let from_up = (0..w).map(|i| ((0, i), Dir::Up));

    let res = from_left
        .chain(from_down)
        .chain(from_right)
        .chain(from_up)
        .map(|x| part1_logic(&arr[..], x))
        .max();

    println!("{res:?}");
}
