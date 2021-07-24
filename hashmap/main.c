#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"

#define RESIZE_BY 10
#define BUFF_SIZE 100

typedef struct Node {
    char* val;
    struct Node* next;
} Node;

typedef struct Map {
    Node** root;
    size_t size;
} Map;

size_t hash(const Map* map, const char* val) {
    const char* str = val;
    size_t sum = 0;
    do {
        sum += *str;
    } while (*str++ != '\0');
    return sum % map->size;
}

void addItem(Map* map, const char* val) {
    Node* node = malloc(sizeof(Node));

    const size_t len = strlen(val);
    node->val = malloc(len + 1);
    strncpy(node->val, val, len);
    node->val[len] = '\0';
    node->next = NULL;

    const size_t idx = hash(map, node->val);
    if (map->root[idx] == NULL) {
        map->root[idx] = node;
    } else {
        Node* rowRef = map->root[idx];
        while (rowRef->next != NULL) {
            rowRef = rowRef->next;
        }
        rowRef->next = node;
    }
}

void printMap(const Map* map) {
    for (size_t i=0; i<map->size; ++i) {
        if (map->root[i] != NULL) {
            printf("[%zu] %s ", i, map->root[i]->val);
            Node* next = map->root[i]->next;
            while (next != NULL) {
                printf("%s ", next->val);
                next = next->next;
            }
            printf("\n");
        }
    }
}

/// I don't want to go back at this point and make a back pointer
/// so I'm using recursion to store pointers on a stack.
void freeLinkedList(Node* node) {
    // Guard against unexpected input
    if (node == NULL) {
        return;
    }
    if (node->next != NULL) {
        freeLinkedList(node->next);
    }
    printf("  - '%s'\n", node->val);
    free(node->val);
    free(node);
}

void freeMap(Map* map) {
    for (size_t i=0; i<map->size; ++i) {
        printf("[%zu] Cleaning...\n", i);
        freeLinkedList(map->root[i]);
    }

    free(map);
    printf("\nCleaned all! Good job not relying on the OS.\n");
}

int main(int argc, char** argv) {
    // Init
    Map* map = malloc(sizeof(Map));
    map->root = malloc(sizeof(Node*) * RESIZE_BY);
    map->size = RESIZE_BY;
    for (size_t i=0; i<RESIZE_BY; ++i) {
        map->root[i] = NULL;
    }

    // Prompt
    bool first = true;
    char input[BUFF_SIZE];
    do {
        // Strip the newline
        const size_t buffLen = strlen(input);
        input[buffLen-1] = '\0';
        
        // Boostrap the I/O
        if (!first) {
            addItem(map, input);
            printMap(map);
        }
        // Clear the buffer for the next attack run
        for (size_t i=0; i<BUFF_SIZE; ++i) {
            input[i] = '\0';
        }
        first = false;
        printf("Add item: ");
    } while (fgets(input, BUFF_SIZE, stdin) != NULL);


    printf("#### Cleaning up: ####\n");
    freeMap(map);

    return EXIT_SUCCESS;
}
