use std::collections::VecDeque as Q;
use std::fs;

struct Item {
    pos: (usize, usize),
    from_vert: bool,
}

pub fn part1() {
    let contents = fs::read("./inputs/day17.txt").unwrap();
    let arr = contents
        .split(|&c| c == b'\n')
        .filter(|&a| !a.is_empty())
        .map(|v| v.iter().map(|&c| (c - b'0') as usize).collect::<Vec<_>>())
        .collect::<Vec<_>>();

    let h = arr.len();
    let w = arr[0].len();
    println!("grid ({h}, {w})");
    let max_d = 10 * w * h;
    let mut dists: Vec<Vec<_>> = vec![vec![(max_d, max_d); w]; h];
    dists[0][0] = (0, 0);

    let mut queue: Q<Item> = Q::new();

    travel(
        &mut queue,
        &mut dists,
        &arr,
        Item {
            pos: (0, 0),
            from_vert: true,
        },
    );
    travel(
        &mut queue,
        &mut dists,
        &arr,
        Item {
            pos: (0, 0),
            from_vert: false,
        },
    );

    while let Some(item) = queue.pop_front() {
        travel(&mut queue, &mut dists, &arr, item);
    }

    println!("{:?}", dists[h - 1][w - 1]);
    /*
    for d in dists {
        println!("{:?}", d);
    }
    */
}

fn travel(
    queue: &mut Q<Item>,
    dists: &mut Vec<Vec<(usize, usize)>>,
    arr: &Vec<Vec<usize>>,
    Item {
        pos: (pos_i, pos_j),
        from_vert,
    }: Item,
) {
    let h = arr.len();
    let w = arr[0].len();

    if from_vert {
        // go left
        let mut new_dist = dists[pos_i][pos_j].1;
        for i in 1..=10 {
            let new_j = pos_j.wrapping_sub(i);
            if new_j >= w {
                break;
            }
            new_dist += arr[pos_i][new_j];
            if i < 4 {
                continue;
            }
            if new_dist < dists[pos_i][new_j].0 {
                dists[pos_i][new_j].0 = new_dist;
                queue.push_back(Item {
                    pos: (pos_i, new_j),
                    from_vert: false,
                });
            }
        }

        // go right
        let mut new_dist = dists[pos_i][pos_j].1;
        for i in 1..=10 {
            let new_j = pos_j + i;
            if new_j >= w {
                break;
            }
            new_dist += arr[pos_i][new_j];
            if i < 4 {
                continue;
            }
            if new_dist < dists[pos_i][new_j].0 {
                dists[pos_i][new_j].0 = new_dist;
                queue.push_back(Item {
                    pos: (pos_i, new_j),
                    from_vert: false,
                });
            }
        }
    } else {
        // go up
        let mut new_dist = dists[pos_i][pos_j].0;
        for i in 1..=10 {
            let new_i = pos_i.wrapping_sub(i);
            if new_i >= h {
                break;
            }
            new_dist += arr[new_i][pos_j];
            if i < 4 {
                continue;
            }
            if new_dist < dists[new_i][pos_j].1 {
                dists[new_i][pos_j].1 = new_dist;
                queue.push_back(Item {
                    pos: (new_i, pos_j),
                    from_vert: true,
                });
            }
        }

        // go down
        let mut new_dist = dists[pos_i][pos_j].0;
        for i in 1..=10 {
            let new_i = pos_i + i;
            if new_i >= h {
                break;
            }
            new_dist += arr[new_i][pos_j];
            if i < 4 {
                continue;
            }
            if new_dist < dists[new_i][pos_j].1 {
                dists[new_i][pos_j].1 = new_dist;
                queue.push_back(Item {
                    pos: (new_i, pos_j),
                    from_vert: true,
                });
            }
        }
    }
}
