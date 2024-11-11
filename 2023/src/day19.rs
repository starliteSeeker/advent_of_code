use itertools::Itertools;
use std::collections::BTreeSet as Set;
use std::collections::HashMap;
use std::collections::VecDeque as Q;
use std::fs;
use std::ops::Add;
use std::str;

#[derive(Debug)]
struct Part {
    x: usize,
    m: usize,
    a: usize,
    s: usize,
}

type Name = Vec<u8>;
// struct Rule(dyn Fn(Part) -> Option<Name>);
struct Flow {
    name: Name,
    rules: Vec<Box<dyn Fn(&Part) -> Option<Name>>>,
    default: Name,
}

pub fn part1() {
    let contents = fs::read("./inputs/day19.txt").unwrap();
    let mut lines = contents.split(|&c| c == b'\n').collect_vec();
    lines.pop();
    let (one, two) = lines.split(|&v| v.is_empty()).collect_tuple().unwrap();
    let mut flows: HashMap<Name, Flow> = HashMap::new();
    for line in one {
        let l = line.len();
        let start = line.iter().position(|&c| c == b'{').unwrap();
        let name = line[..start].to_owned();
        let arr = &line[start + 1..l - 1].split(|&c| c == b',').collect_vec();
        let default = arr[arr.len() - 1].to_owned();
        let mut rules = Vec::new();
        for a in &arr[..arr.len() - 1] {
            let para = a
                .iter()
                .copied()
                .take_while(|c| u8::is_ascii_alphabetic(c))
                .collect_vec();
            let comp = a
                .iter()
                .copied()
                .filter(|&c| c == b'<' || c == b'>')
                .collect_vec();
            let num: usize = str::parse(
                str::from_utf8(
                    &a.iter()
                        .copied()
                        .filter(|c| u8::is_ascii_digit(c))
                        .collect_vec(),
                )
                .unwrap(),
            )
            .unwrap();
            let goal = a
                .iter()
                .copied()
                .skip_while(|&c| c != b':')
                .skip(1)
                .collect_vec();
            /*
            println!(
                "-- {} {} {} {}",
                str::from_utf8(&para).unwrap(),
                str::from_utf8(&comp).unwrap(),
                num,
                str::from_utf8(&goal).unwrap(),
            );
            */
            match (&para[..], &comp[..]) {
                (b"x", b"<") => rules
                    .push(Box::new(move |p: &Part| (p.x < num).then_some(goal.clone())) as Box<_>),
                (b"x", b">") => rules
                    .push(Box::new(move |p: &Part| (p.x > num).then_some(goal.clone())) as Box<_>),
                (b"m", b"<") => rules
                    .push(Box::new(move |p: &Part| (p.m < num).then_some(goal.clone())) as Box<_>),
                (b"m", b">") => rules
                    .push(Box::new(move |p: &Part| (p.m > num).then_some(goal.clone())) as Box<_>),
                (b"a", b"<") => rules
                    .push(Box::new(move |p: &Part| (p.a < num).then_some(goal.clone())) as Box<_>),
                (b"a", b">") => rules
                    .push(Box::new(move |p: &Part| (p.a > num).then_some(goal.clone())) as Box<_>),
                (b"s", b"<") => rules
                    .push(Box::new(move |p: &Part| (p.s < num).then_some(goal.clone())) as Box<_>),
                (b"s", b">") => rules
                    .push(Box::new(move |p: &Part| (p.s > num).then_some(goal.clone())) as Box<_>),
                _ => panic!(),
            }
        }
        /*
        println!(
            "{} {:?}",
            str::from_utf8(&name).unwrap(),
            str::from_utf8(&default).unwrap()
        );
        */
        flows.insert(
            name.clone(),
            Flow {
                name,
                rules,
                default,
            },
        );
    }
    let mut parts = Vec::new();
    for line in two {
        let temp = line
            .iter()
            .copied()
            .filter(|&c| u8::is_ascii_digit(&c) || c == b',')
            .collect_vec();
        let (x, m, a, s) = temp.split(|&c| c == b',').collect_tuple().unwrap();
        let x = str::from_utf8(x).unwrap().parse().unwrap();
        let m = str::from_utf8(m).unwrap().parse().unwrap();
        let a = str::from_utf8(a).unwrap().parse().unwrap();
        let s = str::from_utf8(s).unwrap().parse().unwrap();
        parts.push(Part { x, m, a, s });
    }

    // for l in flows {
    //     println!("{}", str::from_utf8(l).unwrap());
    // }

    let mut count = 0;
    for part in parts {
        let mut curr = b"in".to_vec();
        'here: loop {
            if curr == b"A" || curr == b"R" {
                break;
            }
            for f in &flows[&curr].rules {
                if let Some(next) = f(&part) {
                    curr = next;
                    continue 'here;
                }
            }
            // no matches, use default
            curr = flows[&curr].default.clone();
        }
        // println!("{:?} {:?}", part, curr);
        if curr == b"A" {
            count += part.x + part.m + part.a + part.s;
        }
    }

    println!("{count}");
}

#[derive(Debug, Clone)]
struct Ranges(Set<(usize, usize)>);

impl Add for &Ranges {
    type Output = Ranges;
    fn add(self, other: Self) -> Self::Output {
        let mut out = Set::new();
        let mut it = self.0.union(&other.0);
        let Some(mut curr) = it.next().map(|a| a.to_owned()) else {
            return Ranges(out);
        };
        for r @ (lo, hi) in it {
            if curr.1 + 1 < *lo {
                // no overlap
                out.insert(curr);
                curr = *r;
            } else {
                // lo is in curr
                if *hi <= curr.1 {
                    // r is in curr, do nothing
                } else {
                    curr.1 = *hi;
                }
            }
        }
        out.insert(curr);

        Ranges(out)
    }
}
fn split_ranges(Ranges(ranges): Ranges, here: usize) -> (Ranges, Ranges) {
    let mut left = Set::new();
    let mut right = Set::new();
    for r @ (lo, hi) in ranges {
        if hi < here {
            left.insert(r);
        } else if lo >= here {
            right.insert(r);
        } else {
            left.insert((lo, here - 1));
            right.insert((here, hi));
        }
    }
    return (Ranges(left), Ranges(right));
}
#[derive(Debug)]
enum Attr {
    X,
    M,
    A,
    S,
}
#[derive(Debug)]
enum Dir {
    LT,
    GT,
}
#[derive(Debug)]
struct Flow2 {
    rules: Vec<(Attr, Dir, usize, Vec<u8>)>,
    default: Vec<u8>,
}
#[derive(Debug, Clone)]
struct Part2 {
    x: Ranges,
    m: Ranges,
    a: Ranges,
    s: Ranges,
}
pub fn part2() {
    let contents = fs::read("./inputs/day19.txt").unwrap();
    let mut lines = contents.split(|&c| c == b'\n').collect_vec();
    lines.pop();
    let (one, _) = lines.split(|&v| v.is_empty()).collect_tuple().unwrap();
    let mut flows: HashMap<Name, Flow2> = HashMap::new();
    for line in one {
        let l = line.len();
        let start = line.iter().position(|&c| c == b'{').unwrap();
        let name = line[..start].to_owned();
        let arr = &line[start + 1..l - 1].split(|&c| c == b',').collect_vec();
        let default = arr[arr.len() - 1].to_owned();
        let mut rules = Vec::new();
        for a in &arr[..arr.len() - 1] {
            let para = a
                .iter()
                .copied()
                .take_while(|c| u8::is_ascii_alphabetic(c))
                .collect_vec();
            let comp = a
                .iter()
                .copied()
                .filter(|&c| c == b'<' || c == b'>')
                .collect_vec();
            let num: usize = str::parse(
                str::from_utf8(
                    &a.iter()
                        .copied()
                        .filter(|c| u8::is_ascii_digit(c))
                        .collect_vec(),
                )
                .unwrap(),
            )
            .unwrap();
            let goal = a
                .iter()
                .copied()
                .skip_while(|&c| c != b':')
                .skip(1)
                .collect_vec();
            /*
            println!(
                "-- {} {} {} {}",
                str::from_utf8(&para).unwrap(),
                str::from_utf8(&comp).unwrap(),
                num,
                str::from_utf8(&goal).unwrap(),
            );
            */
            match (&para[..], &comp[..]) {
                (b"x", b"<") => rules.push((Attr::X, Dir::LT, num, goal)),
                (b"x", b">") => rules.push((Attr::X, Dir::GT, num, goal)),
                (b"m", b"<") => rules.push((Attr::M, Dir::LT, num, goal)),
                (b"m", b">") => rules.push((Attr::M, Dir::GT, num, goal)),
                (b"a", b"<") => rules.push((Attr::A, Dir::LT, num, goal)),
                (b"a", b">") => rules.push((Attr::A, Dir::GT, num, goal)),
                (b"s", b"<") => rules.push((Attr::S, Dir::LT, num, goal)),
                (b"s", b">") => rules.push((Attr::S, Dir::GT, num, goal)),
                _ => panic!(),
            }
        }
        flows.insert(name.clone(), Flow2 { rules, default });
    }

    let mut queue = Q::new();
    queue.push_back((
        b"in".to_vec(),
        Part2 {
            x: Ranges(Set::from([(1, 4000)])),
            m: Ranges(Set::from([(1, 4000)])),
            a: Ranges(Set::from([(1, 4000)])),
            s: Ranges(Set::from([(1, 4000)])),
        },
    ));

    let mut accepted = Vec::new();
    let mut rejected = Vec::new();
    while let Some((name, ranges)) = queue.pop_front() {
        // looping is not possible
        if name == b"A" {
            accepted.push(ranges.clone());
            continue;
        } else if name == b"R" {
            rejected.push(ranges.clone());
            continue;
        }
        let flow = &flows[&name];
        let mut curr = ranges;
        for (attr, dir, num, goal) in &flow.rules {
            match (attr, dir) {
                (Attr::X, Dir::LT) => {
                    let (left, right) = split_ranges(curr.x.clone(), *num);
                    let mut next = curr.clone();
                    next.x = left;
                    queue.push_back((goal.to_vec(), next));
                    curr.x = right;
                }
                (Attr::X, Dir::GT) => {
                    let (left, right) = split_ranges(curr.x.clone(), *num + 1);
                    let mut next = curr.clone();
                    next.x = right;
                    queue.push_back((goal.to_vec(), next));
                    curr.x = left;
                }
                (Attr::M, Dir::LT) => {
                    let (left, right) = split_ranges(curr.m.clone(), *num);
                    let mut next = curr.clone();
                    next.m = left;
                    queue.push_back((goal.to_vec(), next));
                    curr.m = right;
                }
                (Attr::M, Dir::GT) => {
                    let (left, right) = split_ranges(curr.m.clone(), *num + 1);
                    let mut next = curr.clone();
                    next.m = right;
                    queue.push_back((goal.to_vec(), next));
                    curr.m = left;
                }
                (Attr::A, Dir::LT) => {
                    let (left, right) = split_ranges(curr.a.clone(), *num);
                    let mut next = curr.clone();
                    next.a = left;
                    queue.push_back((goal.to_vec(), next));
                    curr.a = right;
                }
                (Attr::A, Dir::GT) => {
                    let (left, right) = split_ranges(curr.a.clone(), *num + 1);
                    let mut next = curr.clone();
                    next.a = right;
                    queue.push_back((goal.to_vec(), next));
                    curr.a = left;
                }
                (Attr::S, Dir::LT) => {
                    let (left, right) = split_ranges(curr.s.clone(), *num);
                    let mut next = curr.clone();
                    next.s = left;
                    queue.push_back((goal.to_vec(), next));
                    curr.s = right;
                }
                (Attr::S, Dir::GT) => {
                    let (left, right) = split_ranges(curr.s.clone(), *num + 1);
                    let mut next = curr.clone();
                    next.s = right;
                    queue.push_back((goal.to_vec(), next));
                    curr.s = left;
                }
            }
        }
        queue.push_back((flow.default.clone(), curr));
    }

    // complete overlap is not possible
    let mut count1 = 0;
    for r in accepted {
        let mut i = 1;
        let mut j = 0;
        for (lo, hi) in r.x.0 {
            j += hi - lo + 1;
        }
        i *= j;
        j = 0;
        for (lo, hi) in r.m.0 {
            j += hi - lo + 1;
        }
        i *= j;
        j = 0;
        for (lo, hi) in r.a.0 {
            j += hi - lo + 1;
        }
        i *= j;
        j = 0;
        for (lo, hi) in r.s.0 {
            j += hi - lo + 1;
        }
        i *= j;
        count1 += i;
    }
    println!("accepted {count1}");
    let mut count2 = 0;
    for r in rejected {
        let mut i = 1;
        let mut j = 0;
        for (lo, hi) in r.x.0 {
            j += hi - lo + 1;
        }
        i *= j;
        j = 0;
        for (lo, hi) in r.m.0 {
            j += hi - lo + 1;
        }
        i *= j;
        j = 0;
        for (lo, hi) in r.a.0 {
            j += hi - lo + 1;
        }
        i *= j;
        j = 0;
        for (lo, hi) in r.s.0 {
            j += hi - lo + 1;
        }
        i *= j;
        count2 += i;
    }
    println!("rejected {count2}");
    println!("{} {}", count1 + count2, 4000 as usize * 4000 * 4000 * 4000);
}
