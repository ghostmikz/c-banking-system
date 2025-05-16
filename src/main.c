#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_USERS 100
#define MAX_NOTE_LENGTH 100
#define MAX_LOANS 100

struct User {
    int accountID;
    int accountNumber;
    char firstName[50];
    char lastName[50];
    char phone[20];
    char password[50];
    char birthday[11]; // YYYY-MM-DD
    double balance;
    char pin[5];
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
int verifyPin(int userId);
void loadUsers();
void saveUsers();
void registerUser();
void showMenu();
void showLoginAnimation();
void changePin(int userId);
void changePassword(int userId);
void showDashboard(int userId);
void showSettings(int userId);
void viewBalance(int userId);
void transferMoney(int userId);
void withdrawMoney(int userId);
void depositMoney(int userId);
void viewTransactionHistory(int userId);
void logSession(int accountNumber);
void logTransaction(int accountNumber, const char* type, double amount, const char* note);

int main() {
    printf("👋 Таны банкны системд тавтай морилно уу!\n");
    
    loadUsers();

    int userId = displayPreLoginMenu(); 
    showDashboard(userId);
    
    return 0;
}

void showLoginAnimation() {
    const char *frames[] = {
        "[=     ]",
        "[==    ]",
        "[===   ]",
        "[====  ]",
        "[===== ]",
        "[======]"
    };
    printf("⏳ Нэвтэрч байна ");
    fflush(stdout);

    for (int i = 0; i < 12; i++) {
        printf("%s", frames[i % 6]);
        fflush(stdout);
        usleep(150000);
        printf("\b\b\b\b\b\b\b\b");  
        fflush(stdout);
    }

    printf("%s\n", frames[5]);  
    printf("✅ Амжилттай нэвтэрлээ!\n");
}

void logSession(int accountNumber) {
    FILE *file = fopen("../data/session.dat", "a");
    if (file == NULL) {
        printf("⚠️ Session файлыг нээж чадсангүй.\n");
        return;
    }

    time_t now = time(NULL);
    char *timeStr = ctime(&now);
    timeStr[strcspn(timeStr, "\n")] = 0; 

    fprintf(file, "%d %s\n", accountNumber, timeStr);
    fclose(file);
}

void changePin(int userId) {
    char newPin[5];
    printf("\n🔒 Шинэ PIN код оруулна уу: ");
    scanf("%4s", newPin);  

    printf("Шинэ PIN кодыг дахин оруулна уу: ");
    char confirmPin[5];
    scanf("%4s", confirmPin);

    if (strcmp(newPin, confirmPin) != 0) {
        printf("⚠️ PIN кодын баталгаажуулалт алдаатай байна.\n");
        return;
    }

    strcpy(users[userId].pin, newPin);

    saveUsers();

    printf("✅ PIN код амжилттай солигдлоо.\n");
}

int verifyPin(int userId) {
    char inputPin[5];
    printf("\n🔒 PIN кодоо оруулна уу: ");
    scanf("%4s", inputPin); 

    if (strcmp(inputPin, users[userId].pin) == 0) {
        return 1;  
    } else {
        printf("⚠️ Буруу PIN.\n");
        return 0;  
    }
}

void changePassword(int userId) {
    char currentPassword[50], newPassword[50], confirmPassword[50];
    
    printf("\n🔑 Нууц үгээ солих\n-------------------\n");

    printf("Одоогийн нууц үг: ");
    scanf("%s", currentPassword);

    if (strcmp(currentPassword, users[userId].password) != 0) {
        printf("❌ Одоогийн нууц үг буруу байна.\n");
        return;
    }

    printf("Шинэ нууц үг: ");
    scanf("%s", newPassword);

    printf("Шинэ нууц үгээ давтан оруулна уу: ");
    scanf("%s", confirmPassword);

    if (strcmp(newPassword, confirmPassword) != 0) {
        printf("❌ Шинэ нууц үг давхардахгүй байна.\n");
        return;
    }

    strcpy(users[userId].password, newPassword);
    saveUsers();

    printf("✅ Нууц үг амжилттай солигдлоо!\n");
}

void showSettings(int userId) {
    int choice;
    
    while (1) {
        printf("\n=========================================\n");
        printf("        ⚙️ Тохиргоо\n");
        printf("=========================================\n");
        printf(" 1. Нууц үг солих\n");
        printf(" 2. PIN код солих\n"); 
        printf(" 3. Буцах\n");
        printf("=========================================\n");
        printf("Сонголтоо оруулна уу (1-3): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                changePassword(userId);
                break;
            case 2:
                changePin(userId);
                break;
            case 3:
                return; 
            default:
                printf("⚠️  1 - 3 хооронд сонголт хийнэ үү...\n");
        }
    }
}

void transferMoney(int userId) {
    if (!verifyPin(userId)) {
        return; 
    }

    int receiverAccount;
    double amount;
    char senderNote[100], receiverNote[100];
    int receiverId = -1;

    printf("\n🔁 Мөнгө шилжүүлэх\n-------------------\n");
    printf("Хүлээн авагчийн дансны дугаарыг оруулна уу: ");
    scanf("%d", &receiverAccount);

    if (receiverAccount == users[userId].accountNumber) {
        printf("⚠️  Өөрийн данс руу шилжүүлэх боломжгүй.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        if (users[i].accountNumber == receiverAccount) {
            receiverId = i;
            break;
        }
    }

    if (receiverId == -1) {
        printf("⚠️  Хүлээн авагч олдсонгүй.\n");
        return;
    }

    printf("Шилжүүлэх дүн: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("⚠️  Мөнгөний дүн нь эерэг байх ёстой.\n");
        return;
    }

    if (amount > users[userId].balance) {
        printf("⚠️  Үлдэгдэл хүрэлцэхгүй байна.\n");
        return;
    }

    users[userId].balance -= amount;
    users[receiverId].balance += amount;

    snprintf(senderNote, sizeof(senderNote), "Данс %d руу шилжүүлсэн", users[receiverId].accountNumber);
    snprintf(receiverNote, sizeof(receiverNote), "Данс %d-с хүлээн авсан", users[userId].accountNumber);

    logTransaction(users[userId].accountNumber, "Зарлага", amount, senderNote);
    logTransaction(users[receiverId].accountNumber, "Орлого", amount, receiverNote);

    saveUsers();

    printf("✅ %.2f₮ амжилттай шилжүүллээ. Үлдэгдэл: %.2f₮\n", amount, users[userId].balance);
}

void withdrawMoney(int userId) {
    if (!verifyPin(userId)) {
        return; 
    }
    double amount;
    char note[100] = "Зарлага";

    printf("\n💸 Мөнгө гаргах\n-------------------\n");
    printf("Бодит мөнгөн дүнг оруулна уу: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("⚠️  Мөнгөний дүн нь эерэг байх ёстой.\n");
        return;
    }

    if (amount > users[userId].balance) {
        printf("⚠️  Таны үлдэгдэл хүрэлцэхгүй байна.\n");
        return;
    }

    users[userId].balance -= amount;

    logTransaction(users[userId].accountNumber, "Зарлага", amount, note);

    saveUsers();

    printf("✅ %.2f₮ гаргалаа. Таны шинэ үлдэгдэл: %.2f₮\n", amount, users[userId].balance);
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
        printf("⚠️  Гүйлгээний файл оруулахад алдаа гарлаа.\n");
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
    if (!verifyPin(userId)) {
        return; 
    }
    double amount;
    char note[100] = "Орлого"; 
    
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
        printf(" 6. ⚙️ Тохиргоо\n");
        printf(" 7. 🚪 Гарах\n");
        printf("=========================================\n");
        printf("Сонголтоо оруулна уу (1-7): ");
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
                transferMoney(userId);
                break;
            case 5:
                viewTransactionHistory(userId);
                break;
            case 6:
                showSettings(userId); 
                break;
            case 7:
                printf("👋 Гарах үйлдлийг гүйцэтгэж байна...\n");
                return;
            default:
                printf("⚠️  1 - 7 хооронд сонголт хийнэ үү...\n");
        }
    }
}

void loadUsers() {
    FILE *file = fopen("../data/users.dat", "r");
    if (file == NULL) {
        printf("⚠️  Хэрэглэгчийн өгөгдлийн файл олдсонгүй. Хоосон өгөгдлийн сантай эхэлж байна.\n");
        return;
    }

    while (fscanf(file, "%d %d %49s %49s %19s %49s %10s %5s %lf\n", 
                   &users[userCount].accountID, 
                   &users[userCount].accountNumber, 
                   users[userCount].firstName, 
                   users[userCount].lastName, 
                   users[userCount].phone, 
                   users[userCount].password, 
                   users[userCount].birthday, 
                   users[userCount].pin,    
                   &users[userCount].balance) == 9) {
        userCount++;
    }

    fclose(file);
}


void saveUsers() {
    FILE *file = fopen("../data/users.dat", "w");
    if (file == NULL) {
        printf("⚠️  Хэрэглэгчийн өгөгдлийг хадгалах файлыг нээхэд алдаа гарлаа.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%d %d %s %s %s %s %s %s %.2f\n", 
                users[i].accountID, 
                users[i].accountNumber, 
                users[i].firstName, 
                users[i].lastName, 
                users[i].phone, 
                users[i].password, 
                users[i].birthday, 
                users[i].pin,
                users[i].balance);
    }

    fclose(file);
    printf("✅ Хэрэглэгчийн өгөгдөл амжилттай хадгалагдлаа.\n");
}

void registerUser() {
    char phone[20], pin[5];
    
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
    
    printf("6. PIN код (4 оронтой): ");
    scanf(" %[^\n]", pin);
    
    if (strlen(pin) != 4) {
        printf("⚠️  PIN код 4 оронтой байх ёстой.\n");
        return;
    }

    strcpy(users[userCount].pin, pin);
    
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
            showLoginAnimation(); 
            logSession(users[i].accountNumber);  
            return i;  
        }
    }    

    printf("❌ Нэвтрэх нэр эсвэл нууц үг буруу байна.\n");
    return -1;  
}