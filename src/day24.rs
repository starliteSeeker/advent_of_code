use itertools::Itertools;
use std::fs;
use std::str;

#[derive(Debug)]
struct Stone {
    pos: (f64, f64, f64),
    vel: (f64, f64, f64),
}

pub fn part1() {
    let contents = fs::read_to_string("./inputs/day24.txt").unwrap();
    let mut stones = Vec::new();
    for l in contents.lines() {
        let (one, two) = l.split_once(" @ ").unwrap();
        let stone = Stone {
            pos: one
                .split(", ")
                .map(|s| str::parse(s.trim()).unwrap())
                .collect_tuple()
                .unwrap(),
            vel: two
                .split(", ")
                .map(|s| str::parse(s.trim()).unwrap())
                .collect_tuple()
                .unwrap(),
        };
        stones.push(stone);
    }

    let mut count = 0;
    for (first, second) in stones.iter().tuple_combinations() {
        // p1x + v1x * t = p2x + v2x * s && p1y + v1y * t = p2y + v2y * s
        // s = (v1y * p2x - v1y * p1x - v1x * p2y + v1x * p1y) / (v1x * v2y - v1y * v2x)
        let s =
            (first.vel.1 * second.pos.0 - first.vel.1 * first.pos.0 - first.vel.0 * second.pos.1
                + first.vel.0 * first.pos.1)
                / (first.vel.0 * second.vel.1 - first.vel.1 * second.vel.0);
        let t = (second.pos.0 + second.vel.0 * s - first.pos.0) / first.vel.0;
        let x = second.pos.0 + second.vel.0 * s;
        let y = second.pos.1 + second.vel.1 * s;

        if t >= 0.0
            && s >= 0.0
            && x >= 200000000000000.0
            && x <= 400000000000000.0
            && y >= 200000000000000.0
            && y <= 400000000000000.0
        {
            count += 1;
        }
    }
    println!("{count}");
}

#[derive(Debug)]
struct Stone2 {
    pos: (isize, isize, isize),
    vel: (isize, isize, isize),
}

// yup math
pub fn part2() {
    let contents = fs::read_to_string("./inputs/day24.txt").unwrap();
    let mut stones = Vec::new();
    for l in contents.lines() {
        let (one, two) = l.split_once(" @ ").unwrap();
        let stone = Stone2 {
            pos: one
                .split(", ")
                .map(|s| str::parse(s.trim()).unwrap())
                .collect_tuple()
                .unwrap(),
            vel: two
                .split(", ")
                .map(|s| str::parse(s.trim()).unwrap())
                .collect_tuple()
                .unwrap(),
        };
        stones.push(stone);
    }

    // guessing v solution for equation of the form ax + by = c
    // https://en.wikipedia.org/wiki/Diophantine_equation#Linear_Diophantine_equations
    'vx: for vx in -600..600 {
        // guess vx so (v1x - vx) * t1 + (-v2x + vx) * t2 = -p1x + p2x have integer solution
        for (one, two) in stones.iter().tuple_combinations() {
            let gcd = gcd(one.vel.0 - vx, vx - two.vel.0);
            if gcd != 0 && (two.pos.0 - one.pos.0) % gcd != 0 {
                continue 'vx;
            }
        }

        // pretend vx is correct, guess vy
        'vy: for vy in -600..600 {
            for (one, two) in stones.iter().tuple_combinations() {
                let gcd = gcd(one.vel.1 - vy, vy - two.vel.1);
                if gcd != 0 && (two.pos.1 - one.pos.1) % gcd != 0 {
                    continue 'vy;
                }
            }

            // pretend vy is correct, guess vz
            'vz: for vz in -600..600 {
                for (one, two) in stones.iter().tuple_combinations() {
                    let gcd = gcd(one.vel.2 - vz, vz - two.vel.2);
                    if gcd != 0 && (two.pos.2 - one.pos.2) % gcd != 0 {
                        continue 'vz;
                    }
                }

                // "found" vx, vy, vz: -99 -269 81
                // solve for t1 then original position
                let one = &stones[0];
                let two = &stones[1];
                // cramers rule
                let t1 = ((two.pos.0 - one.pos.0) * (vy - two.vel.1)
                    - (two.pos.1 - one.pos.1) * (vx - two.vel.0))
                    / ((one.vel.0 - vx) * (vy - two.vel.1) - (one.vel.1 - vy) * (vx - two.vel.0));
                let px = one.pos.0 + (one.vel.0 - vx) * t1;
                let py = one.pos.1 + (one.vel.1 - vy) * t1;
                let pz = one.pos.2 + (one.vel.2 - vz) * t1;
                println!("{t1}: {px} {py} {pz}");
                println!("{}", px + py + pz);
                return;
            }
        }
    }
}

// taken from wikipedia :)
fn gcd(mut u: isize, mut v: isize) -> isize {
    u = u.abs();
    v = v.abs();

    // Base cases: gcd(n, 0) = gcd(0, n) = n
    if u == 0 {
        return v;
    } else if v == 0 {
        return u;
    }

    // Using identities 2 and 3:
    // gcd(2ⁱ u, 2ʲ v) = 2ᵏ gcd(u, v) with u, v odd and k = min(i, j)
    // 2ᵏ is the greatest power of two that divides both 2ⁱ u and 2ʲ v
    let i = u.trailing_zeros();
    u >>= i;
    let j = v.trailing_zeros();
    v >>= j;
    let k = i.min(j);

    loop {
        // u and v are odd at the start of the loop
        debug_assert!(u % 2 == 1, "u = {} should be odd", u);
        debug_assert!(v % 2 == 1, "v = {} should be odd", v);

        // Swap if necessary so u ≤ v
        if u > v {
            let s = u;
            u = v;
            v = s;
        }

        // Identity 4: gcd(u, v) = gcd(u, v-u) as u ≤ v and u, v are both odd
        v -= u;
        // v is now even

        if v == 0 {
            // Identity 1: gcd(u, 0) = u
            // The shift by k is necessary to add back the 2ᵏ factor that was removed before the loop
            return u << k;
        }

        // Identity 3: gcd(u, 2ʲ v) = gcd(u, v) as u is odd
        v >>= v.trailing_zeros();
    }
}
