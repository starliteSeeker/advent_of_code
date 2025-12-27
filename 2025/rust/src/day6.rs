#[derive(Clone, Debug)]
enum Op {
    Add,
    Mul,
}

#[derive(Debug)]
pub struct Day6 {
    numbers: Vec<Vec<u64>>,
    numbers_2: Vec<Vec<u64>>,
    ops: Vec<Op>,
}

impl Day6 {
    pub fn with_file(file_name: &str) -> Day6 {
        let contents = std::fs::read_to_string(file_name).unwrap();
        let mut lines = contents.lines();

        let ops_line = lines.next_back().unwrap();
        let width = ops_line.len();

        let ops = ops_line
            .split_whitespace()
            .map(|c| if c == "+" { Op::Add } else { Op::Mul })
            .collect();

        let numbers = lines
            .clone()
            .map(|l| l.split_whitespace().map(|n| n.parse().unwrap()).collect())
            .collect();

        let mut numbers_2 = Vec::new();
        let numbers_lines: Vec<Vec<_>> = lines.map(|l| l.bytes().collect()).collect();
        let mut curr_group = Vec::new();
        for i in 0..width {
            let mut empty_column = true;
            let mut curr: u64 = 0;

            for l in &numbers_lines {
                if l[i] == b' ' {
                    continue;
                }
                empty_column = false;

                curr = curr * 10 + (l[i] - b'0') as u64;
            }

            if empty_column {
                numbers_2.push(curr_group.clone());
                curr_group.clear();
            } else {
                curr_group.push(curr);
            }
        }
        numbers_2.push(curr_group.clone());

        Day6 {
            ops,
            numbers,
            numbers_2,
        }
    }

    pub fn part1(&self) {
        let mut result: u64 = 0;
        for i in 0..self.ops.len() {
            let ns = self.numbers.iter().map(|v| v[i]);
            result += match self.ops[i] {
                Op::Add => ns.sum::<u64>(),
                Op::Mul => ns.product(),
            }
        }

        println!("day 6 part 1: {result}");
    }

    pub fn part2(&self) {
        let mut result: u64 = 0;

        for (nums, op) in std::iter::zip(self.numbers_2.clone(), self.ops.clone()) {
            result += match op {
                Op::Add => nums.iter().sum::<u64>(),
                Op::Mul => nums.iter().product(),
            }
        }
        println!("day 6 part 2: {result}");
    }
}
