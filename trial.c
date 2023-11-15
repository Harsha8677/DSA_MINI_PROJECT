#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book {
    int id;
    char title[100];
    char author[100];
    struct Book* next;
} Book;

typedef struct User {
    int id;
    char name[100];
    Book* borrowedBooks;
    struct User* next;
    struct User* prev;
} User;

typedef struct Queue {
    int bookId;
    struct Queue* next;
} Queue;

typedef struct Review {
    char text[500];
	char bn[500];
    struct Review* next;
} Review;

typedef struct Stack {
    Review* review;
    struct Stack* next;
} Stack;

Book* bookRecords = NULL; 
User* userRecords = NULL; 
Stack* bookReviews = NULL; 
Queue* bookQueue = NULL; 



Book* create_book(int id, const char* title, const char* author) {
    Book* book = (Book*)malloc(sizeof(Book));
    book->id = id;
    strncpy(book->title, title, sizeof(book->title));
    strncpy(book->author, author, sizeof(book->author));
    book->next = NULL;
    return book;
}



void addBook(Book** head, Book* new_book) {
    if (*head == NULL || (*head)->id > new_book->id) {
        new_book->next = *head;
        *head = new_book;
    } else {
        Book* current = *head;
        while (current->next != NULL && current->next->id < new_book->id) {
            current = current->next;
        }
        new_book->next = current->next;
        current->next = new_book;
    }
}


void displayBooks() {
    printf("Book Catalog:\n");
    Book* current = bookRecords;
	if (current == NULL) {
        printf("The library is empty.\n");
	}
    while (current != NULL) {
        printf("ID: %d\n", current->id);
		printf("Title: %s", current->title);
		printf("Author: %s\n", current->author);
        current = current->next;
    }
}


void deleteBook(Book** head, int id) {
    if (*head == NULL) {
        printf("The library is empty. No books to delete.\n");
        return;
    }
    Book* current = *head;
    Book* prev = NULL;

    while (current != NULL && current->id != id) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Book with ID %d not found.\n", id);
        return;
    }

    if (prev == NULL) {
        *head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    printf("Book with ID %d has been deleted.\n", id);
}


void addUser(int id, const char* name) {
    User* newUser = (User*)malloc(sizeof(User));
    newUser->id = id;
    strcpy(newUser->name, name);
    newUser->borrowedBooks = NULL;
    newUser->next = userRecords;
    newUser->prev = NULL;
    if (userRecords != NULL) {
        userRecords->prev = newUser;
    }
    userRecords = newUser;
    printf("User added successfully!\n");
}



void displayUsers() {
    printf("User Records:\n");
    User* current = userRecords;
    while (current != NULL) {
        printf("ID: %d, Name: %s\n", current->id, current->name);
        if (current->borrowedBooks != NULL) {
            printf("  Borrowed Books:\n");
            Book* borrowedBook = current->borrowedBooks;
            while (borrowedBook != NULL) {
                printf("  - ID: %d, Title: %s\n", borrowedBook->id, borrowedBook->title);
                borrowedBook = borrowedBook->next;
            }
        }
        current = current->next;
    }
}



void borrowBook(int userId, int bookId) {
    User* currentUser = userRecords;
    while (currentUser != NULL && currentUser->id != userId) {
        currentUser = currentUser->next;
    }

    if (currentUser == NULL) {
        printf("User not found with ID: %d\n", userId);
        return;
    }

    Book* currentBook = bookRecords;
    while (currentBook != NULL && currentBook->id != bookId) {
        currentBook = currentBook->next;
    }

    if (currentBook == NULL) {
        printf("Book not found with ID: %d\n", bookId);
        return;
    }

    Book* borrowedBook = currentUser->borrowedBooks;
    while (borrowedBook != NULL) {
        if (borrowedBook->id == bookId) {
            printf("The user already borrowed this book.\n");
            return;
        }
        borrowedBook = borrowedBook->next;
    }

    currentBook->next = currentUser->borrowedBooks;
    currentUser->borrowedBooks = currentBook;
    printf("Book with ID %d has been successfully borrowed by User with ID %d.\n", bookId, userId);
}




void returnBook(int userId, int bookId) {

    User* currentUser = userRecords;
    while (currentUser != NULL && currentUser->id != userId) {
        currentUser = currentUser->next;
    }

    if (currentUser == NULL) {
        printf("User not found with ID: %d\n", userId);
        return;
    }

    Book* borrowedBook = currentUser->borrowedBooks;
    Book* prevBook = NULL;
    while (borrowedBook != NULL && borrowedBook->id != bookId) {
        prevBook = borrowedBook;
        borrowedBook = borrowedBook->next;
    }

    if (borrowedBook == NULL) {
        printf("User with ID %d has not borrowed a book with ID %d.\n", userId, bookId);
        return;
    }
  
    if (prevBook == NULL) {
        currentUser->borrowedBooks = borrowedBook->next;
    } else {
        prevBook->next = borrowedBook->next;
    }

    printf("Book with ID %d has been successfully returned by User with ID %d.\n", bookId, userId);
}




void addReservation(int bookId) {
    Queue* newNode = (Queue*)malloc(sizeof(Queue));
    newNode->bookId = bookId;
    newNode->next = NULL;
    
    if (bookQueue == NULL) {
        bookQueue = newNode;
    } else {
        Queue* current = bookQueue;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    
    printf("Book reservation added successfully!\n");
}




int removeReservation() {
    if (bookQueue == NULL) {
        printf("No book reservations found!\n");
        return -1; // Error code for no reservations
    }
    
    int bookId = bookQueue->bookId;
    Queue* temp = bookQueue;
    bookQueue = bookQueue->next;
    free(temp);
    
    return bookId;
}



void giveReview(const char* text, const char* book) {
    Book* currentBook = bookRecords;
    int bookFound = 0;

    while (currentBook != NULL) {
        if (strcmp(currentBook->title, book) == 0) {
            bookFound = 1;
            break;
        }
        currentBook = currentBook->next;
    }

    if (!bookFound) {
        printf("Book '%s' not found in the library. Cannot add review.\n", book);
        return;
    }

    Review* newReview = (Review*)malloc(sizeof(Review));
    strcpy(newReview->text, text);
    strcpy(newReview->bn, book);
    newReview->next = NULL;

    Stack* newNode = (Stack*)malloc(sizeof(Stack));
    newNode->review = newReview;
    newNode->next = bookReviews;
    bookReviews = newNode;

    printf("Review for book '%s' added successfully!\n", book);
}




void displayBookReviews() {
    printf("Book Reviews:\n");
    Stack* current = bookReviews;
    while (current != NULL) {
        printf("%s", current->review->bn);
		printf("%s\n", current->review->text);
        current = current->next;
    }
}




int main() {
    int choice;
	printf("\nWELCOME TO THE E-LIBRARY\n");
    while (1) {
        printf("\nE-Library System Menu:\n");
        printf("1.  Add a Book\n");
        printf("2.  Display Books\n");
		printf("3.  Delete a Book\n");
        printf("4.  Add a User\n");
        printf("5.  Display Users\n");
        printf("6.  Borrow a Book\n");
        printf("7.  Return the Book\n");
		printf("8.  Add Reservation\n");
        printf("9.  Remove Reservation\n");
        printf("10. Give Book Review\n");
        printf("11. Display Book Reviews\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
				printf("\n");
                int id;
                char title[100];
                char author[100];
                printf("Enter Book ID: ");
                scanf("%d", &id);
                printf("Enter Book Title: ");
                getchar();
                fgets(title, sizeof(title), stdin);
                printf("Enter Book Author: ");
                fgets(author, sizeof(author), stdin);
				addBook(&bookRecords, create_book(id, title, author));
                break;

            case 2:
			    printf("\n");
                displayBooks();
                break;
				
			case 3:
				printf("\n");
				printf("Enter Book ID: ");
                scanf("%d", &id);
				deleteBook(&bookRecords, id);
				break;

            case 4:
				printf("\n");
                int userId;
                char userName[100];
                printf("Enter User ID: ");
                scanf("%d", &userId);
                printf("Enter User Name: ");
                scanf("%s", userName);
                addUser(userId, userName);
                break;

            case 5:
				printf("\n");
                displayUsers();
                break;

            case 6:
				printf("\n");
                int userBorrowId;
                int bookBorrowId;
                printf("Enter User ID: ");
                scanf("%d", &userBorrowId);
                printf("Enter Book ID: ");
                scanf("%d", &bookBorrowId);
                borrowBook(userBorrowId, bookBorrowId);
                break;

            case 7:
				printf("\n");
                int userReturnId;
                int bookReturnId;
                printf("Enter User ID: ");
                scanf("%d", &userReturnId);
                printf("Enter Book ID: ");
                scanf("%d", &bookReturnId);
                returnBook(userReturnId, bookReturnId);
                break;
				
			 case 8:
				printf("\n");
                int bookReserveId;char bn[50];
				printf("Enter Book Name to reserve: ");
				scanf("%s",bn);
                printf("Enter Book ID to reserve: ");
                scanf("%d", &bookReserveId);
                addReservation(bookReserveId);
                break;
            
            case 9:
				printf("\n");
                int reservedBookId = removeReservation();
                if (reservedBookId != -1) {
                    printf("Reserved Book ID %d has been dequeued.\n", reservedBookId);
                }
                break;

            case 10:
				printf("\n");
                char reviewText[500]; char book[500];
				printf("Enter Book Name: ");
                getchar();
                fgets(book, sizeof(book), stdin);
                printf("Enter Book Review: ");
                fgets(reviewText, sizeof(reviewText), stdin);
                giveReview(reviewText,book);
                break;

            case 11:
				printf("\n");
                displayBookReviews();
                break;

            case 12:
				printf("\nTHANK YOU FOR VISITING\n\n");
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}