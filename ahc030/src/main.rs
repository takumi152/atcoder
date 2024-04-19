#![allow(non_snake_case, unused_macros, unused_braces)]

#[allow(dead_code)]
pub mod grid {
    use std::{
        fmt,
        ops::{Add, AddAssign, Sub, SubAssign},
    };
    
    pub type PosType = i16;

    #[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Default, Hash)]
    pub struct Position {
        pub x: PosType,
        pub y: PosType,
    }

    impl Position {
        pub fn new(x: PosType, y: PosType) -> Self {
            Self { x, y }
        }

        pub fn manhattan_distance(&self, other: &Self) -> PosType {
            (self.x - other.x).abs() + (self.y - other.y).abs()
        }
    }

    #[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
    pub struct PositionDelta {
        pub dx: PosType,
        pub dy: PosType,
    }

    impl PositionDelta {
        pub fn new(dx: PosType, dy: PosType) -> Self {
            Self { dx, dy }
        }
    }

    impl Add<PositionDelta> for Position {
        type Output = Position;

        fn add(self, rhs: PositionDelta) -> Position {
            Position::new(self.x + rhs.dx, self.y + rhs.dy)
        }
    }

    impl AddAssign<PositionDelta> for Position {
        fn add_assign(&mut self, rhs: PositionDelta) {
            self.x += rhs.dx;
            self.y += rhs.dy;
        }
    }

    impl Sub<Position> for Position {
        type Output = PositionDelta;

        fn sub(self, rhs: Position) -> PositionDelta {
            PositionDelta::new(self.x - rhs.x, self.y - rhs.y)
        }
    }

    impl Sub<PositionDelta> for Position {
        type Output = Position;

        fn sub(self, rhs: PositionDelta) -> Position {
            Position::new(self.x - rhs.dx, self.y - rhs.dy)
        }
    }

    impl SubAssign<PositionDelta> for Position {
        fn sub_assign(&mut self, rhs: PositionDelta) {
            self.x -= rhs.dx;
            self.y -= rhs.dy;
        }
    }

    impl fmt::Display for Position {
        fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
            write!(f, "({}, {})", self.x, self.y)
        }
    }
}

#[allow(dead_code, unused_imports)]
mod solver {
    use std::{
        mem::swap,
        cell::{Cell, RefCell},
        io::{self, stderr, stdin, stdout, BufReader, Write},
        time::Instant,
    };
    
    use nalgebra::clamp;
    use proconio::{input, source::line::LineSource};
    use rand::{seq::SliceRandom, Rng};
    use rand_core::SeedableRng;
    use rand_pcg::Pcg64Mcg;

    use super::grid::{Position, PositionDelta, PosType};

    const OUTPUT_VIS_COMMENT: bool = true;
    const OUTPUT_DEBUG: bool = false;

    #[derive(Debug, Clone)]
    pub struct HyperParameter {
        pub seed: u64,
        pub temperature: f64,
        pub target_prob: f64,
        pub mcmc_mismatch_penalty: f64,
        pub confidence_lower_bound: f64,
        pub confidence_upper_bound: f64,
    }

    impl HyperParameter {
        pub fn new() -> Self {
            let args: Vec<String> = std::env::args().collect();
            let seed: u64 = if args.len() > 1 { args[1].parse().unwrap() } else { 42 };
            let temperature: f64 = if args.len() > 2 { args[2].parse().unwrap() } else { 80.20790351514765 };
            let target_prob: f64 = if args.len() > 3 { args[3].parse().unwrap() } else { 0.6368299426171158 };
            let mcmc_mismatch_penalty: f64 = if args.len() > 4 { args[4].parse().unwrap() } else { 249.32845632240088 };
            let confidence_lower_bound: f64 = if args.len() > 5 { args[5].parse().unwrap() } else { 0.04815655195851749 };
            let confidence_upper_bound: f64 = if args.len() > 6 { args[6].parse().unwrap() } else { 0.8577665172219888 };
            Self { seed, temperature, target_prob, mcmc_mismatch_penalty, confidence_lower_bound, confidence_upper_bound }
        }
    }

    #[derive(Debug, Clone)]
    pub struct Polyomino {
        pub size: i32,
        pub cells: Vec<Position>,
    }

    #[derive(Debug, Clone)]
    pub struct Input {
        pub board_size: i32,
        pub polyomino_num: i32,
        pub error_factor: f64,
        pub polyominos: Vec<Polyomino>,
    }

    #[derive(Debug, Clone)]
    pub struct Environment {
        pub board_size: i32,
        pub polyomino_num: i32,
        pub error_factor: f64,
        pub polyominos: Vec<Polyomino>,

        pub hyper_param: HyperParameter,
    }

    impl Environment {
        pub fn within_board(&self, pos: &Position) -> bool {
            pos.x >= 0 && pos.x < self.board_size as PosType && pos.y >= 0 && pos.y < self.board_size as PosType
        }
    }

    #[derive(Debug, Clone)]
    pub struct Query {
        pub cells: Vec<Position>,
    }

    pub fn do_query(query: &Query) -> i32 {
        // magic
        let mut stdin = LineSource::new(BufReader::new(io::stdin()));
        macro_rules! input(($($tt:tt)*) => (proconio::input!(from &mut stdin, $($tt)*)));
        
        assert!(query.cells.len() > 0);
        print!("q {}", query.cells.len());
        for cell in &query.cells {
            print!(" {} {}", cell.x, cell.y);
        }
        println!();
        stdout().flush().unwrap();
        input! {
            result: i32,
        }
        result
    }

    #[derive(Debug, Clone)]
    pub struct Answer {
        pub cells: Vec<Position>,
    }

    pub fn do_answer(answer: &Answer) -> i32 {
        // magic
        let mut stdin = LineSource::new(BufReader::new(io::stdin()));
        macro_rules! input(($($tt:tt)*) => (proconio::input!(from &mut stdin, $($tt)*)));

        assert!(answer.cells.len() > 0);
        print!("a {}", answer.cells.len());
        for cell in &answer.cells {
            print!(" {} {}", cell.x, cell.y);
        }
        println!();
        stdout().flush().unwrap();
        input! {
            result: i32,
        }
        result
    }

    #[derive(Debug, Clone)]
    pub struct Color {
        pub r: u8,
        pub g: u8,
        pub b: u8,
    }

    impl Color {
        pub fn from_rgb(r: u8, g: u8, b: u8) -> Self {
            Self { r, g, b }
        }
        pub fn from_hsv(h: f64, s: f64, v: f64) -> Self {
            assert!(0.0 <= h && h < 360.0);
            assert!(0.0 <= s && s <= 1.0);
            assert!(0.0 <= v && v <= 1.0);
            let c = v * s;
            let x = c * (1.0 - ((h / 60.0) % 2.0 - 1.0).abs());
            let m = v - c;
            let mut r = 0.0;
            let mut g = 0.0;
            let mut b = 0.0;
            if (0.0..60.0).contains(&h) {
                r = c;
                g = x;
            } else if (60.0..120.0).contains(&h) {
                r = x;
                g = c;
            } else if (120.0..180.0).contains(&h) {
                g = c;
                b = x;
            } else if (180.0..240.0).contains(&h) {
                g = x;
                b = c;
            } else if (240.0..300.0).contains(&h) {
                r = x;
                b = c;
            } else if (300.0..360.0).contains(&h) {
                r = c;
                b = x;
            }
            Self {
                r: ((r + m) * 255.0) as u8,
                g: ((g + m) * 255.0) as u8,
                b: ((b + m) * 255.0) as u8,
            }
        }
        pub fn to_color_code(&self) -> String {
            format!("#{:02X}{:02X}{:02X}", self.r, self.g, self.b)
        }
    }

    pub struct VisComment {
        pub pos: Position,
        pub color: Color,
    }

    impl VisComment {
        pub fn new(pos: Position, color: Color) -> Self {
            Self { pos, color }
        }
    }

    pub fn do_vis_comment(comment: &VisComment) {
        println!("#c {} {} {}", comment.pos.x, comment.pos.y, comment.color.to_color_code());
    }

    pub fn get_input() -> Input {
        // magic
        let mut stdin = LineSource::new(BufReader::new(io::stdin()));
        macro_rules! input(($($tt:tt)*) => (proconio::input!(from &mut stdin, $($tt)*)));
        
        let mut polyominos = Vec::new();
        input! {
            board_size: i32,
            polyomino_num: i32,
            error_factor: f64,
        }
        for _ in 0..polyomino_num {
            let mut cells = Vec::new();
            input! {
                size: i32,
            }
            for _ in 0..size {
                input! {
                    x: i32,
                    y: i32,
                }
                cells.push(Position::new(x as PosType, y as PosType));
            }
            polyominos.push(Polyomino { size, cells });
        }
        Input { board_size, polyomino_num, error_factor, polyominos }
    }

    pub fn init_env(input: &Input) -> Environment {
        Environment {
            board_size: input.board_size,
            polyomino_num: input.polyomino_num,
            error_factor: input.error_factor,
            polyominos: input.polyominos.clone(),
            hyper_param: HyperParameter::new(),
        }
    }

    pub fn solve_stupid(env: &Environment) {
        let mut cell_pos_with_polyomino = Vec::new();
        for x in 0..env.board_size {
            for y in 0..env.board_size {
                let res = do_query(&Query { cells: vec![Position::new(x as PosType, y as PosType)] });
                if res >= 1 {
                    cell_pos_with_polyomino.push(Position::new(x as PosType, y as PosType));
                }
            }
        }
        let ans = Answer { cells: cell_pos_with_polyomino };
        do_answer(&ans);
    }

    pub fn solve_deterministic(env: &Environment, rng: &mut Pcg64Mcg) {
        let mut digged = vec![vec![false; env.board_size as usize]; env.board_size as usize];
        let mut dig_results = vec![vec![0; env.board_size as usize]; env.board_size as usize];
        #[allow(while_true)]
        while true {
            let mut positive_prob = vec![vec![0.0; env.board_size as usize]; env.board_size as usize];
            for i in 0..env.board_size {
                for j in 0..env.board_size {
                    if digged[i as usize][j as usize] && dig_results[i as usize][j as usize] >= 1 {
                        positive_prob[i as usize][j as usize] = 1.0;
                    }
                }
            }
            for k in 0..env.polyomino_num {
                let mut total_possible_placement_num = 0;
                let mut possible_placement_num = vec![vec![0; env.board_size as usize]; env.board_size as usize];
                let polyomino = &env.polyominos[k as usize];
                for i in 0..env.board_size {
                    for j in 0..env.board_size {
                        let mut possible = true;
                        for cell in &polyomino.cells {
                            let x = i as PosType + cell.x;
                            let y = j as PosType + cell.y;
                            if !env.within_board(&Position::new(x, y)) || (digged[x as usize][y as usize] && dig_results[x as usize][y as usize] == 0) {
                                possible = false;
                                break;
                            }
                        }
                        if possible {
                            for cell in &polyomino.cells {
                                let x = i as PosType + cell.x;
                                let y = j as PosType + cell.y;
                                possible_placement_num[x as usize][y as usize] += 1;
                            }
                            total_possible_placement_num += 1;
                        }
                    }
                }
                for i in 0..env.board_size {
                    for j in 0..env.board_size {
                        let cell_placed_prob = possible_placement_num[i as usize][j as usize] as f64 / total_possible_placement_num as f64;
                        positive_prob[i as usize][j as usize] = positive_prob[i as usize][j as usize] + (1.0 - positive_prob[i as usize][j as usize]) * cell_placed_prob;
                    }
                }
            }

            // ビジュアライザ出力用
            if OUTPUT_VIS_COMMENT {
                for i in 0..env.board_size {
                    for j in 0..env.board_size {
                        let hue = if positive_prob[i as usize][j as usize] < 1.0 {120.0 * positive_prob[i as usize][j as usize]} else {180.0};
                        let sat = if positive_prob[i as usize][j as usize] > 0.0 {1.0} else {0.0};
                        let val = if positive_prob[i as usize][j as usize] > 0.0 {1.0} else {0.5};
                        let color = Color::from_hsv(hue, sat, val);
                        do_vis_comment(&VisComment::new(Position::new(i as PosType, j as PosType), color));
                    }
                }
            }

            // 確率が0より大きく1より小さいセルのうち、確率が一定値に最も近いセルを掘る
            let mut position_check_order = Vec::new();
            for i in 0..env.board_size {
                for j in 0..env.board_size {
                    position_check_order.push(Position::new(i as PosType, j as PosType));
                }
            }
            position_check_order.shuffle(rng);

            let target_prob = 0.5;
            let mut unconfirmed_cell_exists = false;
            let mut best_cell = Position::new(0, 0);
            let mut best_diff = 1.0;
            for pos in position_check_order {
                let i = pos.x;
                let j = pos.y;
                let prob = positive_prob[i as usize][j as usize];
                if digged[i as usize][j as usize] || !(0.0 < prob && prob < 1.0) {
                    continue;
                }
                let diff = (prob - target_prob).abs();
                if diff < best_diff {
                    unconfirmed_cell_exists = true;
                    best_diff = diff;
                    best_cell = Position::new(i as PosType, j as PosType);
                }
            }
            if unconfirmed_cell_exists {
                let res = do_query(&Query { cells: vec![best_cell] });
                digged[best_cell.x as usize][best_cell.y as usize] = true;
                dig_results[best_cell.x as usize][best_cell.y as usize] = res;
            } else {
                // 無ければ答えが確定している
                let mut ans = Vec::new();
                for i in 0..env.board_size {
                    for j in 0..env.board_size {
                        if positive_prob[i as usize][j as usize] > 0.5 {
                            ans.push(Position::new(i as PosType, j as PosType));
                        }
                    }
                }
                do_answer(&Answer { cells: ans });
                break;
            }
        }
    }

    pub fn solve_mcmc(env: &Environment, rng: &mut Pcg64Mcg) {
        #[derive(Debug, Clone)]
        pub struct MetropolisEnvironment {
            env: Environment,

            pub digged: Vec<Vec<bool>>,
            pub dig_results: Vec<Vec<i32>>,

            pub positive_prob: Vec<Vec<f64>>,
            pub can_place_polyomino: Vec<Vec<Vec<bool>>>, // polyonimo_id, x_of_top_left, y_of_top_left
            pub polyomino_placeable_positions: Vec<Vec<Position>>, // polyonimo_id, position
        }

        impl MetropolisEnvironment {
            pub fn new(env: &Environment) -> MetropolisEnvironment {
                let mut m_env = MetropolisEnvironment {
                    env: env.clone(),

                    digged: vec![vec![false; env.board_size as usize]; env.board_size as usize],
                    dig_results: vec![vec![0; env.board_size as usize]; env.board_size as usize],

                    positive_prob: vec![vec![0.0; env.board_size as usize]; env.board_size as usize],
                    can_place_polyomino: vec![vec![vec![false; env.board_size as usize]; env.board_size as usize]; env.polyomino_num as usize],
                    polyomino_placeable_positions: vec![Vec::new(); env.polyomino_num as usize],
                };
                m_env.update_environment();
                m_env
            }
            pub fn dig_position(&mut self, pos: &Position) {
                let res = do_query(&Query { cells: vec![*pos] });
                self.digged[pos.x as usize][pos.y as usize] = true;
                self.dig_results[pos.x as usize][pos.y as usize] = res;
                self.update_environment();
            }
            fn update_environment(&mut self) {
                self.update_positive_prob();
                self.update_can_place_polyomino();
                self.update_polyomino_placeable_positions();
            }
            fn update_positive_prob(&mut self) {
                for i in 0..self.env.board_size {
                    for j in 0..self.env.board_size {
                        self.positive_prob[i as usize][j as usize] = if self.digged[i as usize][j as usize] && self.dig_results[i as usize][j as usize] >= 1 {
                            1.0
                        } else {
                            0.0
                        };
                    }
                }
                for k in 0..self.env.polyomino_num {
                    let mut total_possible_placement_num = 0;
                    let mut possible_placement_num = vec![vec![0; self.env.board_size as usize]; self.env.board_size as usize];
                    let polyomino = &self.env.polyominos[k as usize];
                    for i in 0..self.env.board_size {
                        for j in 0..self.env.board_size {
                            let mut possible = true;
                            for cell in &polyomino.cells {
                                let x = i as PosType + cell.x;
                                let y = j as PosType + cell.y;
                                if !self.env.within_board(&Position::new(x, y)) || (self.digged[x as usize][y as usize] && self.dig_results[x as usize][y as usize] == 0) {
                                    possible = false;
                                    break;
                                }
                            }
                            if possible {
                                for cell in &polyomino.cells {
                                    let x = i as PosType + cell.x;
                                    let y = j as PosType + cell.y;
                                    possible_placement_num[x as usize][y as usize] += 1;
                                }
                                total_possible_placement_num += 1;
                            }
                        }
                    }
                    for i in 0..self.env.board_size {
                        for j in 0..self.env.board_size {
                            let cell_placed_prob = possible_placement_num[i as usize][j as usize] as f64 / total_possible_placement_num as f64;
                            self.positive_prob[i as usize][j as usize] = self.positive_prob[i as usize][j as usize] + (1.0 - self.positive_prob[i as usize][j as usize]) * cell_placed_prob;
                        }
                    }
                }
            }
            fn update_can_place_polyomino(&mut self) {
                for k in 0..self.env.polyomino_num {
                    let polyomino = &self.env.polyominos[k as usize];
                    for i in 0..self.env.board_size {
                        for j in 0..self.env.board_size {
                            let mut possible = true;
                            for cell in &polyomino.cells {
                                let x = i as PosType + cell.x;
                                let y = j as PosType + cell.y;
                                if !self.env.within_board(&Position::new(x, y)) || (self.digged[x as usize][y as usize] && self.dig_results[x as usize][y as usize] == 0) {
                                    possible = false;
                                    break;
                                }
                            }
                            self.can_place_polyomino[k as usize][i as usize][j as usize] = possible;
                        }
                    }
                }
            }
            fn update_polyomino_placeable_positions(&mut self) {
                for k in 0..self.env.polyomino_num {
                    self.polyomino_placeable_positions[k as usize].clear();
                    for i in 0..self.env.board_size {
                        for j in 0..self.env.board_size {
                            if self.can_place_polyomino[k as usize][i as usize][j as usize] {
                                self.polyomino_placeable_positions[k as usize].push(Position::new(i as PosType, j as PosType));
                            }
                        }
                    }
                }
            }
        }

        #[derive(Debug, Clone)]
        pub struct MetropolisState {
            m_env: MetropolisEnvironment,

            pub polyomino_positions: Vec<Position>,

            placed_cell_num: Vec<Vec<i32>>,
            cell_score: Vec<Vec<f64>>,
            total_cell_score: f64,
        }

        impl MetropolisState {
            pub fn new(m_env: MetropolisEnvironment) -> MetropolisState {
                let mut m_state = MetropolisState {
                    m_env: m_env.clone(),

                    polyomino_positions: vec![Position::new(0, 0); m_env.env.polyomino_num as usize],

                    placed_cell_num: vec![vec![0; m_env.env.board_size as usize]; m_env.env.board_size as usize],
                    cell_score: vec![vec![0.0; m_env.env.board_size as usize]; m_env.env.board_size as usize],
                    total_cell_score: 0.0,
                };
                m_state.update_state_full();
                m_state
            }
            pub fn update_state_full(&mut self) {
                for i in 0..self.m_env.env.board_size {
                    for j in 0..self.m_env.env.board_size {
                        self.placed_cell_num[i as usize][j as usize] = 0;
                    }
                }
                for k in 0..self.m_env.env.polyomino_num {
                    let polyomino = &self.m_env.env.polyominos[k as usize];
                    let pos = self.polyomino_positions[k as usize];
                    for cell in &polyomino.cells {
                        let x = pos.x + cell.x;
                        let y = pos.y + cell.y;
                        assert!(self.m_env.env.within_board(&Position::new(x, y)));
                        self.placed_cell_num[x as usize][y as usize] += 1;
                    }
                }
                for i in 0..self.m_env.env.board_size {
                    for j in 0..self.m_env.env.board_size {
                        self.cell_score[i as usize][j as usize] = self.calculate_cell_score(&Position::new(i as PosType, j as PosType));
                    }
                }
                self.total_cell_score = 0.0;
                for i in 0..self.m_env.env.board_size {
                    for j in 0..self.m_env.env.board_size {
                        self.total_cell_score += self.cell_score[i as usize][j as usize];
                    }
                }
            }
            fn calculate_cell_score(&self, pos: &Position) -> f64 {
                let placed_num = self.placed_cell_num[pos.x as usize][pos.y as usize];
                if self.m_env.digged[pos.x as usize][pos.y as usize] {
                    let dig_result = self.m_env.dig_results[pos.x as usize][pos.y as usize];
                    return ((dig_result - placed_num).abs()) as f64 * self.m_env.env.hyper_param.mcmc_mismatch_penalty;
                }
                let positive_prob = self.m_env.positive_prob[pos.x as usize][pos.y as usize];
                let score = (positive_prob - clamp(placed_num, 0, 1) as f64).abs();
                score
            }
            pub fn update_score_partial(&mut self, polyomino_id: usize, new_pos: &Position) {
                let old_pos = self.polyomino_positions[polyomino_id];
                let polyomino = &self.m_env.env.polyominos[polyomino_id];
                for cell in &polyomino.cells {
                    let x = old_pos.x + cell.x;
                    let y = old_pos.y + cell.y;
                    assert!(self.m_env.env.within_board(&Position::new(x, y)));
                    assert!(self.placed_cell_num[x as usize][y as usize] > 0);
                    self.placed_cell_num[x as usize][y as usize] -= 1;
                    self.total_cell_score -= self.cell_score[x as usize][y as usize];
                    self.cell_score[x as usize][y as usize] = self.calculate_cell_score(&Position::new(x, y));
                    self.total_cell_score += self.cell_score[x as usize][y as usize];
                }
                self.polyomino_positions[polyomino_id] = new_pos.clone();
                for cell in &polyomino.cells {
                    let x = new_pos.x + cell.x;
                    let y = new_pos.y + cell.y;
                    assert!(self.m_env.env.within_board(&Position::new(x, y)));
                    self.placed_cell_num[x as usize][y as usize] += 1;
                    self.total_cell_score -= self.cell_score[x as usize][y as usize];
                    self.cell_score[x as usize][y as usize] = self.calculate_cell_score(&Position::new(x, y));
                    self.total_cell_score += self.cell_score[x as usize][y as usize];
                }
            }
        }

        let m_env = MetropolisEnvironment::new(env);
        let mut m_state = MetropolisState::new(m_env);
        let mut turn = 0;
        let turn_limit = 2 * env.board_size * env.board_size;
        #[allow(while_true)]
        while true {
            let mut sampled_polyomino_position_count = vec![vec![vec![0; env.board_size as usize]; env.board_size as usize]; env.polyomino_num as usize];

            let temperature: f64 = env.hyper_param.temperature;

            const BURN_IN_ITER: usize = 2048;
            const SAMPLE_NUM: usize = 1024;
            const SAMPLE_INTERVAL: usize = 8;
            const MAX_ITER: usize = BURN_IN_ITER + SAMPLE_NUM * SAMPLE_INTERVAL;
            {
                let time_begin = Instant::now();
                let mut score = m_state.total_cell_score;
                let mut iter_count = 0;
                while iter_count < MAX_ITER {
                    let roll = rng.gen_range(0.0..1.0);
                    if roll < 0.5 {
                        let p = rng.gen_range(0..env.polyomino_num as usize);
                        let new_pos = m_state.m_env.polyomino_placeable_positions[p].choose(rng).unwrap().clone();

                        let old_pos = m_state.polyomino_positions[p];

                        m_state.update_score_partial(p, &new_pos);

                        let new_score = m_state.total_cell_score;
                        if new_score < score || rng.gen_bool(((score - new_score) / temperature).exp()) {
                            score = new_score;
                        } else {
                            m_state.update_score_partial(p, &old_pos);
                        }
                    } else if roll < 1.0 {
                        let p1 = rng.gen_range(0..env.polyomino_num as usize);
                        let p2 = rng.gen_range(0..env.polyomino_num as usize);
                        let new_pos1 = m_state.m_env.polyomino_placeable_positions[p1].choose(rng).unwrap().clone();
                        let new_pos2 = m_state.m_env.polyomino_placeable_positions[p2].choose(rng).unwrap().clone();

                        let old_pos1 = m_state.polyomino_positions[p1];
                        let old_pos2 = m_state.polyomino_positions[p2];

                        m_state.update_score_partial(p1, &new_pos1);
                        m_state.update_score_partial(p2, &new_pos2);

                        let new_score = m_state.total_cell_score;
                        if new_score < score || rng.gen_bool(((score - new_score) / temperature).exp()) {
                            score = new_score;
                        } else {
                            m_state.update_score_partial(p1, &old_pos1);
                            m_state.update_score_partial(p2, &old_pos2);
                        }
                    }

                    iter_count += 1;
                    if iter_count >= BURN_IN_ITER && (iter_count - BURN_IN_ITER) % SAMPLE_INTERVAL == 0 {
                        for k in 0..env.polyomino_num {
                            let pos = &m_state.polyomino_positions[k as usize];
                            sampled_polyomino_position_count[k as usize][pos.x as usize][pos.y as usize] += 1;
                        }
                    }
                }

                let time_end = Instant::now();
                if OUTPUT_DEBUG {
                    eprintln!("turn: {:>3}, time: {:>6} us, score: {}", turn, (time_end - time_begin).as_micros(), score);
                }
            }

            let mut sampled_cell_positive_prob = vec![vec![0.0; env.board_size as usize]; env.board_size as usize];
            for k in 0..env.polyomino_num {
                let mut sampled_cell_position_count = vec![vec![0; env.board_size as usize]; env.board_size as usize];
                let polyomino = &env.polyominos[k as usize];
                for i in 0..env.board_size {
                    for j in 0..env.board_size {
                        if sampled_polyomino_position_count[k as usize][i as usize][j as usize] > 0 {
                            for cell in &polyomino.cells {
                                let x = i as PosType + cell.x;
                                let y = j as PosType + cell.y;
                                assert!(env.within_board(&Position::new(x, y)));
                                sampled_cell_position_count[x as usize][y as usize] += sampled_polyomino_position_count[k as usize][i as usize][j as usize];
                            }
                        }
                    }
                }
                for i in 0..env.board_size {
                    for j in 0..env.board_size {
                        sampled_cell_positive_prob[i as usize][j as usize] = 1.0 - (1.0 - sampled_cell_positive_prob[i as usize][j as usize]) * (1.0 - (sampled_cell_position_count[i as usize][j as usize] as f64 / SAMPLE_NUM as f64));
                    }
                }
            }

            // ビジュアライザ出力用
            if OUTPUT_VIS_COMMENT {
                for i in 0..env.board_size {
                    for j in 0..env.board_size {
                        let prob = sampled_cell_positive_prob[i as usize][j as usize];
                        let hue = if prob < 1.0 {120.0 * prob} else {180.0};
                        let sat = if prob > 0.0 {1.0} else {0.0};
                        let val = if prob > 0.0 {1.0} else {0.5};
                        let color = Color::from_hsv(hue, sat, val);
                        do_vis_comment(&VisComment::new(Position::new(i as PosType, j as PosType), color));
                    }
                }
            }

            // 確率が0より大きく1より小さいセルのうち、確率が一定値に最も近いセルを掘る
            let mut position_check_order = Vec::new();
            for i in 0..env.board_size {
                for j in 0..env.board_size {
                    position_check_order.push(Position::new(i as PosType, j as PosType));
                }
            }
            position_check_order.shuffle(rng);

            let target_prob = env.hyper_param.target_prob;
            let mut unconfirmed_cell_exists = false;
            let mut best_cell = Position::new(0, 0);
            let mut best_diff = 1.0;
            for pos in position_check_order {
                let i = pos.x;
                let j = pos.y;
                let prob = sampled_cell_positive_prob[i as usize][j as usize];
                if m_state.m_env.digged[i as usize][j as usize] || !(env.hyper_param.confidence_lower_bound < prob && prob < env.hyper_param.confidence_upper_bound) {
                    continue;
                }
                let diff = (prob - target_prob).abs();
                if diff < best_diff {
                    unconfirmed_cell_exists = true;
                    best_diff = diff;
                    best_cell = Position::new(i as PosType, j as PosType);
                }
            }
            if unconfirmed_cell_exists {
                if OUTPUT_DEBUG {
                    eprintln!("turn: {:>3}, dig_pos: ({}, {}), prob: {}", turn, best_cell.x, best_cell.y, m_state.m_env.positive_prob[best_cell.x as usize][best_cell.y as usize]);
                }
                m_state.m_env.dig_position(&best_cell);
                m_state.update_state_full();
            } else {
                // 無ければ(おそらく)答えが確定している
                let mut ans = Vec::new();
                for i in 0..env.board_size {
                    for j in 0..env.board_size {
                        let prob = sampled_cell_positive_prob[i as usize][j as usize];
                        if prob > 0.5 {
                            ans.push(Position::new(i as PosType, j as PosType));
                        }
                    }
                }
                let res = do_answer(&Answer { cells: ans.clone() });
                if res == 1 {
                    break;
                }
                else {
                    // 答えとして出力したセルのうち、まだ掘られていないセルを掘る
                    if turn < turn_limit - 2 {
                        for cell in &ans {
                            if m_state.m_env.digged[cell.x as usize][cell.y as usize] {
                                continue;
                            }
                            turn += 1;
                            if OUTPUT_DEBUG {
                                eprintln!("turn: {:>3}, dig_pos: ({}, {})", turn, cell.x, cell.y);
                            }
                            m_state.m_env.dig_position(cell);
                            m_state.update_state_full();
                            break;
                        }
                    }
                }
            }

            turn += 1;
            if turn >= turn_limit {
                break;
            }
        }
    }

    #[allow(unused_variables)]
    pub fn solve_bayesian(env: Environment, rng: &mut Pcg64Mcg) {
        #[derive(Debug, Clone)]
        pub struct BayesianEnvironment {
            env: Environment,

            pub digged: Vec<Vec<bool>>,
            pub dig_results: Vec<Vec<i32>>,

            pub can_place_polyomino: Vec<Vec<Vec<bool>>>, // polyonimo_id, x_of_top_left, y_of_top_left
            pub polyomino_placeable_positions: Vec<Vec<Position>>, // polyonimo_id, position
        }

        impl BayesianEnvironment {
            pub fn new(env: &Environment) -> BayesianEnvironment {
                let mut m_env = BayesianEnvironment {
                    env: env.clone(),

                    digged: vec![vec![false; env.board_size as usize]; env.board_size as usize],
                    dig_results: vec![vec![0; env.board_size as usize]; env.board_size as usize],

                    can_place_polyomino: vec![vec![vec![false; env.board_size as usize]; env.board_size as usize]; env.polyomino_num as usize],
                    polyomino_placeable_positions: vec![Vec::new(); env.polyomino_num as usize],
                };
                m_env.update_environment();
                m_env
            }
            pub fn dig_position(&mut self, pos: &Position) {
                let res = do_query(&Query { cells: vec![*pos] });
                self.digged[pos.x as usize][pos.y as usize] = true;
                self.dig_results[pos.x as usize][pos.y as usize] = res;
                self.update_environment();
            }
            fn update_environment(&mut self) {
                self.update_can_place_polyomino();
                self.update_polyomino_placeable_positions();
            }
            fn update_can_place_polyomino(&mut self) {
                for k in 0..self.env.polyomino_num {
                    let polyomino = &self.env.polyominos[k as usize];
                    for i in 0..self.env.board_size {
                        for j in 0..self.env.board_size {
                            let mut possible = true;
                            for cell in &polyomino.cells {
                                let x = i as PosType + cell.x;
                                let y = j as PosType + cell.y;
                                if !self.env.within_board(&Position::new(x, y)) || (self.digged[x as usize][y as usize] && self.dig_results[x as usize][y as usize] == 0) {
                                    possible = false;
                                    break;
                                }
                            }
                            self.can_place_polyomino[k as usize][i as usize][j as usize] = possible;
                        }
                    }
                }
            }
            fn update_polyomino_placeable_positions(&mut self) {
                for k in 0..self.env.polyomino_num {
                    self.polyomino_placeable_positions[k as usize].clear();
                    for i in 0..self.env.board_size {
                        for j in 0..self.env.board_size {
                            if self.can_place_polyomino[k as usize][i as usize][j as usize] {
                                self.polyomino_placeable_positions[k as usize].push(Position::new(i as PosType, j as PosType));
                            }
                        }
                    }
                }
            }
        }

        #[derive(Debug, Clone)]
        pub struct QueryHistory {
            pub cells: Vec<Position>,
            pub result: i32,
        }

        #[derive(Debug, Clone)]
        pub struct BayesianState {
            b_env: BayesianEnvironment,

            pub cell_placed_num_prob: Vec<Vec<Vec<f64>>>, // x, y, probability_of_placed_num

            pub query_history: Vec<QueryHistory>,
        }

        impl BayesianState {
            pub fn new(b_env: BayesianEnvironment) -> BayesianState {
                let mut b_state = BayesianState {
                    b_env: b_env.clone(),

                    cell_placed_num_prob: vec![vec![vec![0.0; (b_env.env.polyomino_num + 1) as usize]; b_env.env.board_size as usize]; b_env.env.board_size as usize],

                    query_history: Vec::new(),
                };
                b_state.update_state_full();
                b_state
            }
            pub fn update_state_full(&mut self) {
                for i in 0..self.b_env.env.board_size {
                    for j in 0..self.b_env.env.board_size {
                        for k in 0..(self.b_env.env.polyomino_num + 1) {
                            self.cell_placed_num_prob[i as usize][j as usize][k as usize] = if self.b_env.digged[i as usize][j as usize] && self.b_env.dig_results[i as usize][j as usize] == k {
                                1.0
                            } else if k == 0 {
                                1.0
                            } else {
                                0.0
                            };
                        }
                    }
                }
                for k in 0..self.b_env.env.polyomino_num {
                    let polyomino = &self.b_env.env.polyominos[k as usize];
                    let mut total_possible_placement_num = 0;
                    let mut possible_placement_num = vec![vec![0; self.b_env.env.board_size as usize]; self.b_env.env.board_size as usize];
                    for i in 0..self.b_env.env.board_size {
                        for j in 0..self.b_env.env.board_size {
                            if self.b_env.can_place_polyomino[k as usize][i as usize][j as usize] {
                                for cell in &polyomino.cells {
                                    let x = i as PosType + cell.x;
                                    let y = j as PosType + cell.y;
                                    possible_placement_num[x as usize][y as usize] += 1;
                                }
                                total_possible_placement_num += 1;
                            }
                        }
                    }
                    assert!(total_possible_placement_num > 0);
                    for i in 0..self.b_env.env.board_size {
                        for j in 0..self.b_env.env.board_size {
                            if self.b_env.digged[i as usize][j as usize] || possible_placement_num[i as usize][j as usize] == 0 {
                                continue;
                            }
                            let cell_placed_prob = possible_placement_num[i as usize][j as usize] as f64 / total_possible_placement_num as f64;
                            for l in (0..self.b_env.env.polyomino_num).rev() {
                                let p0 = self.cell_placed_num_prob[i as usize][j as usize][l as usize];
                                let p1 = self.cell_placed_num_prob[i as usize][j as usize][(l + 1) as usize];
                                self.cell_placed_num_prob[i as usize][j as usize][l as usize] = p0 * (1.0 - cell_placed_prob);
                                self.cell_placed_num_prob[i as usize][j as usize][(l + 1) as usize] = p1 + p0 * cell_placed_prob;
                            }
                        }
                    }
                }
            }
        }
    }

    pub fn solve(env: &Environment) {
        let seed = env.hyper_param.seed;
        let mut rng = Pcg64Mcg::seed_from_u64(seed);
        // solve_stupid(env);
        // solve_deterministic(env, &mut rng);
        solve_mcmc(env, &mut rng);
    }

    pub fn run() {
        let input = get_input();
        let environment = init_env(&input);
        solve(&environment);
    }
}

fn main() {
    solver::run();
}
