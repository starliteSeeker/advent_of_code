use std::fs;
use std::str;

pub fn part1() -> Option<()> {
    let mut contents = fs::read("./inputs/day10.txt").unwrap();
    let mut arr: Vec<_> = contents.split_mut(|&c| c == b'\n').collect();

    let width = arr[0].len();
    let height = arr.len();

    let mut sx = 0;
    let mut sy = 0;
    'find_s: for i in 0..arr.len() {
        for j in 0..arr[0].len() {
            if arr[i][j] == b'S' {
                sx = j;
                sy = i;
                break 'find_s;
            }
        }
    }

    println!("S at {sx},{sy}");

    // first step is left
    let sx = sx;
    let sy = sy;
    let mut x = sx - 1;
    let mut y = sy;
    let mut count = 1;
    let to_left = b"-J7";
    let to_right = b"-LF";
    let to_up = b"|JL";
    let to_down = b"|7F";
    // arr[sy][sx] = b'7';
    arr[sy][sx] = b'-';
    // keep track of top left corner
    let mut tx = sx;
    let mut ty = sy;
    while x != sx || y != sy {
        // go left
        if x > 0
            && to_left.iter().any(|c| *c == arr[y][x])
            && to_right.iter().any(|c| *c == arr[y][x - 1])
        {
            arr[y][x] = b'l';
            x -= 1;
        }
        // go right
        else if x < arr[0].len() - 1
            && to_right.iter().any(|&c| c == arr[y][x])
            && to_left.iter().any(|&c| c == arr[y][x + 1])
        {
            arr[y][x] = b'r';
            x += 1;
        }
        // go up
        else if y > 0
            && to_up.iter().any(|&c| c == arr[y][x])
            && to_down.iter().any(|&c| c == arr[y - 1][x])
        {
            arr[y][x] = b'u';
            y -= 1;
        }
        // go down
        else if y < arr.len() - 1
            && to_down.iter().any(|&c| c == arr[y][x])
            && to_up.iter().any(|&c| c == arr[y + 1][x])
        {
            arr[y][x] = b'd';
            y += 1;
        } else {
            println!("fail {x},{y}");
            return None;
        }
        // take 1 step
        count += 1;
        if x < tx || x == tx && y < ty {
            tx = x;
            ty = y;
        }
    }
    arr[sy][sx] = b'l';
    println!("part1: {}", count / 2);

    println!("{} {}", arr.len(), arr[0].len());
    for s in arr.iter() {
        println!("{}", str::from_utf8(s).unwrap());
    }

    // top left corner is down
    println!("left top corner is {tx}, {ty} : {}", arr[ty][tx]);

    fn empty_spaces(a: &mut Vec<&mut [u8]>, maxx: usize, maxy: usize, x: usize, y: usize) -> u32 {
        if x >= maxx || y >= maxy {
            return 0;
        }
        match a[y][x] {
            b'u' | b'd' | b'l' | b'r' | b'x' => return 0,
            _ => {
                a[y][x] = b'x';
                return 1
                    + empty_spaces(a, maxx, maxy, x - 1, y)
                    + empty_spaces(a, maxx, maxy, x + 1, y)
                    + empty_spaces(a, maxx, maxy, x, y - 1)
                    + empty_spaces(a, maxx, maxy, x, y + 1);
            }
        }
    }

    // check left side for inner space
    x = tx;
    y = ty + 1;
    count = 0;
    let mut last = b'l';
    while x != tx || y != ty {
        let c = arr[y][x];
        match c {
            b'u' => {
                if last == b'l' {
                    count += empty_spaces(&mut arr, width, height, x, y + 1);
                }
                count += empty_spaces(&mut arr, width, height, x - 1, y);
                y -= 1;
            }
            b'd' => {
                if last == b'r' {
                    count += empty_spaces(&mut arr, width, height, x, y - 1);
                }
                count += empty_spaces(&mut arr, width, height, x + 1, y);
                y += 1;
            }
            b'l' => {
                if last == b'd' {
                    count += empty_spaces(&mut arr, width, height, x + 1, y);
                }
                count += empty_spaces(&mut arr, width, height, x, y + 1);
                x -= 1;
            }
            b'r' => {
                if last == b'u' {
                    count += empty_spaces(&mut arr, width, height, x - 1, y);
                }
                count += empty_spaces(&mut arr, width, height, x, y - 1);
                x += 1;
            }
            _ => return None,
        }
        last = c;
    }
    println!("spaces : {count}");
    Some(())
}
