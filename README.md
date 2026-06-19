# Tetris — LandTiger LPC1768

An implementation of the classic **Tetris** game running on the **LandTiger LPC1768** board
(ARM Cortex-M3 microcontroller), with graphics rendered on a **GLCD** display and input from
a **joystick** and **push buttons (EINT)**.

Developed in C using **Keil µVision**, starting from the PowerMCU/AVRman GLCD/Touch Panel sample.

> Modified by **Matteo Zylfo** — v2.0

---

## Controls

| Input | Action |
|-------|--------|
| **KEY1** | Start / Pause / New game (after Game Over) |
| **KEY2** | Hard drop (instant piece drop) |
| **Joystick ◄ / ►** | Move the piece left / right |
| **Joystick ▲** | Rotate the piece |
| **Joystick ▼** | Soft drop (speed up the fall) |

---

## Architecture

The project follows the **lightweight ISR + main loop** pattern, typical of embedded systems:

- The **ISRs** (RIT, Timer0, EINT) never run game logic or rendering: they simply set
  **`volatile` flags** (`gravity_event`, `move_l_event`, `rotate_event`, ...).
- The **main loop** sleeps on `__ASM("wfi")` (Wait For Interrupt) for power saving and,
  on each wake-up, calls `Tetris_Process_Events()`, which reads the flags and runs the
  game logic and GLCD rendering.

This keeps interrupts extremely short, while all the heavy work (LCD drawing, gravity,
line clearing) happens outside the interrupt context.

### Timers and interrupts

| Peripheral | Period | Role |
|------------|--------|------|
| **Timer0** | 1 s (soft drop: ~0.5 s) | Gravity: raises `gravity_event`. `MR0` is changed at runtime for the soft drop |
| **RIT** | 50 ms | Joystick polling + button debouncing |
| **EINT1 / EINT2** | — | KEY1 / KEY2 press (debouncing handled via RIT) |

---

## Game model

- **Board**: a `20 × 10` matrix (`matrix_game` for occupancy, `matrix_color` for colors).
- **Tetrominoes**: constant array `tetromino[7][4][4][4]` → 7 pieces (I, O, T, J, L, S, Z),
  4 rotations each, on a `4 × 4` grid.
- **Collisions**: all centralized in `can_move(row, col, rot)`, used for movement,
  rotation and falling.
- **Game over**: detected when a newly spawned piece cannot fit into the board.

### Scoring

| Event | Points |
|-------|--------|
| Piece placed | +10 |
| 1 / 2 / 3 lines | +100 per line |
| 4 lines (*Tetris!*) | +600 |

The high score (`TOP`) is updated automatically at the end of each game.

---

## Project structure

```
Source/
├── sample.c              # main(): peripheral init + main loop
├── tetris/
│   ├── tetris.c          # game logic, rendering, event handling
│   └── tetris.h          # prototypes and global variables (extern)
├── RIT/                  # Repetitive Interrupt Timer (input polling)
├── timer/                # Timer0/2/3 (gravity)
├── button_EXINT/         # button handling via external interrupts
├── joystick/             # joystick init
├── led/                  # LED handling
├── GLCD/                 # graphic display driver
├── TouchPanel/           # touch driver (not used in the game)
├── adc/                  # ADC driver
├── Map/                  # helper functions
└── CMSIS_core/           # CMSIS Cortex-M3 headers
```

---

## Build & Run

1. Open the project in **Keil µVision** (`sample.uvprojx`).
2. Build the project (**F7**).
3. Flash it onto the LandTiger LPC1768 board, or run it in the **simulator**
   (the `SIMULATOR` macro in `sample.c` is already defined).

---

## Author

**Matteo Zylfo** — Politecnico di Torino

Based on the original GLCD/Touch Panel sample by PowerMCU / AVRman.
