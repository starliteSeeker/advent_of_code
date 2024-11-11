use itertools::Itertools;
// use petgraph::dot::{Config, Dot};
use petgraph::graph::EdgeReference;
use petgraph::graph::NodeIndex;
use petgraph::visit::Walker;
use petgraph::visit::{Dfs, EdgeRef};
use petgraph::Graph;
use petgraph::Undirected;
use rand::thread_rng;
use rand::Rng;
use std::collections::BTreeMap as M;
use std::collections::VecDeque as Q;
use std::fs;

pub fn part1() {
    let contents = fs::read_to_string("./inputs/day25.txt").unwrap();
    let mut graph = Graph::new_undirected();
    let mut node_map: M<&str, NodeIndex> = M::new();
    for l in contents.lines() {
        let (left, right) = l.split_once(": ").unwrap();
        let left_node = if let Some(n) = node_map.get(&left) {
            *n
        } else {
            let x = graph.add_node(left);
            node_map.insert(left, x);
            x
        };

        for r in right.split(" ") {
            let r_node = if let Some(n) = node_map.get(&r) {
                *n
            } else {
                let x = graph.add_node(r);
                node_map.insert(r, x);
                x
            };
            graph.add_edge(left_node, r_node, ());
        }
    }

    let graph_clone = graph.clone();
    let (edge1, edge2, edge3) = part1_inner(&graph_clone);
    graph.remove_edge(edge1.id());
    graph.remove_edge(edge2.id());
    graph.remove_edge(edge3.id());

    let max_node = graph.node_count();
    let one_side = Dfs::new(&graph, NodeIndex::new(0)).iter(&graph).count();
    println!("{one_side} {}", one_side * (max_node - one_side));
}

fn part1_inner<A, B: std::cmp::PartialEq>(
    graph: &Graph<A, B, Undirected>,
) -> (
    EdgeReference<'_, B>,
    EdgeReference<'_, B>,
    EdgeReference<'_, B>,
) {
    // pick two nodes, assume theyre on different "sides"
    let mut rng = thread_rng();
    let max_node = graph.node_count();
    loop {
        let start = NodeIndex::new(rng.gen_range(0..max_node));
        let end = NodeIndex::new(rng.gen_range(0..max_node));
        if start == end {
            continue;
        }

        println!("{start:?} {end:?}");
        let path = dijkstra(&graph, start, end, Vec::new()).unwrap();

        // println!("{path:?}");

        // guess that one edge is part of the 3 edges to disconnect
        for (a, b) in path.iter().tuple_windows() {
            // delete this edge and start another dijkstra
            let edge = graph.edges_connecting(*a, *b).next().unwrap();
            let path2 = dijkstra(&graph, start, end, vec![edge]).unwrap();
            // println!("{path2:?}");
            for (c, d) in path2.iter().tuple_windows() {
                let edge2 = graph.edges_connecting(*c, *d).next().unwrap();
                let path3 = dijkstra(&graph, start, end, vec![edge, edge2]).unwrap();
                // println!("{path3:?}");
                for (e, f) in path3.iter().tuple_windows() {
                    let edge3 = graph.edges_connecting(*e, *f).next().unwrap();
                    let path4 = dijkstra(&graph, start, end, vec![edge, edge2, edge3]);
                    if path4.is_none() {
                        // no connection, we guessed right
                        return (edge, edge2, edge3);
                    }
                }
            }
        }
    }
}

fn dijkstra<A, B: std::cmp::PartialEq>(
    graph: &Graph<A, B, Undirected>,
    start: NodeIndex<u32>,
    end: NodeIndex<u32>,
    deleted: Vec<EdgeReference<'_, B>>,
) -> Option<Vec<NodeIndex<u32>>> {
    let max_node = graph.node_count();
    let mut path_vec = vec![None; max_node];
    path_vec[start.index()] = Some(start);
    let mut queue = Q::new();
    queue.push_back(start);
    let mut good = false;
    while let Some(curr) = queue.pop_front() {
        if curr == end {
            good = true;
            break;
        }

        for e in graph.edges(curr) {
            if deleted.iter().any(|&c| c == e) {
                continue;
            }
            let next = if e.source() != curr {
                e.source()
            } else {
                e.target()
            };
            if path_vec[next.index()].is_some() {
                continue;
            }

            path_vec[next.index()] = Some(curr);
            queue.push_back(next);
        }
    }
    if !good {
        return None;
    }

    let mut curr = end;
    let mut path = Vec::new();
    path.push(curr);
    while let Some(n) = path_vec[curr.index()] {
        path.push(n);
        curr = n;
        if n == start {
            break;
        }
    }

    Some(path)
}
