use itertools::Itertools;
use petgraph::visit::Dfs;
use petgraph::visit::IntoNodeIdentifiers;
use petgraph::visit::Walker;
use petgraph::{Graph, Incoming};
use std::cmp::Ordering;
use std::collections::BTreeMap as M;
use std::collections::BTreeSet as S;
use std::fs;

#[derive(Debug, PartialEq, Eq, Clone)]
struct Brick((u32, u32, u32), (u32, u32, u32));

// compare z then x then y
impl Ord for Brick {
    fn cmp(&self, other: &Self) -> Ordering {
        self.0
             .2
            .cmp(&other.0 .2)
            .then(self.0 .0.cmp(&other.0 .0))
            .then(self.0 .1.cmp(&other.0 .1))
    }
}

impl PartialOrd for Brick {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(
            self.0
                 .2
                .cmp(&other.0 .2)
                .then(self.0 .0.cmp(&other.0 .0))
                .then(self.0 .1.cmp(&other.0 .1)),
        )
    }
}

fn new_brick(a: (u32, u32, u32), b: (u32, u32, u32)) -> Brick {
    // swap order so first is lower
    Brick(
        (a.0.min(b.0), a.1.min(b.1), a.2.min(b.2)),
        (a.0.max(b.0), a.1.max(b.1), a.2.max(b.2)),
    )
}
pub fn part1() {
    let contents = fs::read_to_string("./inputs/day22.txt").unwrap();
    let mut bricks = S::new();
    for l in contents.lines() {
        let (one, two) = l.split_once('~').unwrap();
        let one = one
            .split(',')
            .map(|n| n.parse::<u32>().unwrap())
            .collect_tuple()
            .unwrap();
        let two = two
            .split(',')
            .map(|n| n.parse::<u32>().unwrap())
            .collect_tuple()
            .unwrap();

        let brick = new_brick(one, two);
        bricks.insert(brick);
    }

    let mut layers: M<u32, Vec<(Brick, _)>> = M::new(); // top of fallen bricks
    let mut stack = Graph::new(); // a -> b a is on top of b
    for brick in bricks {
        let node = stack.add_node(brick.clone());
        let mut bottom = brick.0 .2;
        while bottom > 1 {
            bottom -= 1;
            let mut is_blocked = false;
            for (b, bidx) in layers.entry(bottom).or_insert_with(|| Vec::new()) {
                if (brick.1 .0 >= b.0 .0 && brick.0 .0 <= b.1 .0)
                    && (brick.1 .1 >= b.0 .1 && brick.0 .1 <= b.1 .1)
                {
                    is_blocked = true;
                    stack.add_edge(node, *bidx, ());
                }
            }
            if is_blocked {
                bottom += 1;
                break;
            }
        }
        layers
            .entry(bottom + brick.1 .2 - brick.0 .2)
            .or_insert_with(|| Vec::new())
            .push((brick, node));
    }

    let mut count = 0;
    'next: for node in stack.node_identifiers() {
        for top in stack.neighbors_directed(node, Incoming) {
            if stack.neighbors(top).count() == 1 {
                continue 'next;
            }
        }
        count += 1;
    }

    println!("{count}");
}

pub fn part2() {
    let contents = fs::read_to_string("./inputs/day22.txt").unwrap();
    let mut bricks = S::new();
    for l in contents.lines() {
        let (one, two) = l.split_once('~').unwrap();
        let one = one
            .split(',')
            .map(|n| n.parse::<u32>().unwrap())
            .collect_tuple()
            .unwrap();
        let two = two
            .split(',')
            .map(|n| n.parse::<u32>().unwrap())
            .collect_tuple()
            .unwrap();

        let brick = new_brick(one, two);
        bricks.insert(brick);
    }
    let total = bricks.len();

    let mut layers: M<u32, Vec<(Brick, _)>> = M::new(); // top of fallen bricks
    let mut stack = Graph::new(); // a <- b a is on top of b
    let ground = stack.add_node(Brick((0, 0, 0), (0, 0, 0)));
    for brick in bricks {
        let node = stack.add_node(brick.clone());
        let mut bottom = brick.0 .2;
        while bottom > 1 {
            bottom -= 1;
            let mut is_blocked = false;
            for (b, bidx) in layers.entry(bottom).or_insert_with(|| Vec::new()) {
                if (brick.1 .0 >= b.0 .0 && brick.0 .0 <= b.1 .0)
                    && (brick.1 .1 >= b.0 .1 && brick.0 .1 <= b.1 .1)
                {
                    is_blocked = true;
                    stack.add_edge(*bidx, node, ());
                }
            }
            if is_blocked {
                bottom += 1;
                break;
            }
        }
        if bottom == 1 {
            stack.add_edge(ground, node, ());
        }
        layers
            .entry(bottom + brick.1 .2 - brick.0 .2)
            .or_insert_with(|| Vec::new())
            .push((brick, node));
    }

    let mut travel = Dfs::new(&stack, ground);
    let mut count = 0;
    while let Some(brick) = travel.next(&stack) {
        if brick == ground {
            continue;
        }
        let mut new_stack = stack.clone();
        new_stack.remove_node(brick);
        let new_total = Dfs::new(&new_stack, ground).iter(&new_stack).count();
        // println!("{total} {new_total}");
        count += total - new_total;
    }

    println!("{count}");
}
