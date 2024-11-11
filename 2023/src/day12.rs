use std::collections::HashMap;
use std::fs;

pub fn part1() -> Option<()> {
    let contents = fs::read("./inputs/day12.txt").unwrap();
    let mut count = 0;

    for line in contents.split(|&c| c == b'\n').filter(|&l| !l.is_empty()) {
        let x: Vec<_> = line.splitn(2, |&c| c == b' ').collect();
        let conditions: &[u8] = &[x[0], b"?", x[0], b"?", x[0], b"?", x[0], b"?", x[0]].concat();
        let consecutives: Vec<usize> = x[1]
            .split(|&c| c == b',')
            .map(|c| std::str::from_utf8(c).unwrap().parse::<usize>().unwrap())
            .cycle()
            .take(x[1].split(|&c| c == b',').count() * 5)
            .collect();
        /*
        let conditions: &[u8] = x[0];
        let consecutives: Vec<usize> = x[1]
            .split(|&c| c == b',')
            .map(|c| std::str::from_utf8(c).unwrap().parse::<usize>().unwrap())
            .collect();
        */
        println!(
            "--- {:?} {:?}",
            std::str::from_utf8(conditions).unwrap(),
            consecutives
        );

        let mut m = HashMap::new();
        let ans = solve_memo(conditions, &consecutives[..], &mut m)?;
        println!("{} {}", std::str::from_utf8(conditions).unwrap(), ans);
        count += ans;
    }
    println!("{count}");
    Some(())
}

fn solve_memo<'a, 'b>(
    condition: &'a [u8],
    consecutives: &'b [usize],
    m: &mut HashMap<(&'a [u8], &'b [usize]), usize>,
) -> Option<usize> {
    if let Some(n) = m.get(&(condition, consecutives)) {
        return Some(*n);
    }
    let n = solve(condition, consecutives, m)?;
    m.insert((condition, consecutives), n);
    return Some(n);
}

fn solve<'a, 'b>(
    condition: &'a [u8],
    consecutives: &'b [usize],
    m: &mut HashMap<(&'a [u8], &'b [usize]), usize>,
) -> Option<usize> {
    if consecutives.len() == 0 {
        if condition.iter().any(|&c| c == b'#') {
            return Some(0);
        } else {
            return Some(1);
        }
    }

    if condition.len() < consecutives[0] {
        return Some(0);
    }

    /*
    println!(
        "--- {:?} {:?} {} {}",
        std::str::from_utf8(condition).unwrap(),
        consecutives,
        dot()?,
        pound()?,
    );
    */

    match condition[0] {
        b'.' => return solve_memo(&condition[1..], consecutives, m),
        b'#' => {
            if condition[..consecutives[0]].iter().all(|&c| c != b'.') {
                if condition.len() == consecutives[0] && consecutives.len() == 1 {
                    return Some(1);
                } else if condition.len() > consecutives[0] && condition[consecutives[0]] != b'#' {
                    return solve_memo(&condition[consecutives[0] + 1..], &consecutives[1..], m);
                } else {
                    return Some(0);
                }
            } else {
                return Some(0);
            }
        }

        b'?' => {
            let dot = solve_memo(&condition[1..], consecutives, m);
            let pound = {
                if condition[..consecutives[0]].iter().all(|&c| c != b'.') {
                    if condition.len() == consecutives[0] && consecutives.len() == 1 {
                        Some(1)
                    } else if condition.len() > consecutives[0]
                        && condition[consecutives[0]] != b'#'
                    {
                        solve_memo(&condition[consecutives[0] + 1..], &consecutives[1..], m)
                    } else {
                        Some(0)
                    }
                } else {
                    Some(0)
                }
            };
            return Some(dot? + pound?);
        }
        _ => unreachable!(),
    }
}

/*
pub fn part1() -> Option<()> {
    let contents = fs::read("./inputs/day12_2.txt").unwrap();
    let mut count = 0;

    for line in contents.split(|&c| c == b'\n').filter(|&l| !l.is_empty()) {
        let x: Vec<_> = line.splitn(2, |&c| c == b' ').collect();
        let conditions: &[u8] = &[x[0], b"?", x[0], b"?", x[0], b"?", x[0], b"?", x[0]].concat();
        let consecutives: Vec<isize> = x[1]
            .split(|&c| c == b',')
            .map(|c| std::str::from_utf8(c).unwrap().parse::<isize>().unwrap())
            .cycle()
            .take(x[1].split(|&c| c == b',').count() * 5)
            .collect();
        println!(
            "--- {:?} {:?}",
            std::str::from_utf8(conditions).unwrap(),
            consecutives
        );

        let ans = solve(&mut vec![conditions], consecutives)?;
        // println!("{} {}", std::str::from_utf8(conditions[0]).unwrap(), ans);
        count += ans;
    }
    println!("{count}");
    Some(())
}

fn solve(conditions: &mut Vec<&[u8]>, consecutives: Vec<isize>) -> Option<isize> {
    // println!(
    //     "--- {:?} {:?}",
    //     conditions
    //         .iter()
    //         .map(|&s| std::str::from_utf8(s).unwrap())
    //         .collect::<Vec<_>>(),
    //     consecutives
    // );

    if consecutives.is_empty() {
        // nothing needs to be filled
        if conditions.iter().any(|&s| s.iter().any(|&c| c == b'#')) {
            return Some(0);
        } else {
            return Some(1);
        }
    }

    // split conditions on '.'
    let mut x: Vec<_> = conditions
        .iter()
        .map(|&l| {
            l.split(|&m| m == b'.')
                .filter(|&m| !m.is_empty())
                .collect::<Vec<_>>()
        })
        .collect::<Vec<_>>()
        .concat();

    if x.len() > 1 {
        // need to divide more
        let mut count = 0;
        let mut tail = x.split_off(1);
        for i in 0..=consecutives.len() {
            let mut front = consecutives.clone();
            let back = front.split_off(i);
            count += solve(&mut x, front)? * solve(&mut tail, back)?;
        }
        Some(count)
    } else {
        let cond = x[0];
        if cond.iter().all(|&c| c == b'?') {
            // all question mark, combination problem
            let n: isize =
                isize::try_from(cond.len()).unwrap() - consecutives.iter().sum::<isize>() + 1;
            let l = isize::try_from(consecutives.len()).unwrap();
            if n <= 0 {
                Some(0)
            } else {
                Some((n - l + 1..=n).product::<isize>() / (1..=l).product::<isize>())
            }
        } else if cond.iter().all(|&c| c == b'#') {
            // no question mark, no guess needed
            if consecutives.len() == 1 && isize::try_from(cond.len()).unwrap() == consecutives[0] {
                Some(1)
            } else {
                Some(0)
            }
        } else if consecutives.len() == 1 {
            let consec = usize::try_from(consecutives[0]).unwrap();
            // only 1 segment to fit in
            match consec.cmp(&cond.len()) {
                Ordering::Less => {
                    let cap = cond.len() - consec;
                    let first = cond.iter().position(|&c| c == b'#').unwrap().min(cap);
                    let last = cond.iter().rev().position(|&c| c == b'#').unwrap().min(cap);
                    if cond.len() - first - last > consec {
                        Some(0)
                    } else {
                        let margin = consec - (cond.len() - first - last);
                        let ans = isize::try_from(margin.min(first).min(last) + 1).unwrap();
                        Some(ans)
                    }
                }
                Ordering::Equal => Some(1),
                Ordering::Greater => Some(0),
            }
        } else {
            // mix of # and ?, multiple segments, more splitting needed
            let mut count = 0;
            let mut head = consecutives.clone();
            let tail = head.split_off(1);
            for i in 1..cond.len() - 1 {
                if cond[i] == b'#' {
                    // cant split here
                    continue;
                }
                let mut front = vec![&cond[0..i]];
                let mut bak = cond[i + 1..].to_vec();
                bak[0] = b'#';
                let bak: &[u8] = &bak;
                let mut back = vec![bak];
                count += solve(&mut front, head.clone())? * solve(&mut back, tail.clone())?;
            }
            Some(count)
        }
        //     println!(
        //         "fail {} {:?}",
        //         std::str::from_utf8(cond).unwrap(),
        //         consecutives
        //     );
        //     None
        // }
    }
}
*/
