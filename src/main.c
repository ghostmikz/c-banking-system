#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_NOTE_LENGTH 100

struct User {
    int accountID;
    int accountNumber;
    char firstName[50];
    char lastName[50];
    char phone[20];
    char password[50];
    char birthday[11]; // YYYY-MM-DD
    double balance;
};

struct Transaction {
    int accountNumber;
    char type[20];
    double amount;
    char note[MAX_NOTE_LENGTH];
    char date[20];
};

struct User users[MAX_USERS];
int userCount = 0;

int loginUser();
int displayPreLoginMenu();
void loadUsers();
void saveUsers();
void registerUser();
void showMenu();
void viewTransactionHistory(int userId);
void showDashboard(int userId);
void viewBalance(int userId);
void depositMoney(int userId);
void logTransaction(int accountNumber, const char* type, double amount, const char* note);

int main() {
    printf("👋 Таны банкны системд тавтай морилно уу!\n");
    
    loadUsers();

    int userId = displayPreLoginMenu(); 
    showDashboard(userId);
    
    return 0;
}

void viewTransactionHistory(int userId) {
    FILE *file = fopen("../data/transactions.dat", "r");
    if (file == NULL) {
        printf("⚠️  Гүйлгээний түүх олдсонгүй.\n");
        return;
    }

    char line[256];
    int acc;
    char type[20], note[MAX_NOTE_LENGTH], date[40];
    double amount;

    printf("\n📜 Гүйлгээний түүх\n-------------------\n");

    while (fgets(line, sizeof(line), file)) {
        char extra[100];
        if (sscanf(line, "%d %s %lf %s %[^\n]", &acc, type, &amount, note, extra) == 5) {
          
            char fullNoteDate[MAX_NOTE_LENGTH + 40];
            snprintf(fullNoteDate, sizeof(fullNoteDate), "%s %s", note, extra);

            if (acc == users[userId].accountNumber) {

                char *lastSpace = strrchr(fullNoteDate, ' ');
                if (lastSpace) {

                    strncpy(date, lastSpace - 8, 19);
                    date[19] = '\0';

                    *(lastSpace - 9) = '\0'; 
                    strcpy(note, fullNoteDate);
                } else {
                    strcpy(note, fullNoteDate);
                    strcpy(date, "???");
                }

                printf("📅 %s\n", date);
                printf("   %s %.2f₮ %s\n", type, amount, note);
                printf("-------------------\n");
            }
        }
    }

    fclose(file);
}


void logTransaction(int accountNumber, const char* type, double amount, const char* note) {
    FILE *file = fopen("../data/transactions.dat", "a");
    if (file == NULL) {
        printf("⚠️  Failed to open transactions file.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char date[20];

    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(file, "%d %s %.2f %s %s\n", accountNumber, type, amount, note, date);

    fclose(file);
}

void depositMoney(int userId) {
    double amount;
    char note[100] = "Орлого орох"; 
    
    printf("\n💰 Мөнгө хийх\n-------------------\n");
    printf("Бодит мөнгөн дүнг оруулна уу: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("⚠️  Мөнгөний дүн нь эерэг байх ёстой.\n");
        return;
    }

    users[userId].balance += amount;

    logTransaction(users[userId].accountNumber, "Орлого", amount, note);

    saveUsers();

    printf("✅ Амжилттай мөнгө хийгдлээ. Таны шинэ үлдэгдэл: %.2f₮\n", users[userId].balance);
}

void viewBalance(int userId) {
    printf("\n💰 Таны үлдэгдэл: %.2f₮\n", users[userId].balance);
}

int displayPreLoginMenu() {
    int choice;

    while (1) {
        printf("\n=========================================\n");
        printf("     📱 Банкны Системд Нэвтрэх\n");
        printf("=========================================\n");
        printf(" 1. 📋 Бүртгүүлэх\n");
        printf(" 2. 🔑 Нэвтрэх\n");
        printf(" 3. 🚪 Гарах\n");
        printf("=========================================\n");
        printf("Сонголтоо оруулна уу (1-3): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();  
                break;
            case 2:
                int userId = loginUser();
                if (userId != -1) return userId;
                break;
            case 3:
                printf("🚪 Та гарч байна...\n");
                exit(0); 
            default:
                printf("⚠️  1 - 3 хооронд сонголт хийнэ үү...\n");
        }
    }
}

void showDashboard(int userId) {
    int choice;

    while (1) {
        printf("\n=========================================\n");
        printf("        💼 Хэрэглэгчийн Самбар\n");
        printf("=========================================\n");
        printf(" 1. 💰 Үлдэгдэл харах\n");
        printf(" 2. ➕ Мөнгө хийх\n");
        printf(" 3. ➖ Мөнгө авах\n");
        printf(" 4. 🔁 Мөнгө шилжүүлэх\n");
        printf(" 5. 📜 Гүйлгээний түүх\n");
        printf(" 6. 🚪 Гарах\n");
        printf("=========================================\n");
        printf("Сонголтоо оруулна уу (1-6): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewBalance(userId);
                break;
            case 2:
                depositMoney(userId);
                break;
            case 3:
                withdrawMoney(userId);
                break;
            case 4:
                //transferMoney();
                break;
            case 5:
                viewTransactionHistory(userId);
                break;
            case 6:
                printf("👋 Гарах үйлдлийг гүйцэтгэж байна...\n");
                return;
            default:
                printf("⚠️  1 - 6 хооронд сонголт хийнэ үү...\n");
        }
    }
}

void loadUsers() {
    FILE *file = fopen("../data/users.dat", "r");
    if (file == NULL) {
        printf("⚠️  User data file not found. Starting with an empty database.\n");
        return;
    }

    while (fscanf(file, "%d %d %49s %49s %19s %49s %10s %lf\n", 
                   &users[userCount].accountID, 
                   &users[userCount].accountNumber, 
                   users[userCount].firstName, 
                   users[userCount].lastName, 
                   users[userCount].phone, 
                   users[userCount].password, 
                   users[userCount].birthday, 
                   &users[userCount].balance) == 8) {
        userCount++;
    }

    fclose(file);
}

void saveUsers() {
    FILE *file = fopen("../data/users.dat", "w");
    if (file == NULL) {
        printf("⚠️  Failed to open file for saving user data.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%d %d %s %s %s %s %s %.2f\n", 
                users[i].accountID, 
                users[i].accountNumber, 
                users[i].firstName, 
                users[i].lastName, 
                users[i].phone, 
                users[i].password, 
                users[i].birthday, 
                users[i].balance);
    }

    fclose(file);
    printf("✅ User data has been successfully saved.\n");
}

void registerUser() {
    char phone[20];
    
    printf("\n✍️ Бүртгүүлэх\n-------------------\n");
    
    printf("1. Утасны дугаар: ");
    scanf(" %[^\n]", phone);
    
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].phone, phone) == 0) {
            printf("⚠️  Энэ утасны дугаар аль хэдийн бүртгэгдсэн байна.\n");
            return;
        }
    }

    printf("2. Нэр: ");
    scanf(" %[^\n]", users[userCount].firstName);
    
    printf("3. Овог: ");
    scanf(" %[^\n]", users[userCount].lastName);
    
    strcpy(users[userCount].phone, phone);
    
    printf("4. Нууц үг: ");
    scanf(" %[^\n]", users[userCount].password);
    
    printf("5. Төрсөн огноо (YYYY-MM-DD): ");
    scanf(" %[^\n]", users[userCount].birthday);
    
    users[userCount].accountID = userCount + 1;  
    users[userCount].accountNumber = 100000 + userCount;  
    
    users[userCount].balance = 0.0;

    userCount++;

    saveUsers();

    printf("✅ Амжилттай бүртгэгдлээ!\n");
}

int loginUser() {
    char phone[20], password[20];
    
    printf("\n🔑 Нэвтрэх\n-------------------\n");

    printf("1. Утасны дугаар: ");
    scanf(" %[^\n]", phone);
    
    printf("2. Нууц үг: ");
    scanf(" %[^\n]", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].phone, phone) == 0 && strcmp(users[i].password, password) == 0) {
            printf("✅ Амжилттай нэвтэрлээ!\n");
            return i;  
        }
    }

    printf("❌ Нэвтрэх нэр эсвэл нууц үг буруу байна.\n");
    return -1;  
}
