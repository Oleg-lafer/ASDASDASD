
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

