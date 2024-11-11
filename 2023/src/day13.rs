use std::fs;

pub fn part1() {
    let contents = fs::read("./inputs/day13.txt").unwrap();
    let lines = contents.split(|&c| c == b'\n').collect::<Vec<_>>();
    let fields = lines
        .split(|&v| v.is_empty())
        .filter(|&a| !a.is_empty())
        .collect::<Vec<_>>();

    let mut count = 0;
    'next: for arr in fields {
        let w = arr[0].len();
        let h = arr.len();
        // println!("{w}x{h}");

        // check rows
        'outer: for i in 0..h - 1 {
            for j in 0..=i.min(h - i - 2) {
                if arr[i - j] != arr[i + 1 + j] {
                    continue 'outer;
                }
            }

            // found mirror
            // println!("row {i}");
            count += (i + 1) * 100; //todo!();
            continue 'next;
        }

        // check cols
        'outer: for i in 0..w - 1 {
            for j in 0..=i.min(w - i - 2) {
                let col1 = i - j;
                let col2 = i + 1 + j;
                for k in 0..h {
                    if arr[k][col1] != arr[k][col2] {
                        continue 'outer;
                    }
                }
            }
            // println!("col {i}");
            count += i + 1;
            continue 'next;
        }

        panic!("no mirror");
    }

    println!("{count}");
}

pub fn part2() {
    let contents = fs::read("./inputs/day13.txt").unwrap();
    let lines = contents.split(|&c| c == b'\n').collect::<Vec<_>>();
    let fields = lines
        .split(|&v| v.is_empty())
        .filter(|&a| !a.is_empty())
        .collect::<Vec<_>>();

    let mut count = 0;
    'next: for arr in fields {
        let w = arr[0].len();
        let h = arr.len();

        // check rows
        'outer: for i in 0..h - 1 {
            let mut error = 0;
            for j in 0..=i.min(h - i - 2) {
                let row1 = i - j;
                let row2 = i + 1 + j;
                for k in 0..w {
                    if arr[row1][k] != arr[row2][k] {
                        error += 1;
                        if error <= 1 {
                            continue;
                        }
                        continue 'outer;
                    }
                }
            }

            // found mirror
            if error != 1 {
                continue;
            }
            println!("row {i}");
            count += (i + 1) * 100; //todo!();
            continue 'next;
        }

        // check cols
        'outer: for i in 0..w - 1 {
            let mut error = 0;
            for j in 0..=i.min(w - i - 2) {
                let col1 = i - j;
                let col2 = i + 1 + j;
                for k in 0..h {
                    if arr[k][col1] != arr[k][col2] {
                        error += 1;
                        if error <= 1 {
                            continue;
                        }
                        continue 'outer;
                    }
                }
            }
            if error != 1 {
                continue;
            }
            println!("col {i}");
            count += i + 1;
            continue 'next;
        }

        panic!("no mirror");
    }

    println!("{count}");
}
