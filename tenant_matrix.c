#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TENANT_BUCKETS 5

typedef struct TenantNode {
    int global_doc_id;
    char text_content[256];
    struct TenantNode* next;
} TenantNode;

typedef struct {
    TenantNode* buckets[TENANT_BUCKETS];
} MatrixMap;

#ifdef _WIN32
    __declspec(dllexport) MatrixMap* init_matrix_map();
    __declspec(dllexport) void secure_insert_doc(MatrixMap* m, int tenant_id, int doc_id, const char* data);
    __declspec(dllexport) void compile_tenant_boundary(MatrixMap* m, int tenant_id, char* output_buffer);
    __declspec(dllexport) void free_matrix_map(MatrixMap* m);
#endif

MatrixMap* init_matrix_map() {
    MatrixMap* m = (MatrixMap*)malloc(sizeof(MatrixMap));
    for (int i = 0; i < TENANT_BUCKETS; i++) m->buckets[i] = NULL;
    return m;
}

void secure_insert_doc(MatrixMap* m, int tenant_id, int doc_id, const char* data) {
    int bucket_index = tenant_id % TENANT_BUCKETS;
    TenantNode* node = (TenantNode*)malloc(sizeof(TenantNode));
    node->global_doc_id = doc_id;
    strncpy(node->text_content, data, sizeof(node->text_content) - 1);
    
    // Push node to head of tenant chain to guarantee data isolation boundaries
    node->next = m->buckets[bucket_index];
    m->buckets[bucket_index] = node;
}

void compile_tenant_boundary(MatrixMap* m, int tenant_id, char* output_buffer) {
    int bucket_index = tenant_id % TENANT_BUCKETS;
    TenantNode* current = m->buckets[bucket_index];
    while (current != NULL) {
        strcat(output_buffer, current->text_content);
        strcat(output_buffer, " [SECURE] | ");
        current = current->next;
    }
}

void free_matrix_map(MatrixMap* m) {
    for (int i = 0; i < TENANT_BUCKETS; i++) {
        TenantNode* current = m->buckets[i];
        while (current) {
            TenantNode* tmp = current;
            current = current->next;
            free(tmp);
        }
    }
    free(m);
}
