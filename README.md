# 🏦 C Banking System

A simple terminal-based banking system written in C. It allows users to register, log in, and perform various banking operations such as deposits, withdrawals, transfers, and viewing transaction history. User data and session logs are persistently stored in `.dat` files.

---

## 📁 File Structure

```markdown
# File Structure:

* [data](data/)
  * [session.dat](data/session.dat)
  * [transactions.dat](data/transactions.dat)
  * [users.dat](data/users.dat)
* [src](src/)
  * [main.c](src/main.c)
* [README.md](README.md)
```

---

## ✅ Features

- 📌 User Registration & Login
- 💰 Deposit, Withdraw, Transfer (with PIN verification)
- 📄 Transaction History Logging and Viewing
- 🔐 Change Password or PIN
- 🔍 User Info Editing
- 📆 Session Logging (who logged in and when)
- ⚠️ Basic error handling and input validation
- 💾 Persistent file-based storage using `.dat` files

---

## 👤 User Data Fields

Each user has the following data:

- `Account ID` – Unique internal ID
- `Account Number` – Public-facing banking number
- `First Name`
- `Last Name`
- `Phone Number`
- `Password`
- `PIN` – 4-digit verification for sensitive operations
- `Birthday`
- `Balance`

---

## ⚙️ How to Compile

Open terminal, navigate to the `src/` directory and run:

```
gcc main.c -o banking
```

## 🚀 How to Run

Still inside the src/ directory, execute the program with:

```
./banking
```

Make sure the data/ folder exists in the parent directory. If it does not exist, create it:

```
mkdir ../data
```

## 📂 Session Logging

Every time a user logs in successfully, their account number and timestamp are saved in data/session.dat. The format is:

```
[YYYY-MM-DD HH:MM:SS] AccountNumber
```

This helps track login history.

## 📝 Notes

PIN is required to make deposits, withdrawals, or transfers.

Data is stored in plain text format and not encrypted.

This project is for educational purposes only. Do not use in production environments.

## 🧑‍💻 License

This project is open-source and free to use for learning and experimentation.