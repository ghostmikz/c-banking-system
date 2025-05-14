#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 100

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

struct User users[MAX_USERS];
int userCount = 0;

int loginUser();
int displayPreLoginMenu();
void loadUsers();
void saveUsers();
void registerUser();
void showMenu();
void showDashboard(int userId);
void viewBalance(int userId);

int main() {
    printf("👋 Таны банкны системд тавтай морилно уу!\n");
    
    loadUsers();

    int userId = displayPreLoginMenu(); 
    showDashboard(userId);
    
    return 0;
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
                viewBalance();
                break;
            case 2:
                //depositMoney();
                break;
            case 3:
                //withdrawMoney();
                break;
            case 4:
                //transferMoney();
                break;
            case 5:
                //viewTransactionHistory();
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
