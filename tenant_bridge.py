import ctypes
import os
import sys

class TenantBridge:
    def __init__(self):
        if not os.path.exists("./libmatrix.so") and not os.path.exists("./libmatrix.dll"):
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o libmatrix.dll tenant_matrix.c")
                lib_path = "./libmatrix.dll"
            else:
                os.system("gcc -shared -fPIC -o libmatrix.so tenant_matrix.c")
                lib_path = "./libmatrix.so"
        else:
            lib_path = "./libmatrix.dll" if sys.platform.startswith("win") else "./libmatrix.so"

        self.lib = ctypes.CDLL(lib_path)
        self.lib.init_matrix_map.restype = ctypes.c_void_p
        self.lib.secure_insert_doc.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_char_p]
        self.lib.compile_tenant_boundary.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p]
        self.lib.free_matrix_map.argtypes = [ctypes.c_void_p]
        
        self.matrix_ptr = self.lib.init_matrix_map()

    def upload_isolated_document(self, company_id: int, reference_id: int, content: str):
        self.lib.secure_insert_doc(self.matrix_ptr, company_id, reference_id, content.encode('utf-8'))

    def gather_isolated_knowledge(self, company_id: int) -> str:
        buf = ctypes.create_string_buffer(4096)
        self.lib.compile_tenant_boundary(self.matrix_ptr, company_id, buf)
        return buf.value.decode('utf-8')
