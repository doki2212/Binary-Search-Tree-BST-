#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------------- Structures -------------------------

typedef struct {
    int lqi;
    int battery;
    int status; // 1 = OK, 0 = busy/offline
} NodeMetrics;

typedef struct DecisionNode {
    int is_leaf;
    char route[10]; // used if leaf
    int (*condition_fn)(NodeMetrics);
    struct DecisionNode* left;
    struct DecisionNode* right;
} DecisionNode;

// ------------------- Condition Functions ------------------

int lqi_above_200(NodeMetrics m) { return m.lqi > 200; }
int battery_above_30(NodeMetrics m) { return m.battery > 30; }
int status_ok(NodeMetrics m) { return m.status == 1; }

// ----------------- Tree Node Builders ---------------------

DecisionNode* create_route_node(const char* route) {
    DecisionNode* node = (DecisionNode*)malloc(sizeof(DecisionNode));
    node->is_leaf = 1;
    strcpy(node->route, route);
    node->left = node->right = NULL;
    return node;
}

DecisionNode* create_decision_node(int (*cond)(NodeMetrics), DecisionNode* left, DecisionNode* right) {
    DecisionNode* node = (DecisionNode*)malloc(sizeof(DecisionNode));
    node->is_leaf = 0;
    node->condition_fn = cond;
    node->left = left;
    node->right = right;
    return node;
}

// ----------------- Tree Builder (Priority-Based) ---------------------

DecisionNode* build_tree_with_priority(int priority) {
    // Priority: 1 = LQI > Battery > Status
    //           2 = Battery > LQI > Status
    //           3 = Status > LQI > Battery

    if (priority == 1) {
        return create_decision_node(lqi_above_200,
            create_decision_node(battery_above_30,
                create_decision_node(status_ok,
                    create_route_node("Node A"),
                    create_route_node("Node B")
                ),
                create_route_node("Node B")
            ),
            create_route_node("Node C")
        );
    }
    else if (priority == 2) {
        return create_decision_node(battery_above_30,
            create_decision_node(lqi_above_200,
                create_decision_node(status_ok,
                    create_route_node("Node A"),
                    create_route_node("Node B")
                ),
                create_route_node("Node B")
            ),
            create_route_node("Node C")
        );
    }
    else {
        return create_decision_node(status_ok,
            create_decision_node(lqi_above_200,
                create_decision_node(battery_above_30,
                    create_route_node("Node A"),
                    create_route_node("Node B")
                ),
                create_route_node("Node B")
            ),
            create_route_node("Node C")
        );
    }
}

// --------------------- Tree Traversal ----------------------

char* traverse_tree(DecisionNode* node, NodeMetrics m) {
    if (node->is_leaf) return node->route;
    if (node->condition_fn(m))
        return traverse_tree(node->left, m);
    else
        return traverse_tree(node->right, m);
}

// --------------------- User Input & Main --------------------

NodeMetrics get_user_metrics() {
    NodeMetrics m;
    printf("Enter LQI (0–255): ");
    scanf("%d", &m.lqi);
    printf("Enter battery level (0–100): ");
    scanf("%d", &m.battery);
    printf("Enter node status (1=OK, 0=Busy/Offline): ");
    scanf("%d", &m.status);
    return m;
}

int main() {
    printf("=== Decision Tree Routing Selector ===\n");

    printf("Choose routing priority:\n");
    printf("1. LQI > Battery > Status\n");
    printf("2. Battery > LQI > Status\n");
    printf("3. Status > LQI > Battery\n");
    int priority;
    scanf("%d", &priority);

    DecisionNode* root = build_tree_with_priority(priority);
    NodeMetrics metrics = get_user_metrics();

    char* selected_route = traverse_tree(root, metrics);
    printf("System-selected route: %s\n", selected_route);

    printf("Do you want to override it? (y/n): ");
    char override;
    scanf(" %c", &override);

    if (override == 'y' || override == 'Y') {
        char manual_route[10];
        printf("Enter route to override with (e.g., Node A): ");
        scanf("%s", manual_route);
        printf("User-overridden route: %s\n", manual_route);
    }
    else {
        printf("Final route used: %s\n", selected_route);
    }

    return 0;
}
