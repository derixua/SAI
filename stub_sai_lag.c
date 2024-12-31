#include "sai.h"
#include "stub_sai.h"
#include "assert.h"

sai_status_t stub_create_lag(
    _Out_ sai_object_id_t* lag_id,
    _In_ uint32_t attr_count,
    _In_ sai_attribute_t *attr_list)
{
    static int32_t next_lag_id = 1;
    sai_status_t status;
    status = stub_create_object(SAI_OBJECT_TYPE_LAG, next_lag_id++, lag_id);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Cannot create a LAG OID\n");
        return status;
    }
    printf("CREATE LAG: 0x%lX\n", *lag_id);
    return SAI_STATUS_SUCCESS;
}

sai_status_t stub_remove_lag(
    _In_ sai_object_id_t  lag_id)
{
    sai_status_t status = stub_remove_object(SAI_OBJECT_TYPE_LAG, lag_id);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Cannot remove LAG OID: 0x%lX\n", lag_id);
        return status;
    }
    printf("REMOVE LAG: 0x%lX\n", lag_id);
    return SAI_STATUS_SUCCESS;
}

sai_status_t stub_set_lag_attribute(
    _In_ sai_object_id_t  lag_id,
    _In_ const sai_attribute_t *attr)
{
    printf("SET LAG_ATTRIBUTE\n");
    return SAI_STATUS_SUCCESS;
}

sai_status_t stub_get_lag_attribute(
    _In_ sai_object_id_t lag_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    printf("GET LAG: 0x%lX PORT_LIST:", lag_id);
    for (uint32_t i = 0; i < attr_count; i++) {
        printf(" PORT 0x%u%s", attr_list[i].id, (i + 1 < attr_count ? "," : ""));
    }
    printf("\n");
    return SAI_STATUS_SUCCESS;
}

sai_status_t stub_create_lag_member(
    _Out_ sai_object_id_t* lag_member_id,
    _In_ uint32_t attr_count,
    _In_ sai_attribute_t *attr_list)
{
    static int32_t next_lag_member_id = 1;
    sai_status_t status;
    status = stub_create_object(SAI_OBJECT_TYPE_LAG_MEMBER, next_lag_member_id++, lag_member_id);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Cannot create a LAG_MEMBER OID\n");
        return status;
    }
    printf("CREATE LAG_MEMBER: 0x%lX\n", *lag_member_id);
    return SAI_STATUS_SUCCESS;
}

sai_status_t stub_remove_lag_member(
    _In_ sai_object_id_t  lag_member_id)
{
    sai_status_t status = stub_remove_object(SAI_OBJECT_TYPE_LAG_MEMBER, lag_member_id);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Cannot remove LAG_MEMBER OID: 0x%lX\n", lag_member_id);
        return status;
    }
    printf("REMOVE LAG_MEMBER: 0x%lX\n", lag_member_id);
    return SAI_STATUS_SUCCESS;
}

sai_status_t stub_set_lag_member_attribute(
    _In_ sai_object_id_t  lag_member_id,
    _In_ const sai_attribute_t *attr)
{
    printf("SET LAG_MEMBER_ATTRIBUTE\n");
    return SAI_STATUS_SUCCESS;
}

sai_status_t stub_get_lag_member_attribute(
    _In_ sai_object_id_t lag_member_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list)
{
    if (attr_list == NULL || attr_count == 0) {
        printf("Failed to get a LAG_MEMBER_ATTRIBUTE, NULL attribute list\n");
        return SAI_STATUS_FAILURE;
    }

    for (uint32_t i = 0; i < attr_count; ++i) {
        switch (attr_list[i].id) {
            case SAI_LAG_MEMBER_ATTR_LAG_ID:
                attr_list[i].id = lag_member_id; 
                printf("GET LAG_MEMBER 0x%lX: LAG %u\n", lag_member_id, attr_list->value.u8);
                break;

            case SAI_LAG_MEMBER_ATTR_PORT_ID:
                attr_list[i].id = lag_member_id; 
                printf("GET LAG_MEMBER 0x%lX: PORT %u\n", lag_member_id, attr_list->value.u8);
                break;

            default:
                printf("Unknown LAG_MEMBER_ATTRIBUTE:0x%X\n", attr_list[i].id);
                return SAI_STATUS_FAILURE;
        }
    }


    return SAI_STATUS_SUCCESS;
}

const sai_lag_api_t lag_api = {
    stub_create_lag,
    stub_remove_lag,
    stub_set_lag_attribute,
    stub_get_lag_attribute,
    stub_create_lag_member,
    stub_remove_lag_member,
    stub_set_lag_member_attribute,
    stub_get_lag_member_attribute
};