# 🏦 C Banking System

A simple terminal-based banking system written in C. It allows users to register, log in, and perform various banking operations such as deposit, withdrawal, transfer, and view transaction history. User data is stored persistently in `.dat` files.

## 📂 File Structure

c-banking-system/
├── data/
│ ├── users.dat # Stores user data
│ ├── transactions.dat # Stores transaction history
├── src/
│ └── main.c # Main source code
├── README.md # Project documentation


## ✅ Features

- User Registration & Login
- Deposit, Withdraw, Transfer (with PIN verification)
- Transaction Logging and Viewing
- Change Password or PIN
- Basic error handling and data persistence

## 🧑‍💻 User Fields

Each user has the following information stored:
- Account ID
- Account Number
- First Name
- Last Name
- Phone Number
- Password
- Birthday
- PIN
- Balance

## ⚙️ How to Compile

Make sure you are in the `src/` directory:

"gcc main.c -o banking"

## 🚀 How to Run

From the src/ directory:

"./banking"

Make sure the data/ folder exists in the parent directory before running the program. If not, create it:

"mkdir ../data"

## 📌 Notes

PIN is required for deposit, withdrawal, and transfers.

Data is saved in plain text files. This system is not secure for real-world use.

## 📝 License

This project is provided for educational purposes only.