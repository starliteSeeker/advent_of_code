#[derive(Debug)]
pub struct Day5 {
    id_ranges: Vec<(u64, u64)>,
    ids: Vec<u64>,
}

impl Day5 {
    pub fn with_file(file_name: &str) -> Day5 {
        let content = std::fs::read_to_string(file_name).unwrap();
        let mut lines = content.lines();

        let range_lines = lines.by_ref().take_while(|s| !s.is_empty());

        Day5 {
            id_ranges: range_lines
                .map(|s| {
                    let (lo, hi): (&str, &str) = s.split_once('-').unwrap();
                    (lo.parse().unwrap(), hi.parse().unwrap())
                })
                .collect(),
            ids: lines.map(|s| s.parse().unwrap()).collect(),
        }
    }

    fn range_cleanup(&mut self) {
        let mut new_ranges = Vec::new();

        if self.id_ranges.iter().any(|(lo, hi)| lo > hi) {
            println!("huh");
        }

        self.id_ranges.sort();

        let (mut lo, mut hi) = self.id_ranges[0];

        for (old_lo, old_hi) in &self.id_ranges {
            if hi < *old_lo - 1 {
                new_ranges.push((lo, hi));
                (lo, hi) = (*old_lo, *old_hi)
            } else if hi < *old_hi {
                hi = *old_hi;
            }
        }

        new_ranges.push((lo, hi));
        self.id_ranges = new_ranges;
    }

    pub fn part1(&self) {
        let mut count = 0;
        for id in &self.ids {
            if self.id_ranges.iter().any(|(lo, hi)| lo <= id && id <= hi) {
                count += 1;
            }
        }

        println!("day 5 part 1: {count}");
    }

    pub fn part2(&mut self) {
        self.range_cleanup();
        let result: u64 = self.id_ranges.iter().map(|(lo, hi)| hi - lo + 1).sum();

        println!("day 5 part 2: {result}");
    }
}
