#ifndef TENANT_MATRIX_H
#define TENANT_MATRIX_H

typedef struct TenantNode TenantNode;
typedef struct MatrixMap MatrixMap;
MatrixMap* init_matrix_map();
void secure_insert_doc(MatrixMap* m, int tenant_id, int doc_id, const char* data);
void compile_tenant_boundary(MatrixMap* m, int tenant_id, char* output_buffer);
void free_matrix_map(MatrixMap* m);

#endif
