from tenant_bridge import TenantBridge

if __name__ == "__main__":
    cloud_vault = TenantBridge()

    # Ingest data from Company 1001 (Enterprise Bank)
    cloud_vault.upload_isolated_document(company_id=1001, reference_id=1, content="Bank Config: Master transactional database operates on port 8443.")

    # Ingest data from Company 2002 (Retail Partner Corp)
    cloud_vault.upload_isolated_document(company_id=2002, reference_id=2, content="Retail Logistics: Public API endpoints expose webhook data parameters via port 8080.")

    print("=== TENANTSHIELD-RAG SEPARATE CHAINING ISOLATION MAP ===")
    
    # Request data for Tenant 1001 only
    target_tenant = 1001
    retrieved_facts = cloud_vault.gather_isolated_knowledge(company_id=target_tenant)
    print(f"[*] Access authorized for Tenant [{target_tenant}]. Context Window Population Stream:\n -> {retrieved_facts}")
