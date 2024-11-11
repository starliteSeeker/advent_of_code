use std::collections::BTreeSet as Set;
use std::fs;
use std::num::Wrapping;
use std::str;

pub fn part1() {
    let mut contents = fs::read("./inputs/day21.txt").unwrap();
    let mut arr: Vec<_> = contents
        .split_mut(|&c| c == b'\n')
        .filter(|l| !l.is_empty())
        .collect();
    let h = arr.len();
    let w = arr[0].len();

    let y = arr
        .iter()
        .position(|l| l.iter().find(|&&c| c == b'S').is_some())
        .unwrap();
    let x = arr[y].iter().position(|&c| c == b'S').unwrap();
    arr[y][x] = b'.';
    println!("{y} {x}");

    let mut queue = Set::new();
    let mut queue2;
    queue.insert((Wrapping(y), Wrapping(x)));
    let one = Wrapping(1);
    for _ in 0..64 {
        queue2 = Set::new();
        for (i, j) in queue {
            if i.0 >= h || j.0 >= w || arr[i.0][j.0] != b'.' {
                continue;
            }
            // arr[i.0][j.0] = b'O';
            queue2.insert((i - one, j));
            queue2.insert((i + one, j));
            queue2.insert((i, j - one));
            queue2.insert((i, j + one));
        }
        queue = queue2;
    }

    let mut count = 0;
    for (i, j) in queue {
        if arr[i.0][j.0] == b'.' {
            count += 1;
            // arr[i.0][j.0] = b'O';
        }
    }
    println!("{count}");
    /*
    for l in arr {
        println!("{}", str::from_utf8(l).unwrap());
    }
    println!("asd");
    */
}

// input is surrounded by space, row and col of S is empty
// track # of pots when entered from center, side, corner,
pub fn part2() {
    // const STEPS: usize = 26501365;
    let contents = fs::read("./inputs/day21.txt").unwrap();
    let arr: Vec<_> = contents
        .split(|&c| c == b'\n')
        .filter(|l| !l.is_empty())
        .collect();
    let h = arr.len();
    let w = arr[0].len();

    let y = arr
        .iter()
        .position(|l| l.iter().find(|&&c| c == b'S').is_some())
        .unwrap();
    let x = arr[y].iter().position(|&c| c == b'S').unwrap();
    println!("{y} {x} {h} {w}"); // 65 65 131 131

    let full_odd = pots(&arr[..], (y, x), 130 + 130 + 1); // odd number steps
    let full_even = pots(&arr[..], (y, x), 130 + 130); // even number steps
    let right_end = pots(&arr[..], (y, 0), 131 - 1);
    let left_end = pots(&arr[..], (y, w - 1), 131 - 1);
    let up_end = pots(&arr[..], (0, x), 131 - 1);
    let down_end = pots(&arr[..], (h - 1, x), 131 - 1);
    let ul_end = pots(&arr[..], (h - 1, w - 1), 65 + 130);
    let ul_end2 = pots(&arr[..], (h - 1, w - 1), 65 - 1);
    let ur_end = pots(&arr[..], (h - 1, 0), 65 + 130);
    let ur_end2 = pots(&arr[..], (h - 1, 0), 65 - 1);
    let dl_end = pots(&arr[..], (0, w - 1), 65 + 130);
    let dl_end2 = pots(&arr[..], (0, w - 1), 65 - 1);
    let dr_end = pots(&arr[..], (0, 0), 65 + 130);
    let dr_end2 = pots(&arr[..], (0, 0), 65 - 1);
    let res = full_odd * ((1 + 202299) * 202299 / 2 + (1 + 202298) * 202298 / 2)
        + full_even * ((1 + 202300) * 202300 / 2 + (1 + 202299) * 202299 / 2)
        + right_end
        + left_end
        + up_end
        + down_end
        + (ul_end + ur_end + dl_end + dr_end) * (202300 - 1)
        + (ul_end2 + ur_end2 + dl_end2 + dr_end2) * 202300;
    println!("{full_odd} {full_even} {right_end} {ul_end} {ul_end2} {res}");

    let mut count = 0;
    for l in arr {
        for c in l {
            if *c == b'#' {
                count += 1;
            }
        }
    }
    println!(
        "{} {} {}",
        count,
        count + full_odd + full_even,
        count + ul_end + dr_end2
    );
}

fn pots(arr: &[&[u8]], start: (usize, usize), round: usize) -> usize {
    let h = arr.len();
    let w = arr[0].len();
    let mut queue = Set::new();
    let mut queue2;
    queue.insert((Wrapping(start.0), Wrapping(start.1)));
    let one = Wrapping(1);
    for _ in 0..round {
        queue2 = Set::new();
        for (i, j) in queue {
            if i.0 >= h || j.0 >= w || arr[i.0][j.0] == b'#' {
                continue;
            }
            queue2.insert((i - one, j));
            queue2.insert((i + one, j));
            queue2.insert((i, j - one));
            queue2.insert((i, j + one));
        }
        queue = queue2;
    }

    let mut count = 0;
    for (i, j) in queue {
        if i.0 < h && j.0 < w && arr[i.0][j.0] != b'#' {
            count += 1;
        }
    }

    count
}
