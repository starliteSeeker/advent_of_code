mod day5;
mod day6;

fn main() {
    let mut d5 = day5::Day5::with_file("input/day5.txt");
    d5.part1();
    d5.part2();

    let mut d6 = day6::Day6::with_file("input/day6_1.txt");
    println!("{d6:?}");
    // d5.part1();
    // d5.part2();
}
