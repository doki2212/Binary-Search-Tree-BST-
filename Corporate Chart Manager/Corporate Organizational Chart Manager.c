#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct employee {
	char id[20];
	char name[50];
	char role[30];
	int level;
	struct employee* manager;
	struct employee* report[5];
	int report_count;
}employee;
// Function Declrations
employee* addCEO(const char* id, const char* role,const char* name, int level, employee* manager);		// Add CEO
employee* addemp(employee* root, const char* new_id, const char* parent_id, const char* role, const char* name);		//Add new employee
void displayemployee(employee* root);		// Display just employee names and hierachy
employee* findbyID(employee* root, const char* id);		//find employee by ID
employee* findbyname(employee* root, const char* name); //Find employee by name
void displayemp(employee* root); //Display all employees details with managers and current roles
void delete(employee* root);

int main() {
	employee* root = NULL;
	int choice;
	char id[20], name[50], role[30], parent_id[20];

	while (1) {
		printf("\n=== Corporate Org Chart Manager ===\n");
		printf("1. Add CEO\n");
		printf("2. Add Employee\n");
		printf("3. Display Hierarchy (Tree View)\n");
		printf("4. Find Employee by ID\n");
		printf("5. Find Employee by Name\n");
		printf("6. Display All Employee Details\n");
		printf("7. Exit\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		getchar();

		switch (choice) {
		case 1: 
			if (root != NULL) {
				printf("CEO already exists.\n");
				break;
			}
			printf("Enter CEO ID.\n");
			fgets(id, sizeof(id), stdin);
			id[strcspn(id, "\n")] = '\0';

			printf("Enter CEO Name.\n");
			fgets(name, sizeof(name), stdin);
			name[strcspn(name, "\n")] = '\0';

			printf("Enter Role.\n");
			fgets(role, sizeof(role), stdin);
			role[strcspn(role, "\n")] = '\0';

			root = addCEO(id, role, name, 0, NULL);
			printf("CEO successfully added.\n");
			break;
		case 2:
			if (root == NULL) {
				printf("CEO has not been added.\n");
				break;
			}
			printf("Enter Manager's ID.\n");
			fgets(parent_id, sizeof(parent_id), stdin);
			parent_id[strcspn(parent_id, "\n")] = '\0';

			printf("Enter Employee ID: ");
			fgets(id, sizeof(id), stdin);
			id[strcspn(id, "\n")] = '\0';

			printf("Enter Employee Name: ");
			fgets(name, sizeof(name), stdin);
			name[strcspn(name, "\n")] = '\0';

			printf("Enter Role: ");
			fgets(role, sizeof(role), stdin);
			role[strcspn(role, "\n")] = '\0';

			employee* added = addemp(root, id, parent_id, role, name);
			if (added != NULL) {
				printf("Employee %s has been added under Manager ID %s.\n", name, parent_id);
			}
			else {
				printf("Could not add employee. Either manager not found or report limit reached.\n");
			}
			break;
		case 3:
			if (root == NULL) {
				printf("No employees to display.\n");
			}
			else {
				displayemployee(root);
			}
			break;

		case 4:
			printf("Enter Employee's ID : \n");
			fgets(id, sizeof(id), stdin);
			id[strcspn(id, "\n")] = '\0';
			{
				employee* found = findbyID(root, id);
				if (found) {
					printf("Found: %s (Role: %s)\n", found->name, found->role);
				}
				else {
					printf("Employee not found.\n");
				}
			}
			break;

		case 5:
			printf("Enter Employee's Name : \n");
			fgets(name, sizeof(name), stdin);
			name[strcspn(name, "\n")] = '\0';
			{
				employee* found = findbyname(root, name);
				if (found) {
					printf("Found: %s (ID: %s, Role: %s)\n", found->name, found->id, found->role);
				}
				else {
					printf("Employee not found.\n");
				}
			}
			break;
		case 6:
			displayemp(root);
			break;

		case 7:
			delete(root);
			printf("Exiting program. Memory freed.\n");
			return 0;

		default:
			printf("Invalid choice. Try again.\n");
		}
	}
}

employee* addCEO(const char* id, const char* role, const char* name, int level, employee* manager) {
	employee* newemp = (employee*)malloc(sizeof(employee));
	if (newemp == NULL) {
		printf("Memory not allocated.\n");
		return NULL;
	}
	strcpy(newemp->id, id);
	strcpy(newemp->role, role);
	strcpy(newemp->name, name);
	newemp->level = level;
	newemp->manager = manager;
	newemp->report_count = 0;
	for (int i = 0; i < 5; i++) {
		newemp->report[i] = NULL;
	}
	return newemp;
}

employee* findbyID(employee* root, const char* id) {
	if (root == NULL) {
		printf("CEO data not available.\n");
		return NULL;
	}
	if (strcmp(root->id, id) == 0) {
		return root;
	}
	for (int i = 0; i < root->report_count; i++) {
		employee* found = findbyID(root->report[i], id);
		if (found != NULL) {
			printf("Employee is found.\n");
			return found;
		}
	}
	return NULL;
}

employee* findbyname(employee* root, const char* name) {
	if (root == NULL) {
		printf("CEO data not available.\n");
		return NULL;
	}
	if (strcmp(root->name, name) == 0) {
		return root;
	}
	for (int i = 0;i < root->report_count;i++) {
		employee* found=findbyname(root->report[i], name);
		if (found != NULL) {
			return found;
		}
	}
	return NULL;
}

employee* addemp(employee* root, const char* new_id, const char* parent_id, const char* role, const char* name) {
	employee* manager = findbyID(root, parent_id);
	if (manager == NULL) {
		printf("Manager Not Found.\n");
		return NULL;
	}
	if (manager->report_count >= 5) {
		printf("No more employees can be added to this Manager.\n");
		return NULL;
	}
	int level = manager->level + 1;
	employee* newemp = addCEO(new_id, role, name, level, manager);
	if (newemp != NULL) {
		manager->report[manager->report_count++] = newemp;
	}
	return newemp;
}

void displayemployee(employee* root) {
	if (root == NULL) return;

	// Print indentation based on level
	for (int i = 0; i < root->level; i++) {
		printf("  ");
	}
	printf("- %s (%s)\n", root->name, root->role);  // e.g., - Alice (Manager)

	// Recurse for reports
	for (int i = 0; i < root->report_count; i++) {
		displayemployee(root->report[i]);
	}
}

void displayemp(employee* root) {
	if (root == NULL) return;

	printf("\nEmployee Details:\n");
	printf("ID     : %s\n", root->id);
	printf("Name   : %s\n", root->name);
	printf("Role   : %s\n", root->role);
	printf("Level  : %d\n", root->level);
	if (root->manager != NULL) {
		printf("Manager: %s\n", root->manager->name);
	}
	else {
		printf("Manager: None (This is the CEO)\n");
	}

	// Recurse for reports
	for (int i = 0; i < root->report_count; i++) {
		displayemp(root->report[i]);
	}
}

void delete(employee* root) {
	if (root == NULL) return;

	// Recursively delete all direct reports (children)
	for (int i = 0; i < root->report_count; i++) {
		delete(root->report[i]);
	}

	// Then free the current employee node
	free(root);
}
