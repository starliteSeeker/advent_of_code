use itertools::Itertools;
use petgraph::dot::Dot;
use petgraph::visit::EdgeRef;
use petgraph::Graph;
use std::collections::BTreeMap as M;
use std::collections::BTreeSet as S;
use std::collections::VecDeque as Q;
use std::fs;

#[derive(Debug, PartialEq, Eq)]
enum Dir {
    Right,
    Up,
    Left,
    Down,
}

// input has no loops?
pub fn part1() {
    let contents = fs::read("./inputs/day23.txt").unwrap();
    let arr = contents
        .split(|&c| c == b'\n')
        .filter(|&l| !l.is_empty())
        .collect_vec();
    let h = arr.len();
    let w = arr[0].len();

    let start = (0, 1);
    let mut dist = vec![vec![0; w]; h];
    let mut queue = Q::new();
    queue.push_back((start, Dir::Down, 0));
    while let Some(((y, x), dir, d)) = queue.pop_front() {
        if x >= w || y >= h || arr[y][x] == b'#' {
            continue;
        }
        dist[y][x] = dist[y][x].max(d);
        match arr[y][x] {
            b'.' => {
                if dir != Dir::Left {
                    queue.push_back(((y, x.wrapping_add(1)), Dir::Right, d + 1));
                }
                if dir != Dir::Down {
                    queue.push_back(((y.wrapping_sub(1), x), Dir::Up, d + 1));
                }
                if dir != Dir::Right {
                    queue.push_back(((y, x.wrapping_sub(1)), Dir::Left, d + 1));
                }
                if dir != Dir::Up {
                    queue.push_back(((y.wrapping_add(1), x), Dir::Down, d + 1));
                }
            }
            b'>' => {
                if dir != Dir::Left {
                    queue.push_back(((y, x.wrapping_add(1)), Dir::Right, d + 1));
                }
            }
            b'^' => {
                if dir != Dir::Down {
                    queue.push_back(((y.wrapping_sub(1), x), Dir::Up, d + 1));
                }
            }
            b'<' => {
                if dir != Dir::Right {
                    queue.push_back(((y, x.wrapping_sub(1)), Dir::Left, d + 1));
                }
            }
            b'v' => {
                if dir != Dir::Up {
                    queue.push_back(((y.wrapping_add(1), x), Dir::Down, d + 1));
                }
            }
            _ => unreachable!(),
        }
    }

    println!("{}", dist[h - 1][w - 2]);
}

pub fn part2() {
    let contents = fs::read("./inputs/day23.txt").unwrap();
    let arr = contents
        .split(|&c| c == b'\n')
        .filter(|&l| !l.is_empty())
        .collect_vec();
    let h = arr.len();
    let w = arr[0].len();

    let mut queue = Q::new();
    let mut maze = Graph::new_undirected();
    let start = maze.add_node((0, 1));
    let mut junctions = M::new();
    junctions.insert((0, 1), start);
    queue.push_back(((0, 1), Dir::Down, 0, start));
    while let Some(((y, x), dir, mut d, mut last_junction)) = queue.pop_front() {
        // out of bounds
        if x >= w || y >= h || arr[y][x] == b'#' {
            continue;
        }

        // at junction or end
        if y == h - 1
            || (arr.get(y.wrapping_sub(1)).and_then(|&v| v.get(x)) != Some(&b'.')
                && arr.get(y.wrapping_add(1)).and_then(|&v| v.get(x)) != Some(&b'.')
                && arr.get(y).and_then(|&v| v.get(x.wrapping_sub(1))) != Some(&b'.')
                && arr.get(y).and_then(|&v| v.get(x.wrapping_add(1))) != Some(&b'.'))
        {
            if let Some(curr) = junctions.get(&(y, x)) {
                maze.add_edge(last_junction, *curr, d);
                continue;
            }
            let curr = maze.add_node((y, x));
            junctions.insert((y, x), curr);
            maze.add_edge(last_junction, curr, d);
            last_junction = curr;
            d = 0;
        }

        match arr[y][x] {
            b'.' => {
                if dir != Dir::Left {
                    queue.push_back(((y, x.wrapping_add(1)), Dir::Right, d + 1, last_junction));
                }
                if dir != Dir::Down {
                    queue.push_back(((y.wrapping_sub(1), x), Dir::Up, d + 1, last_junction));
                }
                if dir != Dir::Right {
                    queue.push_back(((y, x.wrapping_sub(1)), Dir::Left, d + 1, last_junction));
                }
                if dir != Dir::Up {
                    queue.push_back(((y.wrapping_add(1), x), Dir::Down, d + 1, last_junction));
                }
            }
            b'>' => {
                if dir != Dir::Left {
                    queue.push_back(((y, x.wrapping_add(1)), Dir::Right, d + 1, last_junction));
                }
            }
            b'^' => {
                if dir != Dir::Down {
                    queue.push_back(((y.wrapping_sub(1), x), Dir::Up, d + 1, last_junction));
                }
            }
            b'<' => {
                if dir != Dir::Right {
                    queue.push_back(((y, x.wrapping_sub(1)), Dir::Left, d + 1, last_junction));
                }
            }
            b'v' => {
                if dir != Dir::Up {
                    queue.push_back(((y.wrapping_add(1), x), Dir::Down, d + 1, last_junction));
                }
            }
            _ => unreachable!(),
        }
    }
    // println!("{:?}", Dot::new(&maze));

    let mut queue = Q::new();
    queue.push_back((junctions[&(0, 1)], 0, S::new()));
    let mut max_dist = 0;
    let end = junctions[&(h - 1, w - 2)];
    while let Some((curr, d, mut visited)) = queue.pop_front() {
        if curr == end {
            max_dist = max_dist.max(d);
            continue;
        }

        visited.insert(curr);
        for edge in maze.edges(curr) {
            let other = if edge.source() != curr {
                edge.source()
            } else {
                edge.target()
            };
            if visited.contains(&other) {
                continue;
            }
            queue.push_back((other, d + edge.weight(), visited.clone()));
        }
    }

    println!("{max_dist}");
}
