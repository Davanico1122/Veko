
# 💛 Veko Native Project

Veko is a simple scripting language with clean syntax, written in C.  
This project contains the **native Veko interpreter** and some `.vk` code examples.

---

## 📦 Files

| File                       | Description                      |
|----------------------------|----------------------------------|
| `veko_native_v3.c`         | The C interpreter for `.vk` files |
| `hello.vk`                 | Simple demo file                 |
| `veko_script_superpower.vk`| Full demo of Veko syntax         |
| `.gitattributes`           | Tell GitHub this is Veko         |

---

## 🚀 How to Use (Windows / Linux)

### 1. Compile Veko
```bash
gcc veko_native_v3.c -o veko
```

### 2. Run Veko file
```bash
./veko hello.vk
./veko veko_script_superpower.vk
```

---

## ✅ Supported Syntax

### Say something
```veko
say: "Hello, World!"
```

### Variable and Math
```veko
x <- 5 + 3
say: x
```

### If / Else (when)
```veko
when x > 5 {
  say: "Big"
} else {
  say: "Small"
}
```

### Loop
```veko
loop 3 times {
  say: "Repeat"
}
```

### While Loop
```veko
n <- 0
while n < 5 {
  say: n
  n <- n + 1
}
```

### Functions
```veko
fn greet {
  say: "Hi!"
}
call greet
```

### Input
```veko
name <- ask
say: name
```

### Import another .vk
```veko
import "other.vk"
```

---

## 🧪 Demo Superpower Script

Run the full-featured script:

```bash
./veko veko_script_superpower.vk
```

---

Made with 💛 by Davanico1122
