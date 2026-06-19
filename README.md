# Tetris — LandTiger LPC1768

Implementazione del classico **Tetris** su scheda **LandTiger LPC1768** (microcontrollore ARM Cortex-M3),
con output grafico su display **GLCD** e input da **joystick** e **pulsanti (EINT)**.

Progetto sviluppato in C in ambiente **Keil µVision** a partire dal sample GLCD/Touch Panel di PowerMCU/AVRman.

> Modificato da **Matteo Zylfo** — v2.0

---

## Comandi

| Input | Azione |
|-------|--------|
| **KEY1** | Start / Pausa / Nuova partita (dopo Game Over) |
| **KEY2** | Hard drop (caduta istantanea del pezzo) |
| **Joystick ◄ / ►** | Sposta il pezzo a sinistra / destra |
| **Joystick ▲** | Ruota il pezzo |
| **Joystick ▼** | Soft drop (accelera la caduta) |

---

## Architettura

Il progetto segue il pattern **ISR leggere + main loop**, tipico dei sistemi embedded:

- Le **ISR** (RIT, Timer0, EINT) non eseguono mai logica di gioco né disegno: si limitano a
  impostare dei **flag `volatile`** (`gravity_event`, `move_l_event`, `rotate_event`, ...).
- Il **main loop** dorme su `__ASM("wfi")` (Wait For Interrupt) per il risparmio energetico
  e, ad ogni risveglio, chiama `Tetris_Process_Events()` che legge i flag ed esegue la logica
  e il rendering sul GLCD.

In questo modo gli interrupt restano cortissimi e tutto il lavoro pesante (disegno su LCD,
gravità, eliminazione linee) avviene fuori dal contesto di interrupt.

### Timer e interrupt

| Periferica | Periodo | Ruolo |
|------------|---------|-------|
| **Timer0** | 1 s (soft drop: ~0,5 s) | Gravità: genera `gravity_event`. `MR0` viene cambiato a runtime per il soft drop |
| **RIT** | 50 ms | Polling del joystick + debouncing dei pulsanti |
| **EINT1 / EINT2** | — | Pressione di KEY1 / KEY2 (con debouncing gestito via RIT) |

---

## Modello di gioco

- **Board**: matrice `20 × 10` (`matrix_game` per occupazione, `matrix_color` per i colori).
- **Tetromini**: array costante `tetromino[7][4][4][4]` → 7 pezzi (I, O, T, J, L, S, Z),
  4 rotazioni ciascuno, su griglia `4 × 4`.
- **Collisioni**: tutte centralizzate in `can_move(row, col, rot)`, usata per movimento,
  rotazione e caduta.
- **Game over**: rilevato quando un nuovo pezzo generato non riesce a entrare nella board.

### Punteggio

| Evento | Punti |
|--------|-------|
| Pezzo piazzato | +10 |
| 1 / 2 / 3 linee | +100 per linea |
| 4 linee (*Tetris!*) | +600 |

Il record (`TOP`) viene aggiornato automaticamente a fine partita.

---

## Struttura del progetto

```
Source/
├── sample.c              # main(): init periferiche + main loop
├── tetris/
│   ├── tetris.c          # logica di gioco, rendering, gestione eventi
│   └── tetris.h          # prototipi e variabili globali (extern)
├── RIT/                  # Repetitive Interrupt Timer (polling input)
├── timer/                # Timer0/2/3 (gravità)
├── button_EXINT/         # gestione pulsanti via interrupt esterni
├── joystick/             # init joystick
├── led/                  # gestione LED
├── GLCD/                 # driver display grafico
├── TouchPanel/           # driver touch (non usato nel gioco)
├── adc/                  # driver ADC
├── Map/                  # funzioni di supporto
└── CMSIS_core/           # header CMSIS Cortex-M3
```

---

## Build & Run

1. Aprire il progetto in **Keil µVision** (`sample.uvprojx`).
2. Compilare il progetto (**F7**).
3. Caricare sulla scheda LandTiger LPC1768, oppure eseguire nel **simulatore**
   (la macro `SIMULATOR` in `sample.c` è già definita).

---

## Autore

**Matteo Zylfo** — Politecnico di Torino

Progetto basato sul sample GLCD/Touch Panel originale di PowerMCU / AVRman.
