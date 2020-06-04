#include <stdlib.h>  // malloc()
#include <string.h>  // memcpy(), strcmp()

#define SIZE 256

typedef struct _node {
    struct _node *prev;
    char key  [SIZE];
    char value[SIZE];
    struct _node *next;
} *map;

void init(map *head);
void set(char* key, char* value, map *head);
void del(char* key, map *head);
map  get(char* key, map head);

void init(map *head) {
    *head = NULL;
}

void set(char* key, char* value, map *head) {
    map node;
    if ((node = get(key, *head))) {
        strcpy(node->value, value);
        return;
    }

    if (!(*head)) {
        *head = node  = (struct _node*) malloc(sizeof(struct _node));
        (*head)->prev = NULL;
    }
    else {
        while (node->next) node = node->next;
        node->next       = (struct _node*) malloc(sizeof(struct _node));
        node->next->prev = node;
        node             = node->next;
    }

    strcpy(node->key,   key);
    strcpy(node->value, value);
    node->next = NULL;
}

void del(char* key, map *head) {
    if (!(*head))
        return;
    map node;
    if ((node = get(key, *head))) {
        if (node == *head) *head = (*head)->next;
        if (node->next) node->next->prev = node->prev;
        if (node->prev) node->prev->next = node->next;
        free(node);
    }
}

map get(char* key, map head) {
    while (head) {
        if (strcmp(head->key, key) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}
