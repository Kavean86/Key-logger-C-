# Linux Key Logger

A lightweight **Linux keyboard event monitor written in C++** using the Linux input event interface.

The project reads keyboard events directly from `/dev/input/eventX`, maps Linux keyboard event codes to human-readable keys, handles `Shift` and `Caps Lock`, and displays special keys with different terminal colors.

This project was created to explore **Linux input devices, low-level event handling, file descriptors, C++ system programming, and keyboard event processing**.

> ⚠️ **Educational Use Only:** Use this project only on systems you own or where you have explicit permission to monitor keyboard input. Keyboard event monitoring can expose sensitive information such as passwords and private messages.

## Features

* ⌨️ Reads keyboard events directly from Linux input devices
* 🐧 Designed for Linux
* ⚙️ Written in C++
* 🔌 Uses `/dev/input/event1`
* 🧩 Uses Linux `input_event` structures
* 🔤 Converts key codes into readable characters
* 🔠 Supports `Shift`
* 🔡 Supports `Caps Lock`
* ␣ Handles `Space`
* 🎨 Uses different terminal colors for special keys
* 🔢 Supports number keys
* ⌨️ Supports function keys from `F1` to `F12`
* 🧭 Supports navigation keys
* 🔢 Includes numpad key mappings

## How It Works

Linux exposes input devices through the `/dev/input/` interface.

This project opens:

```text
/dev/input/event1
```

and continuously reads `input_event` structures.

```text
Keyboard
   │
   ▼
Linux Input Subsystem
   │
   ▼
/dev/input/event1
   │
   ▼
input_event
   │
   ▼
C++ Event Processing
   │
   ├── Key Mapping
   ├── Shift Handling
   ├── Caps Lock Handling
   └── Special Key Coloring
   │
   ▼
Terminal Output
```

The program ignores events that are not keyboard events:

```cpp
if (event.type != EV_KEY)
    continue;
```

It then extracts the key code and event value:

```cpp
int code = event.code;
int value = event.value;
```

The key code is looked up in the project's keyboard database.

## Keyboard Database

The file `database.cpp` contains a mapping between Linux keyboard event codes and readable key names.

For example:

```cpp
{{16, 1}, "Q"},
{{17, 1}, "W"},
{{18, 1}, "E"},
```

Special keys are also mapped:

```cpp
{{28, 1}, "ENTER"},
{{29, 1}, "LEFT CTRL"},
{{57, 1}, "SPACE"},
{{58, 1}, "CAPS LOCK"},
```

Navigation keys are included as well:

```cpp
{{102, 1}, "HOME"},
{{103, 1}, "UP"},
{{105, 1}, "LEFT"},
{{106, 1}, "RIGHT"},
{{110, 1}, "INSERT"},
{{111, 1}, "DELETE"},
```

## Shift and Caps Lock

The program keeps track of the current `Shift` state:

```cpp
bool shift = false;
```

It also maintains a separate `Caps Lock` state:

```cpp
bool caps = false;
```

For alphabetic characters, the final case is determined using:

```cpp
bool upper = shift ^ caps;
```

This allows combinations such as:

```text
Shift + A       → A
Caps Lock + A   → A
Shift + Caps+A  → a
```

## Colored Special Keys

Special keys are displayed using ANSI terminal colors.

The project defines several colors:

```cpp
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";
const string WHITE   = "\033[37m";
```

Different special keys are assigned different colors.

For example:

| Key                 | Color   |
| ------------------- | ------- |
| `ESC`               | Red     |
| `TAB`               | Green   |
| `ENTER`             | Yellow  |
| `BACKSPACE`         | Blue    |
| `SPACE`             | Magenta |
| Arrow keys          | Cyan    |
| `DELETE` / `INSERT` | Red     |
| `HOME` / `END`      | Green   |
| `F1` - `F4`         | Yellow  |
| `F5` - `F8`         | Blue    |
| `F9` - `F12`        | Magenta |

Normal letters and numbers are displayed without the special-key formatting.

## Project Structure

```text
Key-logger-C-/
│
├── main.cpp
├── database.cpp
├── README.md
└── LICENSE
```

### `main.cpp`

Contains the main event-processing logic.

Responsibilities include:

* Opening the input event device
* Reading keyboard events
* Detecting key presses
* Handling Shift
* Handling Caps Lock
* Looking up key names
* Formatting special keys
* Printing the result

### `database.cpp`

Contains the keyboard event-code database.

It maps Linux keyboard codes to human-readable names.

## Requirements

* Linux
* C++ compiler
* Linux input subsystem
* Access to the selected `/dev/input/event*` device

## Compilation

Compile the project with:

```bash
g++ main.cpp -o keylogger
```

## Running

The program currently uses:

```text
/dev/input/event1
```

Run it with sufficient permissions:

```bash
sudo ./keylogger
```

Depending on the system, the keyboard may be associated with a different event device.

You can inspect available input devices with:

```bash
ls /dev/input/
```

You can also inspect the devices recognized by Linux:

```bash
cat /proc/bus/input/devices
```

## Example

After starting the program, normal keyboard input may appear directly:

```text
Hello World 123
```

Special keys are displayed using their mapped names:

```text
[ENTER] [TAB] [BACKSPACE]
```

The actual special-key representation is colorized in the terminal.

## Technologies

* **C++**
* **Linux Input Subsystem**
* **POSIX/Linux system calls**
* `open()`
* `read()`
* `close()`
* `/dev/input/event*`
* `linux/input.h`
* ANSI escape sequences

## Learning Objectives

This project is primarily intended to provide practical experience with:

* Linux device files
* File descriptors
* Low-level input handling
* Linux kernel input events
* `struct input_event`
* Keyboard event codes
* C++ STL containers
* `std::map`
* Terminal ANSI escape sequences
* Bitwise XOR logic for keyboard state handling
* System-level programming

## Limitations

The current implementation intentionally remains simple.

* The input device is hardcoded to `/dev/input/event1`.
* Keyboard layout handling is limited.
* The database contains manually defined key mappings.
* It is focused on Linux.
* It currently displays events rather than providing a configurable logging system.

## Future Improvements

Possible improvements include:

* Automatic keyboard device detection
* Support for multiple keyboard layouts
* More complete key mapping
* Configurable input device
* Better handling of modifier keys
* Timestamped events
* Cleaner separation between event handling and key mapping
* Configuration file for keyboard mappings
* Improved error handling

## Security Notice

This software interacts directly with keyboard input devices and may have access to sensitive keystrokes.

**Do not use it to monitor another person's activity, capture credentials, or collect private information without explicit authorization.**

Use it only in your own laboratory, test environment, or other authorized systems.

## Author

**Kaveh Nazem**

Original author and creator of this project.

## License

Copyright © 2026 **Kaveh Nazem**.

Licensed under the **Apache License, Version 2.0**.

See the [`LICENSE`](LICENSE) file for the complete license text.

---

Created by **Kaveh Nazem** as a practical Linux and C++ systems programming project.
