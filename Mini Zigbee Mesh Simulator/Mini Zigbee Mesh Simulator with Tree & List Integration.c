#define SECURE_CRT_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_ROUTES 100

typedef struct Node {
	int id;
	int type;
	int battery;
	struct Node* parent;
	struct Node* left_child;
	struct Node* right_sibling;
	struct Neighbour* neighbour_list;
}Node;

typedef struct Neighbour {
	int neighbour_id;
	int link_quality;
	struct Neighbour* next;
} Neighbour;

typedef struct {
	int source;
	int destination;
	int next_hop;
} RouteEntry;

Node* coordinator = NULL;
int node_counter = 0;
RouteEntry routing_table[MAX_ROUTES];
int route_count = 0;

Node* create_node(int type, int battery);
void attach_to_network(Node* node);
void add_child(Node* parent, Node* child);
void add_neighbour(Node* node, int neighbour_id, int lqi);
int route_exists(Node* source, int dest_id);
int traverse_and_find(Node* node, int dest_id);
void update_routing_table(int src, int dest, int next_hop);
void print_network(Node* root, int depth);
void print_neighbours(Node* node);
void print_routing_table();

Node* create_node(int type, int battery) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->id = node_counter++;
	node->type = type;
	node->battery = battery;
	node->parent = NULL;
	node->left_child = NULL;
	node->right_sibling = NULL;
	node->neighbour_list = NULL;
	return node;
}

void add_child(Node* parent, Node* child) {
	child->parent = parent;
	if (!parent->left_child)
		parent->left_child = child;
	else {
		Node* sibling = parent->left_child;
		while (sibling->right_sibling)
			sibling = sibling->right_sibling;
		sibling->right_sibling = child;
	}
}

void attach_to_network(Node* node) {
	if (!coordinator) {
		coordinator = node;
		return;
	}
	add_child(coordinator, node);
}

void add_neighbour(Node* node, int neighbour_id, int lqi) {
	Neighbour* new_neigh = (Neighbour*)malloc(sizeof(Neighbour));
	new_neigh->neighbour_id = neighbour_id;
	new_neigh->link_quality = lqi;
	new_neigh->next = node->neighbour_list;
	node->neighbour_list = new_neigh;
}

int traverse_and_find(Node* node, int dest_id) {
	if (!node) return 0;
	if (node->id == dest_id) return 1;
	return traverse_and_find(node->left_child, dest_id) ||
		traverse_and_find(node->right_sibling, dest_id);
}

int route_exists(Node* source, int dest_id) {
	Neighbour* n = source->neighbour_list;
	while (n) {
		if (n->neighbour_id == dest_id)
			return 1;
		n = n->next;
	}
	return traverse_and_find(coordinator, dest_id);
}

void update_routing_table(int src, int dest, int next_hop) {
	routing_table[route_count++] = (RouteEntry){ src, dest, next_hop };
}

void print_network(Node* root, int depth) {
	if (!root) return;
	for (int i = 0; i < depth; i++) printf("  ");
	printf("Node %d [Type: %d, Battery: %d]\n", root->id, root->type, root->battery);
	print_network(root->left_child, depth + 1);
	print_network(root->right_sibling, depth);
}

void print_neighbours(Node* node) {
	printf("Node %d Neighbours: ", node->id);
	Neighbour* n = node->neighbour_list;
	while (n) {
		printf("(ID: %d, LQI: %d) ", n->neighbour_id, n->link_quality);
		n = n->next;
	}
	printf("\n");
}

void print_routing_table() {
	for (int i = 0; i < route_count; i++) {
		printf("From %d to %d => Next hop: %d\n",
			routing_table[i].source,
			routing_table[i].destination,
			routing_table[i].next_hop);
	}
}

int main() {
	printf("=== Mini Zigbee Mesh Simulator ===\n");

	Node* router1 = create_node(1, 90);
	Node* router2 = create_node(1, 85);
	Node* end1 = create_node(2, 80);
	Node* end2 = create_node(2, 75);

	attach_to_network(create_node(0, 100)); // Coordinator
	attach_to_network(router1);
	attach_to_network(router2);
	attach_to_network(end1);
	attach_to_network(end2);

	add_neighbour(router1, router2->id, 220);
	add_neighbour(router2, router1->id, 215);
	add_neighbour(router1, end1->id, 200);
	add_neighbour(router2, end2->id, 210);

	printf("\n--- Network Topology ---\n");
	print_network(coordinator, 0);

	printf("\n--- Neighbour Lists ---\n");
	print_neighbours(router1);
	print_neighbours(router2);

	int src_id = end1->id;
	int dest_id = end2->id;

	printf("\nSimulating route from Node %d to Node %d...\n", src_id, dest_id);

	if (route_exists(end1, dest_id)) {
		printf("Route found!\n");
		update_routing_table(src_id, dest_id, router1->id);
	}
	else {
		printf("No route available.\n");
	}

	printf("\n--- Routing Table ---\n");
	print_routing_table();

	return 0;
}
