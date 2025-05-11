#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student {
    int roll;
    char name[100];
    float gpa;
    struct Student* left;
    struct Student* right;
} Student;

// Create a new student node
Student* createStudent(int roll, const char* name, float gpa) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (!newStudent) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    newStudent->roll = roll;
    strcpy(newStudent->name, name);
    newStudent->gpa = gpa;
    newStudent->left = NULL;
    newStudent->right = NULL;
    return newStudent;
}

// Insert a new student into the BST
Student* insertStudent(Student* root, int roll, const char* name, float gpa) {
    if (root == NULL) {
        return createStudent(roll, name, gpa);
    }
    if (roll < root->roll) {
        root->left = insertStudent(root->left, roll, name, gpa);
    }
    else if (roll > root->roll) {
        root->right = insertStudent(root->right, roll, name, gpa);
    }
    else {
        printf("Student with roll number %d already exists.\n", roll);
    }
    return root;
}

// Search for a student by roll number
Student* searchStudent(Student* root, int roll) {
    if (root == NULL || root->roll == roll)
        return root;

    if (roll < root->roll)
        return searchStudent(root->left, roll);
    else
        return searchStudent(root->right, roll);
}

// Inorder Traversal (sorted by roll number)
void inorderTraversal(Student* root) {
    if (root == NULL)
        return;
    inorderTraversal(root->left);
    printf("Roll: %d, Name: %s, GPA: %.2f\n", root->roll, root->name, root->gpa);
    inorderTraversal(root->right);
}

// Find minimum value node (for deletion case)
Student* findMin(Student* node) {
    Student* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// Delete a student by roll number
Student* deleteStudent(Student* root, int roll) {
    if (root == NULL)
        return root;

    if (roll < root->roll) {
        root->left = deleteStudent(root->left, roll);
    }
    else if (roll > root->roll) {
        root->right = deleteStudent(root->right, roll);
    }
    else {
        // Node found
        if (root->left == NULL) {
            Student* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Student* temp = root->left;
            free(root);
            return temp;
        }
        // Two children
        Student* temp = findMin(root->right);
        root->roll = temp->roll;
        strcpy(root->name, temp->name);
        root->gpa = temp->gpa;
        root->right = deleteStudent(root->right, temp->roll);
    }
    return root;
}

// Free all memory
void freeTree(Student* root) {
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Student* root = NULL;
    int choice, roll;
    char name[100];
    float gpa;

    while (1) {
        printf("\n=== Student Record System ===\n");
        printf("1. Add Student\n");
        printf("2. Search Student\n");
        printf("3. Display All Students\n");
        printf("4. Delete Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // clear newline

        switch (choice) {
        case 1:
            printf("Enter Roll Number: ");
            scanf("%d", &roll);
            getchar();
            printf("Enter Name: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0'; // Remove newline
            printf("Enter GPA: ");
            scanf("%f", &gpa);
            root = insertStudent(root, roll, name, gpa);
            printf("Student added successfully.\n");
            break;

        case 2:
            printf("Enter Roll Number to Search: ");
            scanf("%d", &roll);
            {
                Student* found = searchStudent(root, roll);
                if (found)
                    printf("Found: Roll: %d, Name: %s, GPA: %.2f\n", found->roll, found->name, found->gpa);
                else
                    printf("Student not found.\n");
            }
            break;

        case 3:
            printf("\n--- List of Students (Sorted by Roll Number) ---\n");
            inorderTraversal(root);
            break;

        case 4:
            printf("Enter Roll Number to Delete: ");
            scanf("%d", &roll);
            root = deleteStudent(root, roll);
            printf("Student deleted successfully (if existed).\n");
            break;

        case 5:
            freeTree(root);
            printf("Exiting program. Goodbye!\n");
            return 0;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}