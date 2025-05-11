#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct bookmanagementsystem {
	int isbn;
	char title[100];
	char author[100];
	float price;
	struct bookmanagementsystem* left;
	struct bookmanagementsystem* right;
}book;
int main() {
	book* root = NULL;
	int choice;
	int isbn;
	char title[100], author[100];
	float price;

	while (1) {
		printf("Library Book Management System.\n");
		printf("1. Add Book.\n");
		printf("2. Search book by ISBN.\n");
		printf("3. Display all Books.\n");
		printf("4. Delete Book.\n");
		printf("5. Exit.\n");
		printf("Enter your Choice.\n");
		scanf("%d", &choice);
		getchar();

		switch (choice) {
		case 1:
			printf("Enter ISBN Number.\n");
			scanf("%d", &isbn);
			getchar();
			printf("Enter Book Title.\n");
			fgets(title, sizeof(title), stdin);
			title[strcspn(title, "\n")] = '\0';
			printf("Enter Author Name.\n");
			fgets(author, sizeof(author), stdin);
			author[strcspn(author, "\n")] = '\0';
			printf("Enter price of book.\n");
			scanf("%f", &price);
			root = insertbook(root, isbn, title, author, price);
			printf("Book added successfully.\n");
			break;

		case 2:
			printf("Enter ISBN number to be searched.\n");
			scanf("%d", &isbn);
			{
				book* found = searchbook(root, isbn);
				if (found)
					printf("Book found successfully.\n");
				else
					printf("Book not in record.\n");
			}
			break;

		case 3:
			printf("\n--- List of Books (Sorted by ISBN) ---\n");
			inorderTraversal(root);
			break;

		case 4:
			printf("Enter ISBN to delete.\n");
			scanf("%d", &isbn);
			root = deletebook(root, isbn);
			printf("Book Successfully deleted.\n");
			break;

		case 5:
			freetree(root);
			printf("Exiting Program.\n");
			return 0;

		default:
			printf("Invalid Choice.\n");

		}

	}
}

book* createbook(int isbn, const char* title, const char* author, float price) {
	book* newbook = (book*)malloc(sizeof(book));
	if (newbook == NULL) {
		printf("Memory not Allocated.\n");
		return NULL;
	}

	newbook->isbn = isbn;
	strcpy(newbook->title, title);
	strcpy(newbook->author, author);
	newbook->price = price;
	newbook->right = NULL;
	newbook->left = NULL;
	return newbook;
}

book* insertbook(book* root, int isbn, const char* title, const char* author, float price) {
	if (root == NULL) {
		return createbook(isbn, title, author, price);
	}
	else if (isbn < root->isbn) {
		root->left = insertbook(root->left, isbn, title, author, price);
	}
	else if (isbn > root->isbn) {
		root->right = insertbook(root->right, isbn, title, author, price);
	}
	else {
		printf("Book with the same ISBN already exists.\n");
	}
	return root;
}

book* searchbook(book* root, int isbn) {
	if (root == NULL || root->isbn == isbn) {
		return root;
	}
	if (isbn < root->isbn) {
		return searchbook(root->left, isbn);
	}
	else if (isbn > root->isbn) {
		return searchbook(root->right, isbn);
	}
}

void inordersort(book* root) {
	if (root == NULL) {
		return;
	}
	inordersort(root->left);
	printf("ISBN: %d, Title: %s, Author: %s, Price: %.2f\n",
		root->isbn, root->title, root->author, root->price);
	inordersort(root->right);
}

book* findMin(book* root) {
	while (root && root->left != NULL) {
		root = root->left;
	}
	return root;
}
book* deletebook(book* root, int isbn) {
	if (root == NULL) {
		return root;
	}

	if (isbn < root->isbn) {
		root->left = deletebook(root->left, isbn);  // Search in left subtree
	}
	else if (isbn > root->isbn) {
		root->right = deletebook(root->right, isbn);  // Search in right subtree
	}
	else {
		// Node found. Searching for how many children there will be 
		// Case 1: No child
		if (root->left == NULL && root->right == NULL) {
			free(root);
			return NULL;
		}
		// Case 2: One child
		else if (root->left == NULL) {
			book* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL) {
			book* temp = root->left;
			free(root);
			return temp;
		}
		// Case 3: Two children
		else {
			book* temp = findMin(root->right);  // Find successor
			// Copy successor's data to current node
			root->isbn = temp->isbn;
			strcpy(root->title, temp->title);
			strcpy(root->author, temp->author);
			root->price = temp->price;
			// Delete the successor
			root->right = deletebook(root->right, temp->isbn);
		}
	}
	return root;
}
void freetree(book* root) {
	if (root == NULL) {
		return;
	}
	// First free left and right subtrees (post-order)
	freetree(root->left);
	freetree(root->right);

	// Then free the current node
	free(root);
}
