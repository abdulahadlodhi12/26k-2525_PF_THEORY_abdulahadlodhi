#include <stdio.h>
#include <string.h>

void displayMenu() {
    printf("\n--- Univrsity Library Menu ---\n");
    printf("1. Membr Registration\n");
    printf("2. Display Availabe Books\n");
    printf("3. Borrow a Book\n");
    printf("4. Return a Book\n");
    printf("5. Display Membr Record\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

void registerMember(char name[], int *studentid, int *membertype) {
    printf("Enter name: ");
    scanf(" %[^\n]s", name);
    printf("Enter studnt ID: ");
    scanf("%d", studentid);
    printf("Enter membership type (1 for Student, 2 for Faculty): ");
    scanf("%d", membertype);
    while (*membertype != 1 && *membertype != 2) {
        printf("Inavlid type. Enter 1 for Student or 2 for Faculty: ");
        scanf("%d", membertype);
    }
}

void displayBooks(int bookids[], char booktitles[][50], int availablecopies[], int totalbooks) {
    printf("\nBook ID\t\tTitle\t\t\tAvailable Copies\n");

    for (int i = 0; i < totalbooks; i++) {
        printf("%d\t\t%s\t\t%d\n", bookids[i], booktitles[i], availablecopies[i]);
    }
}

void borrowBook(int bookids[], char booktitles[][50], int availablecopies[], int totalbooks, int borrowedbookids[], int *borrowedcount, int membertype) {
    if (*borrowedcount >= 3) {
        printf("Error: You can borrow a maxmum of three books at a time.\n");
        return;
    }

    int id;
    printf("Enter book ID to borrow: ");
    scanf("%d", &id);

    int index = -1;
    for (int i = 0; i < totalbooks; i++) {
        if (bookids[i] == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Inavlid book ID.\n");
        return;
    }

    if (availablecopies[index] <= 0) {
        printf("No copies availabe for this book.\n");
        return;
    }

    borrowedbookids[*borrowedcount] = id;
    (*borrowedcount)++;
    availablecopies[index]--;
    printf("Book borrowed successfully!\n");
}

float returnBook(int bookids[], int availablecopies[], int totalbooks, int borrowedbookids[], int *borrowedcount, int membertype, float *totalfine) {
    if (*borrowedcount <= 0) {
        printf("You have not borrowed any books.\n");
        return 0.0f;
    }

    int id;
    printf("Enter book ID to return: ");
    scanf("%d", &id);

    int foundindex = -1;
    int arrayindex = -1;
    for (int i = 0; i < *borrowedcount; i++) {
        if (borrowedbookids[i] == id) {
            arrayindex = i;
            break;
        }
    }

    if (arrayindex == -1) {
        printf("You have not borrowed this book.\n");
        return 0.0f;
    }

    for (int i = 0; i < totalbooks; i++) {
        if (bookids[i] == id) {
            foundindex = i;
            break;
        }
    }

    int days;
    printf("Enter actual number of days kept: ");
    scanf("%d", &days);
    while (days <= 0) {
        printf("Please enter a positive number of days: ");
        scanf("%d", &days);
    }

    int limit = (membertype == 1) ? 14 : 30;
    float currentfine = 0.0f;

    if (days > limit) {
        int extra = days - limit;
        int rate = (membertype == 1) ? 20 : 10;
        currentfine = extra * rate;
    }

    *totalfine += currentfine;

    if (foundindex != -1) {
        availablecopies[foundindex]++;
    }

    for (int i = arrayindex; i < *borrowedcount - 1; i++) {
        borrowedbookids[i] = borrowedbookids[i + 1];
    }
    (*borrowedcount)--;

    printf("Book returned successfully.\n");
    return currentfine;
}

void displayMemberRecord(char name[], int studentid, int membertype, int borrowedbookids[], int borrowedcount, float totalfine, float fineafterwaiver) {
    printf("\n--- Membr Record ---\n");
    printf("Name: %s\n", name);
    printf("Student ID: %d\n", studentid);
    printf("Membership Type: %s\n", (membertype == 1) ? "Student" : "Faculty");
    printf("Currently Borrowed Book IDs: ");
    if (borrowedcount == 0) {
        printf("None");
    } else {
        for (int i = 0; i < borrowedcount; i++) {
            printf("%d ", borrowedbookids[i]);
        }
    }
    printf("\nTotal Fine Charged (Before Waiver): Rs. %.2f\n", totalfine);
    printf("Final Fine Payable (After Waiver): Rs. %.2f\n", fineafterwaiver);
}

int main() {
    char name[100] = "";
    int studentid = 0;
    int membertype = 1;

    int bookids[5] = {101, 102, 103, 104, 105};
    char booktitles[5][50] = {"C Programming", "Data Structures", "Algorithms", "Database Systems", "Operating Systems"};
    int availablecopies[5] = {3, 2, 4, 1, 5};

    int borrowedbookids[3];
    int borrowedcount = 0;

    float totalfine = 0.0f;
    float fineafterwaiver = 0.0f;

    int choice;
    do {
        displayMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Inavlid input.\n");
            break;
        }

        if (choice == 1) {
            registerMember(name, &studentid, &membertype);
        } 
        else if (choice == 2) {
            displayBooks(bookids, booktitles, availablecopies, 5);
        } 
        else if (choice == 3) {
            borrowBook(bookids, booktitles, availablecopies, 5, borrowedbookids, &borrowedcount, membertype);
        } 
        else if (choice == 4) {
            float fine = returnBook(bookids, availablecopies, 5, borrowedbookids, &borrowedcount, membertype, &totalfine);
            if (fine > 0) {
                char code[20];
                printf("Enter membership code (e.g., LIB2026): ");
                scanf("%s", code);
                if (strcmp(code, "LIB2026") == 0) {
                    fineafterwaiver = totalfine * 0.5f;
                    printf("50% waiver applied successfully!\n");
                } else {
                    printf("Inocrrect code. No waiver applied.\n");
                    fineafterwaiver = totalfine;
                }
            } else {
                fineafterwaiver = totalfine;
            }
        } 
        else if (choice == 5) {
            displayMemberRecord(name, studentid, membertype, borrowedbookids, borrowedcount, totalfine, fineafterwaiver);
        } 
        else if (choice == 6) {
            printf("Exiting program. Thank you!\n");
        } 
        else {
            printf("Inavlid choice. Please try again.\n");
        }

    } while (choice != 6);

    return 0;
}
