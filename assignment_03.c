/*
* LeetCode Min Stack
* https://leetcode.com/problems/min-stack/
*/
struct node {
    int data;
    struct node* prev;
};
typedef struct node Node;

typedef struct {
    Node* last;
    Node* min_last;
} MinStack;


MinStack* minStackCreate() {
    MinStack* min_stack = (MinStack*)malloc(sizeof(MinStack));
    min_stack->last = NULL;
    min_stack->min_last = NULL;
    return min_stack;
}

void minStackPush(MinStack* obj, int val) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = val;

    if(obj->last == NULL){
       obj->last = new_node; 
        new_node->prev = NULL;
    } else {
        new_node->prev = obj->last;
    }
    obj->last = new_node;
    if(obj->min_last == NULL) {
        Node* new_min_node = (Node*)malloc(sizeof(Node));
        new_min_node->data = val;
        new_min_node->prev = NULL;
        obj->min_last = new_min_node;
    } else {
        if( val <= obj->min_last->data) {
            Node* new_min_node = (Node*)malloc(sizeof(Node));
            new_min_node->data = val;
            new_min_node->prev = obj->min_last;
            obj->min_last = new_min_node;
        }
    }

}

void minStackPop(MinStack* obj) {
    if(obj->last->data == obj->min_last->data) {
        Node* min_prev = obj->min_last->prev;
        free(obj->min_last);
        obj->min_last = min_prev;
    }
    Node* prev = obj->last->prev;
    free(obj->last);
    obj->last = prev;

}

int minStackTop(MinStack* obj) {
    return obj->last->data;
}

int minStackGetMin(MinStack* obj) {
    return obj->min_last->data;
}

void minStackFree(MinStack* obj) {
    free(obj);
}

/**
 * Your MinStack struct will be instantiated and called as such:
 * MinStack* obj = minStackCreate();
 * minStackPush(obj, val);
 
 * minStackPop(obj);
 
 * int param_3 = minStackTop(obj);
 
 * int param_4 = minStackGetMin(obj);
 
 * minStackFree(obj);
*/
