#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct studentcourseenrollment {
	int roll;
	char name[100];
	char dept[100];
	int no_course;
	char courses[5][50];
	struct studentcourseenrollment* left;
	struct studentcourseenrollment* right;
}student;

int main(){
	student* root = NULL;
	int choice;
	int roll;
	char name[100];
	char dept[100];
	int no_course;
	char courses[5][50];

	while (1) {
		printf("Student Course Enrollment.\n");
		printf("1. Add Student Details.\n");
		printf("2. Search Student.\n");
		printf("3. Update Student.\n");
		printf("4. Display all students.\n");
		printf("5. Delete Student.\n");
		printf("6. Exit.\n");
		printf("Enter your choice.\n");
		scanf("%d", &choice);
		getchar();

		switch (choice) {
		case 1: 
			printf("Enter Roll Number.\n");
			scanf("%d", &roll);
			getchar();
			printf("Enter Name of Student.\n");
			fgets(name, sizeof(name), stdin);
			name[strcspn(name, "\n")] = '\0';
			printf("Enter Department of the student.\n");
			fgets(dept, sizeof(dept), stdin);
			dept[strcspn(dept, "\n")] = '\0';
			printf("Enter Number of courses.\n");
			scanf("%d", &no_course);
			getchar();
			printf("Enter Courses (one per line):\n");
			for (int i = 0; i < no_course; i++) {
				printf("Course %d: ", i + 1);
				fgets(courses[i], sizeof(courses[i]), stdin);
				courses[i][strcspn(courses[i], "\n")] = '\0'; // Remove newline
			}

			root = insertstudent(root, roll, name, dept, no_course, courses);
			printf("Student Enrolled.\n");
			break;

		case 2:
			printf("Enter Roll Number you want to search.\n");
			scanf("%d", &roll);
			{
				student* found = searchstudent(root, roll);
				if (found) {
					printf("Student found:\n");
					printf("Roll Number: %d\n", found->roll);
					printf("Name: %s\n", found->name);
					printf("Department: %s\n", found->dept);
					printf("Number of Courses: %d\n", found->no_course);
					printf("Courses:\n");
					for (int i = 0; i < found->no_course; i++) {
						printf("  %s\n", found->courses[i]);
					}
				}
				else {
					printf("Student not found.\n");
				}
			}
			break;

		case 3:
			printf("Enter Roll Number of Student whose deails need to be Edited.\n");
			scanf("%d", &roll);
			getchar();
			{
				student* update = updatestudent(root, roll);
				if (update) {
					printf("Student details changed. Updated details are as follows:\n");
					printf("Roll Number: %d\n", update->roll);
					printf("Name: %s\n", update->name);
					printf("Department: %s\n", update->dept);
					printf("Number of Courses: %d\n", update->no_course);
					printf("Courses:\n");
					for (int i = 0; i < update->no_course; i++) {
						printf("  %s\n", update->courses[i]);
					}
				}
				else {
					printf("Student not found.\n");
				}
			}
			break;

		case 4:
			printf("List of students.\n");
			inordersort(root);
			break;

		case 5:
			printf("Enter Roll Number of the Student whose data has to be removed.\n ");
			scanf("%d", &roll);
			root = deletestudent(root, roll);
			printf("Student Successfully deleted.\n");
			break;

		case 6:
			freetree(root);
			printf("Exiting Program.\n");
			return 0;

		default:
			printf("Invalid Choice.\n");

		}
	}
}

student* createstudent(int roll, const char* name, const char* dept, int no_course, char courses[5][50]) {
	student* newstudent = (student*)malloc(sizeof(student));
	if (newstudent == NULL) {
		printf("Memory Not Allocated.\n");
		return NULL;
	 }
	newstudent->roll = roll;
	strcpy(newstudent->name, name);
	strcpy(newstudent->dept, dept);
	newstudent->no_course = no_course;
	for (int i = 0; i < no_course; i++) {
		strcpy(newstudent->courses[i], courses[i]);
	}
	newstudent->left = NULL;
	newstudent->right = NULL;
	return newstudent;
}

student* insertstudent(student* root, int roll, const char* name, const char* dept, int no_course, char courses[5][50]) {
	if (root == NULL) {
		return createstudent(roll, name, dept, no_course, courses);
	}
	else if (roll < root->roll) {
		root->left = insertstudent(root->left, roll, name, dept, no_course, courses);
	}
	else if (roll > root->roll) {
		root->right = insertstudent(root->right, roll, name, dept, no_course, courses);
	}
	return root;
}

student* searchstudent(student* root, int roll) {
	if (root == NULL || root->roll == roll) {
		return(root);
	}
	else if (roll < root->roll) {
		return searchstudent(root->left, roll);
	}
	else if (roll > root->roll) {
		return searchstudent(root->right, roll);
	}
}

void inordersort(student* root) {
	if (root == NULL) {
		return;
	}
	inordersort(root->left);
	printf("Roll Number   : %d\n", root->roll);
	printf("Name          : %s\n", root->name);
	printf("Department    : %s\n", root->dept);
	printf("Courses       :\n");
	for (int i = 0; i < root->no_course; i++) {
		printf("  - %s\n", root->courses[i]);
	}
	inordersort(root->right);
 }

student* findMin(student* root) {
	while (root && root->left != NULL) {
		root = root->left;
	}
	return root;
}

student* deletestudent(student* root, int roll) {
	if (root == NULL) {
		return root;
	}
	if (roll < root->roll) {
		root->left = deletestudent(root->left, roll);
	}
	else if (roll > root->roll) {
		root->right = deletestudent(root->right, roll);
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
			student* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL) {
			student* temp = root->left;
			free(root);
			return temp;
		}
		// Case 3: Two children
		else {
			student* temp = findMin(root->right);  // Find successor
			// Copy successor's data to current node
			root->roll = temp->roll;
			strcpy(root->name, temp->name);
			strcpy(root->dept, temp->dept);
			root->no_course = temp->no_course;
			for (int i = 0; i < temp->no_course; i++) {
				strcpy(root->courses[i], temp->courses[i]); // 
			}
			// Delete the successor
			root->right = deletestudent(root->right, temp->roll);
		}
	}
	return root;
}
void freetree(student* root) {
	if (root == NULL) {
		return;
	}
	// First free left and right subtrees (post-order)
	freetree(root->left);
	freetree(root->right);

	// Then free the current node
	free(root);
}