use itertools::Itertools;
use std::collections::VecDeque as Q;
use std::fs;
use std::iter::once;
use std::str;

pub fn part1() {
    let contents = fs::read("./inputs/day18.txt").unwrap();
    let arr = contents
        .split(|&c| c == b'\n')
        .filter(|&l| !l.is_empty())
        .map(|l| {
            l.split(|&c| c == b' ')
                .collect_tuple::<(_, _, _)>()
                .unwrap()
        })
        .collect_vec();

    // find bound
    let mut left = 0;
    let mut right = 0;
    let mut up = 0;
    let mut down = 0;
    let mut curr_x = 0;
    let mut curr_y = 0;
    for line in &arr[..] {
        let count: i32 = str::from_utf8(line.1).unwrap().parse().unwrap();
        match line.0 {
            b"L" => {
                curr_x -= count;
                if curr_x < left {
                    left = curr_x;
                }
            }
            b"R" => {
                curr_x += count;
                if curr_x > right {
                    right = curr_x;
                }
            }
            b"U" => {
                curr_y -= count;
                if curr_y < up {
                    up = curr_y;
                }
            }
            b"D" => {
                curr_y += count;
                if curr_y > down {
                    down = curr_y;
                }
            }
            _ => unreachable!(),
        }
    }

    println!("{left} {right} {up} {down}");

    // draw field
    let w = (right - left + 1 + 2).try_into().unwrap();
    let h = (down - up + 1 + 2).try_into().unwrap();
    let mut field = vec![vec![b'.'; w]; h];
    curr_x = -left + 1;
    curr_y = -up + 1;
    for line in arr {
        let count: i32 = str::from_utf8(line.1).unwrap().parse().unwrap();
        match line.0 {
            b"L" => {
                for _ in 0..count {
                    curr_x -= 1;
                    field[curr_y as usize][curr_x as usize] = b'l';
                }
            }
            b"R" => {
                for _ in 0..count {
                    curr_x += 1;
                    field[curr_y as usize][curr_x as usize] = b'r';
                }
            }
            b"U" => {
                for _ in 0..count {
                    curr_y -= 1;
                    field[curr_y as usize][curr_x as usize] = b'u';
                }
            }
            b"D" => {
                for _ in 0..count {
                    curr_y += 1;
                    field[curr_y as usize][curr_x as usize] = b'd';
                }
            }
            _ => unreachable!(),
        }
    }

    // for l in &field {
    //     println!("{}", str::from_utf8(&l).unwrap());
    // }

    let mut outer = 0;
    let mut queue = Q::new();
    queue.push_back((0, 0));

    while let Some((x, y)) = queue.pop_front() {
        if x >= w || y >= h {
            continue;
        }
        if field[y][x] == b'.' {
            outer += 1;
            field[y][x] = b'#';
            queue.push_back((x.wrapping_sub(1), y));
            queue.push_back((x.wrapping_add(1), y));
            queue.push_back((x, y.wrapping_sub(1)));
            queue.push_back((x, y.wrapping_add(1)));
        }
    }
    println!("{}", w * h - outer);
}

#[derive(Debug, Hash, Eq, PartialEq, Copy, Clone)]
enum Dir {
    Right,
    Up,
    Left,
    Down,
}

pub fn part2() {
    let contents = fs::read("./inputs/day18.txt").unwrap();
    let arr = contents
        .split(|&c| c == b'\n')
        .filter(|&l| !l.is_empty())
        .map(|l| {
            l.split(|&c| c == b' ')
                .collect_tuple::<(_, _, _)>()
                .unwrap()
        })
        .collect_vec();

    // get direction from color code
    let mut path = Vec::new();
    for (_, _, color) in arr {
        let count: isize =
            isize::from_str_radix(str::from_utf8(&color[2..=6]).unwrap(), 16).unwrap();
        match color[7] {
            b'3' => {
                path.push((Dir::Up, count));
            }
            b'1' => {
                path.push((Dir::Down, count));
            }
            b'2' => {
                path.push((Dir::Left, count));
            }
            b'0' => {
                path.push((Dir::Right, count));
            }
            _ => unreachable!(),
        }
    }

    let mut border1 = Vec::new(); // counterclockwise
    let mut border2 = Vec::new(); // clockwise
    let mut curr_x: isize = 0;
    let mut curr_y: isize = 0;
    path.push(path[0]); // loop back to start
    for ((dir_a, count_a), (dir_b, _)) in path.iter().tuple_windows() {
        match dir_a {
            Dir::Left => {
                curr_x -= count_a;
            }
            Dir::Right => {
                curr_x += count_a;
            }
            Dir::Up => {
                curr_y -= count_a;
            }
            Dir::Down => {
                curr_y += count_a;
            }
        }
        match (dir_a, dir_b) {
            (Dir::Up, Dir::Right) | (Dir::Right, Dir::Up) => {
                border1.push((curr_x + 1, curr_y + 1));
                border2.push((curr_x, curr_y));
            }
            (Dir::Right, Dir::Down) | (Dir::Down, Dir::Right) => {
                border1.push((curr_x, curr_y + 1));
                border2.push((curr_x + 1, curr_y));
            }
            (Dir::Down, Dir::Left) | (Dir::Left, Dir::Down) => {
                border1.push((curr_x, curr_y));
                border2.push((curr_x + 1, curr_y + 1));
            }
            (Dir::Left, Dir::Up) | (Dir::Up, Dir::Left) => {
                border1.push((curr_x + 1, curr_y));
                border2.push((curr_x, curr_y + 1));
            }
            _ => panic!("why tho"),
        }
    }

    // calculate area
    // https://en.wikipedia.org/wiki/Shoelace_formula
    border1.push(border1[0]); // loop back to start
    let mut area1 = 0;
    for ((x1, y1), (x2, y2)) in border1.iter().tuple_windows() {
        area1 += x1 * y2 - y1 * x2;
    }
    area1 /= 2;
    border2.push(border2[0]); // loop back to start
    let mut area2 = 0;
    for ((x1, y1), (x2, y2)) in border2.iter().tuple_windows() {
        area2 += x1 * y2 - y1 * x2;
    }
    area2 /= 2;
    println!("{area1} {area2}");
}
