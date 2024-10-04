use std::borrow::BorrowMut;
use orbclient::{Color, EventOption, Renderer, Window};
use num::complex::Complex;
fn main() {
    //set up window
    let (width, height) = orbclient::get_display_size().unwrap();
    let mut window = Window::new(
        (width as i32) / 4,
        (height as i32) / 4,
        1000,
        1000,
        "Charlie's Rust Mandel"
    ).unwrap();

    //power used in mandelbrot eq
    let mut pow = 2.0;

    //draw here
    draw(window.borrow_mut(),pow);

    //update window
    window.sync();

    'events: loop {
        for event in window.events() {
            match event.to_option() {
                // top left/right X (exit) button
                EventOption::Quit(_quit_event) => break 'events,
                // Process button events
                EventOption::Button(button_event) => {
                    if button_event.left {
                        pow += 0.25;
                        draw(window.borrow_mut(), pow);
                        window.sync();
                    }
                }
                //print all events
                event_option => println!("{:?}", event_option),
            }
        }
    }
}

// x, y coordinates in -> color indication (how long to diverge) out
fn mandelbrot(x: f32, y: f32, pow: f32) -> f32 {
    //create complex numbers
    let mut z: Complex<f32> = Complex::new(0.0, 0.0);
    let c: Complex<f32> = Complex::new(x, y);

    let ittr = 512;
    let mut i = 0;

    // find number of itterations for the dist/norm of copmlex z to diverge greater than 32
    while i < ittr && z.norm() < 32.0 {
        let z2 = z.powf(pow) ;
        z = z2 + c;
        i += 1;
    }

    //number of itterations / our substitute for infinity (divergence) will be the color
    //better description of this funny math at https://rotgers.io/posts/mandelbrot-rust/
    return ((i as f32) - z.norm().log2().log2()) / (ittr as f32);
}

fn draw(window: &mut Window, pow: f32) {
    //window is 1k by 1k but mandelbrot graph is only interesting from like -2 to 2 in either direction
    //most of the funny math in here is to accomidate for this scaling but using rusts itterators makes it more compact at least
    for x in (-500..500).map(|x| x as f32 * 0.004) {
        for y in (-500..500).map(|y| y as f32 * 0.004) {
            let m = mandelbrot(x, y, pow);
            let color: Color = Color::rgb(color(m)[0], color(m)[1], color(m)[2]);
            window.pixel((x * 250.0) as i32 + 500, (y * 250.0) as i32 + 500, color);

        }
    }
}

//color function ripped from https://rotgers.io/posts/mandelbrot-rust/
fn color(m: f32) -> [u8; 3] {
    let a = (0.5, 0.5, 0.5);
    let b = (0.5, 0.5, 0.5);
    let c = (1.0, 1.0, 1.0);
    let d = (0.0, 0.10, 0.20);
    let r = b.0 * (6.28318 * (c.0 * m + d.0)).cos() + a.0;
    let g = b.1 * (6.28318 * (c.1 * m + d.1)).cos() + a.1;
    let b = b.2 * (6.28318 * (c.2 * m + d.2)).cos() + a.2;
    [(255.0 * r) as u8, (255.0 * g) as u8, (255.0 * b) as u8]

}