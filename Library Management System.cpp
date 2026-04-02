#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 1007

typedef struct Book {
    char id[20];
    char title[51];
    char author[26];
    char isbn[14];
    int pages;
    struct Book *next;
} Book;

Book *hashTable[TABLE_SIZE];

int hash(char *id) {
    int sum = 0;
    for (int i = 0; id[i] != '\0'; i++) {
        sum += id[i];
    }
    return sum % TABLE_SIZE;
}

void insertBook() {
    Book *newBook = (Book*) malloc(sizeof(Book));
    if (newBook == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    printf("Enter Book Title (between 5 and 50 characters): ");
    fgets(newBook->title, sizeof(newBook->title), stdin);
    newBook->title[strcspn(newBook->title, "\n")] = 0;
    if (strlen(newBook->title) < 5 || strlen(newBook->title) > 50) {
        printf("Book Title must be between 5 and 50 characters.\n");
        free(newBook);
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        Book *temp = hashTable[i];
        while (temp != NULL) {
            if (strcmp(temp->title, newBook->title) == 0) {
                printf("Book Title must be unique.\n");
                free(newBook);
                return;
            }
            temp = temp->next;
        }
    }

    printf("Enter Book Author (start with 'Mr.' or 'Mrs.' between 3 and 25 characters): ");
    fgets(newBook->author, sizeof(newBook->author), stdin);
    newBook->author[strcspn(newBook->author, "\n")] = 0;
    if (strncmp(newBook->author, "Mr. ", 4) != 0 && strncmp(newBook->author, "Mrs. ", 5) != 0) {
        printf("Book Author must start with 'Mr.' or 'Mrs.'.\n");
        free(newBook);
        return;
    }
    if (strlen(newBook->author) < 3 || strlen(newBook->author) > 25) {
        printf("Book Author must be between 3 and 25 characters.\n");
        free(newBook);
        return;
    }

    printf("Enter ISBN (numeric between 10 and 13 digits): ");
    fgets(newBook->isbn, sizeof(newBook->isbn), stdin);
    newBook->isbn[strcspn(newBook->isbn, "\n")] = 0;
    for (int i = 0; newBook->isbn[i] != '\0'; i++) {
        if (!isdigit(newBook->isbn[i])) {
            printf("ISBN must be numeric.\n");
            free(newBook);
            return;
        }
    }
    if (strlen(newBook->isbn) < 10 || strlen(newBook->isbn) > 13) {
        printf("ISBN must be between 10 and 13 digits.\n");
        free(newBook);
        return;
    }

    printf("Enter Page Number (at least 16): ");
    scanf("%d", &newBook->pages);
    getchar();
    if (newBook->pages < 16) {
        printf("Page Number must be at least 16.\n");
        free(newBook);
        return;
    }

    int lastId = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book *temp = hashTable[i];
        while (temp != NULL) {
            lastId++;
            temp = temp->next;
        }
    }
    char authorInitial = toupper(newBook->author[4]);
    char titleInitial = toupper(newBook->title[0]);
    sprintf(newBook->id, "B%05d-%s-%c%c", lastId + 1, newBook->isbn, authorInitial, titleInitial);

    // Insert into hash table
    int key = hash(newBook->id);
    newBook->next = hashTable[key];
    hashTable[key] = newBook;

    printf("Book inserted successfully!\n");
}

void viewBook() {
    int isEmpty = 1;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book *temp = hashTable[i];
        while (temp != NULL) {
            isEmpty = 0;
            printf("ID: %s\nTitle: %s\nAuthor: %s\nISBN: %s\nPages: %d\n\n", temp->id, temp->title, temp->author, temp->isbn, temp->pages);
            temp = temp->next;
        }
    }
    if (isEmpty) {
        printf("There is no book(s)!\n");
    }
}

void removeBook() {
    char id[20];
    printf("Enter Book ID to remove: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int key = hash(id);
    Book *current = hashTable[key];
    Book *prev = NULL;
    while (current != NULL && strcmp(current->id, id) != 0) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Book not found.\n");
        return;
    }

    printf("Book found:\nID: %s\nTitle: %s\nAuthor: %s\nISBN: %s\nPages: %d\n", current->id, current->title, current->author, current->isbn, current->pages);

    char confirm;
    printf("Are you sure you want to remove this book? (y/n): ");
    scanf(" %c", &confirm);
    if (confirm == 'y') {
        if (prev == NULL) {
            hashTable[key] = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
        printf("Book removed successfully!\n");
    } else if (confirm == 'n') {
        printf("Removal cancelled.\n");
    } else {
        printf("Invalid input.\n");
    }
}

int main() {
    int choice;
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    while (1) {
        printf("\nBluejack Library Menu:\n");
        printf("1. View Book\n");
        printf("2. Insert Book\n");
        printf("3. Remove Book\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        puts(" ");
        getchar();

        switch (choice) {
            case 1:
                viewBook();
                break;
            case 2:
                insertBook();
                break;
            case 3:
                removeBook();
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
        }
    }

    return 0;
}