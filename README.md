# 🐍 Terminal Snake Game in C

A classic terminal-based Snake game written in C, designed to run in a UNIX environment[cite: 1].

---

## 🎮 How to Play / Controls
* **W** – Move Up[cite: 1]
* **S** – Move Down[cite: 1]
* **A** – Move Left[cite: 1]
* **D** – Move Right[cite: 1]
* **Q** – Quit Game[cite: 1]

---

## 🛠️ Features & Implementation
* **Real-time Input**: Non-blocking input handling using `termios` and `kbhit()` without needing to press Enter[cite: 1].
* **High Score System**: Scores are saved and persisted via `high_score.txt`[cite: 1].
* **Sound Effects**: Audio playback support using `mpg123` (MP3) and `aplay` (WAV)[cite: 1].
* **ANSI Graphics**: Smooth screen refreshing and colored elements using ANSI escape codes (`\033[H\033[J`)[cite: 1].

---

## ⚠️ Challenges & Solutions
* **Input Delay**: Solved the issue of needing an Enter key press by implementing real-time `kbhit()` logic[cite: 1].
* **Emojis vs ASCII**: Replaced emojis (which caused terminal alignment issues) with clean ASCII characters (`@`, `#`, `^`, etc.)[cite: 1].
* **Matrix Logic**: Used direct `x` and `y` coordinate tracking instead of a complex 2D character matrix[cite: 1].

---
