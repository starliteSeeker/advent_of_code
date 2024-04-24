use itertools::Itertools;
use std::collections::BTreeMap as Map;
use std::collections::VecDeque;
use std::fs;

trait Module<'a> {
    fn ping(&mut self, queue: &mut VecDeque<(&'a str, &'a str, bool)>, signal: bool, from: &'a str);
}

struct FlipFlop<'a> {
    name: &'a str,
    state: bool,
    targets: Vec<&'a str>,
}
impl<'a> Module<'a> for FlipFlop<'a> {
    fn ping(&mut self, queue: &mut VecDeque<(&'a str, &'a str, bool)>, signal: bool, _: &'a str) {
        if signal {
            return;
        }
        self.state = !self.state;
        for t in &self.targets {
            queue.push_back((t, self.name, self.state));
        }
    }
}

struct Conjunction<'a> {
    name: &'a str,
    state: Map<&'a str, bool>,
    targets: Vec<&'a str>,
}
impl<'a> Module<'a> for Conjunction<'a> {
    fn ping(
        &mut self,
        queue: &mut VecDeque<(&'a str, &'a str, bool)>,
        signal: bool,
        from: &'a str,
    ) {
        self.state.insert(from, signal);
        let sig = !self.state.values().all(|&x| x == true);
        for t in &self.targets {
            queue.push_back((t, self.name, sig));
        }
    }
}

struct Broadcast<'a> {
    name: &'a str,
    targets: Vec<&'a str>,
}
impl<'a> Module<'a> for Broadcast<'a> {
    fn ping(&mut self, queue: &mut VecDeque<(&'a str, &'a str, bool)>, signal: bool, _: &'a str) {
        for t in &self.targets {
            queue.push_back((t, self.name, signal));
        }
    }
}

pub fn part1() {
    let contents = fs::read_to_string("./inputs/day20.txt").unwrap();

    // build modules
    let mut modules: Map<&str, Box<dyn Module>> = Map::new();
    let mut arr = Vec::new();
    for line in contents.lines() {
        // println!("{line}");
        let (name, targets) = line.split(" -> ").collect_tuple().unwrap();
        let (typ, name) = name.split_at(1);
        let targets = targets.split(", ").collect_vec();
        arr.push((typ, name, targets));
    }

    let mut sources = Map::new();
    for (typ, name, _) in &arr {
        if *typ != "&" {
            continue;
        }
        let mut v = Vec::new();
        for (_, name2, targets2) in &arr {
            if let Some(_) = targets2.iter().find(|&&x| x == *name) {
                v.push(*name2);
            }
        }
        sources.insert(*name, v);
    }
    for (typ, name, targets) in arr {
        match typ {
            "b" => modules.insert(name, Box::new(Broadcast { name, targets })),
            "%" => modules.insert(
                name,
                Box::new(FlipFlop {
                    name,
                    state: false,
                    targets,
                }),
            ),
            "&" => modules.insert(
                name,
                Box::new(Conjunction {
                    name,
                    state: sources[name].iter().map(|&x| (x, false)).collect(),
                    targets,
                }),
            ),
            _ => unreachable!(),
        };
    }

    // push button
    let mut low: usize = 0;
    let mut high = 0;
    for _ in 0..1000 {
        let mut queue = VecDeque::new();
        queue.push_back(("roadcaster", "button", false));
        while let Some(x @ (name, from, signal)) = queue.pop_front() {
            // println!("{x:?}");
            if signal {
                high += 1;
            } else {
                low += 1;
            }
            if let Some(m) = modules.get_mut(name) {
                m.ping(&mut queue, signal, from);
            }
        }
    }
    println!("{}", low * high);
}

// feels like theres bunch of edge cases but oh well (¬_¬)
pub fn part2() {
    let contents = fs::read_to_string("./inputs/day20.txt").unwrap();

    // build modules
    let mut modules: Map<&str, Box<dyn Module>> = Map::new();
    let mut arr = Vec::new();
    for line in contents.lines() {
        // println!("{line}");
        let (name, targets) = line.split(" -> ").collect_tuple().unwrap();
        let (typ, name) = name.split_at(1);
        let targets = targets.split(", ").collect_vec();
        arr.push((typ, name, targets));
    }

    let mut sources = Map::new();
    for (typ, name, _) in &arr {
        if *typ != "&" {
            continue;
        }
        let mut v = Vec::new();
        for (_, name2, targets2) in &arr {
            if let Some(_) = targets2.iter().find(|&&x| x == *name) {
                v.push(*name2);
            }
        }
        sources.insert(*name, v);
    }
    let arr2 = arr.clone();
    let avails = arr2.iter().map(|(_, n, _)| n).collect_vec();
    for (typ, name, targets) in arr {
        let targets = targets
            .into_iter()
            .filter(|&n| n == "rx" || avails.iter().find(|&&&m| m == n).is_some())
            .collect_vec();
        match typ {
            "b" => modules.insert(name, Box::new(Broadcast { name, targets })),
            "%" => modules.insert(
                name,
                Box::new(FlipFlop {
                    name,
                    state: false,
                    targets,
                }),
            ),
            "&" => modules.insert(
                name,
                Box::new(Conjunction {
                    name,
                    state: sources[name].iter().map(|&x| (x, false)).collect(),
                    targets,
                }),
            ),
            _ => unreachable!(),
        };
    }

    // push button
    let mut presses = 0;
    /*
    let mut th = Vec::new();
    let mut ch = Vec::new();
    let mut gh = Vec::new();
    let mut sv = Vec::new();
    */
    let mut xs_cycle = -1;
    let mut ml_cycle = -1;
    let mut kl_cycle = -1;
    let mut jn_cycle = -1;
    'done: loop {
        presses += 1;
        let mut queue = VecDeque::new();
        queue.push_back(("roadcaster", "button", false));
        while let Some(x @ (name, from, signal)) = queue.pop_front() {
            // println!("{x:?}");
            match from {
                "th" => {
                    if xs_cycle == -1 && signal {
                        xs_cycle = presses;
                    }
                } // th.push(signal),
                "ch" => {
                    if ml_cycle == -1 && signal {
                        ml_cycle = presses;
                    }
                } // th.push(signal),
                "gh" => {
                    if kl_cycle == -1 && signal {
                        kl_cycle = presses;
                    }
                } // th.push(signal),
                "sv" => {
                    if jn_cycle == -1 && signal {
                        jn_cycle = presses;
                    }
                } // th.push(signal),
                _ => (),
            }

            if let Some(m) = modules.get_mut(name) {
                m.ping(&mut queue, signal, from);
            }
        }
        /*
        println!(
            "{}{}{}{}",
            if th[presses - 1] { 1 } else { 0 },
            if ch[presses - 1] { 1 } else { 0 },
            if gh[presses - 1] { 1 } else { 0 },
            if sv[presses - 1] { 1 } else { 0 },
        );
        if presses == 1000 {
            break;
        }
        */
        if xs_cycle != -1 && ml_cycle != -1 && kl_cycle != -1 && jn_cycle != -1 {
            break;
        }
    }
    println!("{} {} {} {}", xs_cycle, ml_cycle, kl_cycle, jn_cycle);
}

// broadcaster -> xs -> (...) -> th -> cn -> rx
//             -> ml -> (...) -> ch ->
//             -> kl -> (...) -> gh ->
//             -> jn -> (...) -> sv ->
// cn sends low when all inputs are high
pub fn draw() {
    let contents = fs::read_to_string("./inputs/day20.txt").unwrap();

    for line in contents.lines() {
        let (name, targets) = line.split(" -> ").collect_tuple().unwrap();
        let (typ, name) = name.split_at(1);
        let targets = targets.split(", ").collect_vec();
        for t in targets {
            println!("{} {}", name, t);
        }
    }
}
