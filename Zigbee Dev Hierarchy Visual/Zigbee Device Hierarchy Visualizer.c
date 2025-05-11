#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct device {
	char address[20];
	char type[20];
	int depth;
	struct device* parent;
	struct device* children[10];
	int child_count;
}devices;

devices* createDevice(const char* address, const char* type, int depth, devices* parent);
devices* addDevice(devices* root, const char* parentAddr, const char* newAddr, const char* type);
devices* searchDevice(devices* root, const char* address);
void preorderTraversal(devices* root, int level);
void freeNetwork(devices* root);

int main() {
	devices* root = NULL;
	int choice;
	char addr[20], parentAddr[20], type[20];

	while (1) {
		printf("Zigbee Tree Visualization.\n");
		printf("1. Add Coordinator\n");
		printf("2. Add Router or End Device\n");
		printf("3. Show Network (Preorder)\n");
		printf("4. Search for a Device\n");
		printf("5. Exit\n");
		printf("Enter your choice.\n");
		scanf("%d", &choice);
		getchar();
		switch (choice) {
		case 1:
			if (root != NULL) {
				printf("Coordinator already exists.\n");
				break;
			}
			printf("Enter Coordinator address.\n");
			fgets(addr, sizeof(addr), stdin);
			addr[strcspn(addr, "\n")] = '\0';
			root = createDevice(addr, "Coordinator", 0, NULL);
			printf("Coordinator added.\n");
			break;

		case 2:
			if (root == NULL) {
				printf("Add Coordinator First.\n");
				break;
			}
			printf("Enter parent device address.\n");
			fgets(parentAddr, sizeof(parentAddr), stdin);
			parentAddr[strcspn(parentAddr, "\n")] = '\0';

			printf("Enter address of device.\n");
			fgets(addr, sizeof(addr), stdin);
			addr[strcspn(addr, "\n")] = '\0';

			printf("Enter device type.\n");
			fgets(type, sizeof(type), stdin);
			type[strcspn(type, "\n")] = '\0';

			devices* added = addDevice(root, parentAddr, addr, type);
			if (added) {
				printf("%s %s added under %s.\n", type, addr, parentAddr);
			}
			else {
				printf("Device addition failed. Check device limit.\n");
			}
			break;
		case 3:
			if (root == NULL) {
				printf("Network is empty.\n");
				break;
			}
			else {
				printf("Preorder Traversal.\n");
				preorderTraversal(root, 0);
			}
			break;
		case 4:
			if (root == NULL) {
				printf("Network is empty.\n");
				break;
			}
			printf("Enter address to be searched.\n");
			fgets(addr, sizeof(addr), stdin);
			addr[strcspn(addr, "\n")] = '\0';
			devices* found = searchDevice(root, addr);
			if (found) {
				printf("Device Found:\n");
				printf("  Address: %s\n", found->address);
				printf("  Type: %s\n", found->type);
				printf("  Depth: %d\n", found->depth);
				if (found->parent) {
					printf("Parent's address is %s.\n", found->parent->address);
				}
			}
			else {
				printf("Device not found.\n");
			}
			break;
		case 5:
			freeNetwork(root);
			printf("Exiting...\n");
			return 0;

		default:
			printf("Invalid choice.\n");
		}
	}
}

devices* createDevice(const char* address, const char* type, int depth, devices* parent) {
	devices* newdevice = (devices*)malloc(sizeof(devices));
	if (newdevice == NULL) {
		printf("Memory allocation failed.\n");
		return NULL;
	}
	strcpy(newdevice->address, address);
	strcpy(newdevice->type, type);
	newdevice->depth = depth;
	newdevice->parent = parent;
	newdevice->child_count = 0;
	for (int i = 0; i < 10; i++) {
		newdevice->children[i] = NULL;
	}
	return newdevice;
}

devices* searchDevice(devices* root, const char* address) {
	if (root == NULL) {
		printf("No devices found.\n");
		return NULL;
	}
	if (strcmp(root->address, address) == 0) {
		return root;
	}
	for (int i = 0;i < root->child_count;i++) {
		devices* found = searchDevice(root->children[i], address);
		if (found != NULL) {
			return found;
		}
	}
	return NULL;
}

devices* addDevice(devices* root, const char* parentAddr, const char* newAddr, const char* type) {
	devices* parent = searchDevice(root, parentAddr);
	if (parent == NULL) {
		printf("Parent Not found.\n");
		return NULL;
	}
	if (parent->child_count >= 10) {
		printf("Max limit reached.\n");
		return NULL;
	}
	int depth = parent->depth + 1;
	devices* newDevice = createDevice(newAddr, type, depth, parent);
	if (newDevice != NULL) {
		parent->children[parent->child_count++] = newDevice;
	}
	return newDevice;
}

void preorderTraversal(devices* root, int level) {
	if (root == NULL) return;

	// Indentation for tree-like structure
	for (int i = 0; i < level; i++) {
		printf("  ");
	}
	printf("- %s (%s, Depth: %d)\n", root->address, root->type, root->depth);

	for (int i = 0; i < root->child_count; i++) {
		preorderTraversal(root->children[i], level + 1);
	}
}

void freeNetwork(devices* root) {
	if (root == NULL) return;

	for (int i = 0; i < root->child_count; i++) {
		freeNetwork(root->children[i]);
	}
	free(root);
}