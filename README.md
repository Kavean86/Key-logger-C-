# Linux Key Logger

A lightweight **Linux keyboard event monitor written in C++** using the Linux input event interface.

The project reads keyboard events directly from Linux input devices through `/dev/input/event*`, automatically detects the appropriate keyboard input device, processes low-level `input_event` structures, maps Linux keyboard event codes to human-readable names, handles modifier states, and displays keyboard input in a readable and colorized format.

This project was created as a practical exploration of **Linux input devices, low-level event handling, file descriptors, C++ system programming, keyboard event processing, and the Linux input subsystem**.

> ⚠️ **Educational Use Only:** Use this project only on systems you own or where you have explicit permission to monitor keyboard input. Keyboard event monitoring can expose sensitive information such as passwords and private messages.

---

## Features

* ⌨️ Reads keyboard events directly from Linux input devices
* 🐧 Designed for Linux
* ⚙️ Written in C++
* 🔍 **Automatically detects the keyboard input device**
* 🔌 Works with `/dev/input/event*`
* 🧩 Uses Linux `input_event` structures
* 🔤 Converts Linux keyboard event codes into human-readable key names
* 🔠 Supports `Shift`
* 🔡 Supports `Caps Lock`
* 🧠 Uses XOR logic for `Shift` and `Caps Lock` state handling
* 🔢 Supports number keys
* 🔤 Supports alphabetic keys
* ␣ Supports `Space`
* ↩️ Supports `Enter`
* ↹ Supports `Tab`
* ⌫ Supports `Backspace`
* 🎛️ Supports modifier keys
* ⌨️ Supports function keys from `F1` to `F12`
* 🧭 Supports navigation keys
* ⬆️ Supports arrow keys
* 🏠 Supports `Home` and `End`
* 📍 Supports `Insert` and `Delete`
* 🔢 Includes numpad key mappings
* 🎨 Uses ANSI terminal colors for special keys
* 🗂️ Uses a separate keyboard database for key mappings
* 🔍 Filters non-keyboard events
* 🖥️ Displays processed keyboard events directly in the terminal

---

## How It Works

Linux exposes input devices through the `/dev/input/` interface.

Instead of relying on normal terminal input, this project works directly with the Linux input subsystem.

The program searches available input event devices and automatically identifies the appropriate keyboard device.

```text
Keyboard
   │
   ▼
Linux Input Subsystem
   │
   ▼
/dev/input/event*
   │
   ▼
Automatic Keyboard Detection
   │
   ▼
input_event
   │
   ▼
C++ Event Processing
   │
   ├── Event Filtering
   ├── Key Code Extraction
   ├── Keyboard Database Lookup
   ├── Modifier State Handling
   ├── Shift Handling
   ├── Caps Lock Handling
   ├── Character Conversion
   ├── Special Key Detection
   └── Terminal Formatting
   │
   ▼
Readable Keyboard Output
```

The program filters events so that only keyboard events are processed:

```cpp
if (event.type != EV_KEY)
    continue;
```

The keyboard event code and event value are then extracted:

```cpp
int code = event.code;
int value = event.value;
```

The event code is looked up in the project's keyboard database and processed according to its type and current keyboard state.

---

## Automatic Keyboard Detection

One of the project's implemented features is **automatic keyboard input-device detection**.

Instead of depending on a hardcoded device such as:

```text
/dev/input/event1
```

the program searches the available Linux input devices and identifies the appropriate keyboard device automatically.

This makes the program more portable across different Linux systems where the keyboard may be assigned to a different event number.

Conceptually:

```text
/dev/input/
    │
    ├── event0
    ├── event1
    ├── event2
    ├── event3
    └── ...
          │
          ▼
   Device Detection
          │
          ▼
   Keyboard Device
          │
          ▼
   Event Processing
```

This removes the need for the user to manually determine which `/dev/input/eventX` device corresponds to the keyboard.

---

## Linux `input_event`

The project works directly with the Linux `input_event` structure provided by:

```cpp
#include <linux/input.h>
```

Keyboard activity is exposed by Linux as input events.

The program primarily processes:

```cpp
event.type == EV_KEY
```

The event value can represent different states of a key:

```text
0 → Key released
1 → Key pressed
2 → Key repeat
```

This allows the program to distinguish between key presses, releases, and repeated key events.

---

## Keyboard Database

The project uses a separate `database.cpp` file containing mappings between Linux keyboard event codes and human-readable key names.

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

Keeping these mappings in a separate file allows the keyboard database to remain independent from the main event-processing logic.

---

## Modifier Key Handling

The program keeps track of modifier-key states instead of treating every keyboard event as an isolated character.

For example, the current `Shift` state is stored using:

```cpp
bool shift = false;
```

The program also tracks the current `Caps Lock` state:

```cpp
bool caps = false;
```

For alphabetic characters, the final case is determined using XOR logic:

```cpp
bool upper = shift ^ caps;
```

This produces the expected behavior:

```text
Shift + A             → A
Caps Lock + A         → A
Shift + Caps Lock+A   → a
```

This allows the program to reproduce the interaction between `Shift` and `Caps Lock` instead of simply mapping each key to a fixed character.

---

## Special Key Handling

The project distinguishes normal printable keys from special keyboard keys.

Supported special keys include:

```text
ESC
TAB
ENTER
BACKSPACE
SPACE
LEFT CTRL
LEFT ALT
SHIFT
CAPS LOCK
ARROW KEYS
HOME
END
INSERT
DELETE
F1 - F12
NUMPAD KEYS
```

Special keys are displayed using readable names instead of being treated as normal characters.

Example:

```text
Hello[SPACE]World[ENTER]
```

The exact representation is colorized in the terminal.

---

## Colored Terminal Output

The project uses ANSI escape sequences to visually distinguish special keys.

The following terminal colors are used:

```cpp
const string RESET   = "\033[0m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";
const string WHITE   = "\033[37m";
```

Different categories of special keys are assigned different colors.

| Key / Category      | Color   |
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

Normal letters and numbers are displayed without special-key formatting.

---

## Supported Keyboard Input

The current keyboard database includes mappings for several categories of keys.

### Alphabetic Keys

```text
A - Z
```

### Number Keys

```text
0 - 9
```

### Modifier Keys

```text
SHIFT
LEFT CTRL
LEFT ALT
CAPS LOCK
```

### Control Keys

```text
ESC
TAB
ENTER
BACKSPACE
SPACE
```

### Function Keys

```text
F1
F2
F3
F4
F5
F6
F7
F8
F9
F10
F11
F12
```

### Navigation Keys

```text
UP
DOWN
LEFT
RIGHT
HOME
END
INSERT
DELETE
```

### Numpad

The project also contains mappings for numpad keys.

---

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

* Detecting the keyboard input device
* Opening the selected input device
* Reading `input_event` structures
* Filtering keyboard events
* Detecting key states
* Processing keyboard event codes
* Tracking `Shift`
* Tracking `Caps Lock`
* Looking up keyboard mappings
* Detecting special keys
* Formatting special-key output
* Applying terminal colors
* Printing processed events

### `database.cpp`

Contains the keyboard event-code database.

It maps Linux keyboard event codes to human-readable names and provides the mapping layer used by the main event-processing logic.

---

## Requirements

* Linux
* C++ compiler
* Linux input subsystem
* Access to Linux input devices
* Sufficient permissions to read the selected input device

---

## Compilation

Compile the project with:

```bash
g++ main.cpp -o keylogger
```

---

## Running

Run the program with sufficient permissions:

```bash
sudo ./keylogger
```

The program automatically searches for the appropriate keyboard input device.

There is no need to manually specify:

```text
/dev/input/event1
```

on systems where automatic device detection succeeds.

Available input devices can still be inspected manually with:

```bash
ls /dev/input/
```

Detailed device information can be viewed with:

```bash
cat /proc/bus/input/devices
```

---

## Example

After starting the program, normal keyboard input may appear as:

```text
Hello World 123
```

Special keys are displayed using readable names:

```text
Hello[SPACE]World[ENTER]
```

Other keyboard events may appear as:

```text
LEFT CTRL
LEFT ALT
TAB
UP
LEFT
RIGHT
BACKSPACE
DELETE
F1
F5
```

Special keys are colorized in the terminal according to their category.

---

## Technologies

* **C++**
* **Linux Input Subsystem**
* **Linux Device Files**
* **POSIX/Linux System Calls**
* `open()`
* `read()`
* `close()`
* `/dev/input/event*`
* `linux/input.h`
* `struct input_event`
* C++ STL
* `std::map`
* ANSI escape sequences
* Linux keyboard event codes

---

## Learning Objectives

This project was created to gain practical experience with:

* Linux device files
* `/dev/input/`
* File descriptors
* Linux input devices
* Linux input events
* The Linux Input Subsystem
* `struct input_event`
* Keyboard event codes
* Low-level event processing
* C++ system programming
* POSIX system calls
* `std::map`
* Keyboard state management
* Modifier keys
* `Shift` and `Caps Lock` interaction
* Bitwise XOR logic
* Terminal ANSI escape sequences
* Separating keyboard data from event-processing logic
* Working with Linux hardware interfaces from user space

---

## Limitations

The current implementation remains intentionally lightweight.

* Keyboard layout handling is limited
* Keyboard mappings are manually defined
* The implementation is Linux-specific
* Different keyboard layouts may require additional mappings
* The current output is primarily designed for terminal display
* The keyboard database may not contain every possible Linux key code

---

## Future Improvements

Possible future improvements include:

* Support for additional keyboard layouts
* More complete keyboard-code mappings
* Better handling of international keyboard layouts
* Improved modifier-key handling
* Timestamped events
* Configurable output formats
* Configuration files for keyboard mappings
* Cleaner separation between event processing and presentation
* Improved error handling
* Additional Linux input-device support

---

## Security Notice

This software interacts directly with Linux keyboard input devices and may have access to sensitive keystrokes.

**Do not use it to monitor another person's activity, capture credentials, or collect private information without explicit authorization.**

Use this project only on systems you own, in your own laboratory, test environment, or on systems where you have explicit permission to monitor keyboard input.

---

## Author

**Kaveh Nazem**

Original author and creator of this project.

---

## License

Copyright © 2026 **Kaveh Nazem**.

Licensed under the **Apache License, Version 2.0**.

---

Created by **Kaveh Nazem** as a practical Linux and C++ systems programming project.
