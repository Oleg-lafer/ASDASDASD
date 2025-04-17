הנה קובץ `README.md` מושקע ומפורט לפרויקט שלך, כולל תיאור, שימושים, עקרונות עבודה, תלותיות, הוראות הפעלה ועוד. אחריו אוסיף גם את הקובץ `requirements.js` שציינת (אני מניח שהתכוונת לדרישות התלויות בקוד או קובץ דמוי `requirements.txt` אבל עבור JavaScript או במקרה שלנו פשוט רשימת ספריות ותלויות בסביבת C++).

---

### ✅ `README.md`

```markdown
# Fortuna C++ - Cryptographically Secure PRNG

A **complete implementation of the Fortuna cryptographically secure pseudorandom number generator (PRNG)** in C++, based on the [original Fortuna design by Bruce Schneier and Niels Ferguson](https://www.schneier.com/academic/archives/2002/08/the_fortuna_random.html).

This project demonstrates a modern, efficient, and educational implementation of Fortuna, written in C++ with inline documentation. It uses **OpenSSL** for cryptographic primitives and is structured with clear modular classes for entropy accumulation, secure key management, and AES-CTR-based random generation.

---

## 🚀 Features

- 🔐 AES-256 in CTR mode for secure random number generation
- 🔄 Automatic key rekeying after 1MB output (as per Fortuna spec)
- 📥 32 entropy pools with adaptive accumulation logic
- 💾 Persistent seed storage (saved and loaded from file)
- 🧠 Clear object-oriented design: `EntropyAccumulator`, `SeedManager`, `Generator`, `Fortuna`
- 🧪 Easy to test and extend with your own entropy sources (e.g., sensors)

---

## 📁 Project Structure

```
Fortuna/
│
├── Fortuna.cpp           # Main C++ implementation
├── seed.dat              # Binary file that stores seed state (auto-created)
├── README.md             # Project documentation
└── requirements.js       # Dependency list for build
```

---

## 🔧 Dependencies

This project uses the following libraries:

- [OpenSSL](https://www.openssl.org/) (libcrypto)
  - AES-256 in CTR mode
  - SHA-256 hashing
  - Secure random bytes (`RAND_bytes`)

> ⚠️ Make sure OpenSSL is installed and linked correctly with your C++ compiler.

---

## 🛠️ Build Instructions

### Linux / macOS (g++)

```bash
g++ Fortuna.cpp -o fortuna -lssl -lcrypto
```

### Windows (MinGW or MSVC)

Make sure OpenSSL is available in your system and linked:

```bash
g++ Fortuna.cpp -o fortuna.exe -lssl -lcrypto
```

---

## 📦 Usage Example

```bash
./fortuna
```

Console output:
```
Generated random data: 4c7e2f9d81a334b6...
```

You can modify the main function to:
- Add entropy from sensors or files
- Generate longer random streams
- Save and reload state across executions

---

## 🔍 Class Overview

| Class               | Responsibility                                       |
|--------------------|------------------------------------------------------|
| `EntropyAccumulator` | Stores entropy in 32 pools and creates reseed input |
| `SeedManager`        | Loads/saves 256-bit seed to `seed.dat` file         |
| `Generator`          | AES-CTR random block generator with rekeying logic  |
| `Fortuna`            | Orchestrates entropy, reseeding, and generation     |

---

## 📚 Further Reading

- 🔗 [Fortuna Paper (Schneier & Ferguson)](https://www.schneier.com/academic/archives/2002/08/the_fortuna_random.html)
- 📘 [OpenSSL EVP Manual](https://www.openssl.org/docs/manmaster/man3/EVP_EncryptInit.html)
- 📎 [AES-CTR Explanation](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Counter_(CTR))

---

## ❤️ Credits

- Original design: Bruce Schneier & Niels Ferguson  
- C++ Implementation: Ported and adapted from JavaScript by [Your Name]

---

## 🧪 Coming Soon (Ideas)

- Entropy from hardware sensors (ESP32, Raspberry Pi)
- Integration with system entropy pool
- Benchmarking and performance metrics
- Interface for secure random UUID generation

---

## 📜 License

This project is released under the MIT License.  
Feel free to use, modify, and learn from it freely.

---
```

---

### 📦 `requirements.js` (או יותר נכון: Build Requirements)

השם מעט מבלבל – ב-C++ נהוג פשוט לציין את הדרישות בתוך README או ב־CMakeLists.txt אם משתמשים ב־CMake. אבל הנה גרסה פשוטה של `requirements.js` בסגנון אינפורמטיבי בלבד:

```js
// This is a documentation-style file for the required dependencies
// for the Fortuna C++ PRNG project

module.exports = {
  name: "Fortuna C++",
  language: "C++",
  requiredLibraries: [
    {
      name: "OpenSSL",
      includes: ["openssl/evp.h", "openssl/rand.h", "openssl/sha.h"],
      description: "Used for AES-256-CTR encryption, SHA-256 hashing, and random byte generation.",
      install: "sudo apt install libssl-dev"
    },
    {
      name: "C++17 Standard",
      features: ["std::vector", "std::array", "fstream", "cstring"],
      description: "Standard library features used in the project"
    }
  ],
  optional: [
    {
      name: "CMake",
      description: "For cross-platform building and configuration",
      install: "sudo apt install cmake"
    }
  ]
}
```

---

רוצה שאבנה גם `CMakeLists.txt` כדי שתוכל לבנות את הפרויקט בפשטות בכל מערכת הפעלה?
