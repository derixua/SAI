#include <stdio.h>
#include "sai.h"

const char* test_profile_get_value(
    _In_ sai_switch_profile_id_t profile_id,
    _In_ const char* variable)
{
    return 0;
}

int test_profile_get_next_value(
    _In_ sai_switch_profile_id_t profile_id,
    _Out_ const char** variable,
    _Out_ const char** value)
{
    return -1;
}

const service_method_table_t test_services = {
    test_profile_get_value,
    test_profile_get_next_value
};

int main()
{
    sai_status_t              status;
    sai_switch_api_t         *switch_api;
    sai_object_id_t           vr_oid;
    sai_attribute_t           attrs[2];
    sai_switch_notification_t notifications;
    sai_object_id_t           port_list[64];
    sai_lag_api_t            *lag_api;
    sai_object_id_t           lag_oid[2];
    sai_object_id_t           lag_member_oid[4];

    sai_attribute_t attr_list_lag1[2] = {[0].id = 1, [1].id = 2},
                    attr_list_lag2[2] = {[0].id = 3, [1].id = 4},
                    attr_list_lag_member[3] = { [0].id = SAI_LAG_MEMBER_ATTR_LAG_ID,  [0].value.u8 = 1,
                                                [2].id = SAI_LAG_MEMBER_ATTR_PORT_ID, [2].value.u8 = 3
                                              };

    status = sai_api_initialize(0, &test_services);
    status = sai_api_query(SAI_API_LAG, (void**)&lag_api);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to query LAG API, status=%d\n", status);
        return 1;
    }
    /* Create LAG#1 */
    status = lag_api->create_lag(&lag_oid[0], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG, status=%d\n", status);
        return 1;
    }
    /* Create LAG_MEMBER#1 {LAG_ID:LAG#1, PORT_ID:PORT#1} */
    status = lag_api->create_lag_member((sai_object_id_t *)&lag_member_oid[0], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG_MEMBER, status=%d\n", status);
        return 1;
    }
    /* Create LAG_MEMBER#2 {LAG_ID:LAG#1, PORT_ID:PORT#2} */
    status = lag_api->create_lag_member((sai_object_id_t *)&lag_member_oid[1], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG_MEMBER, status=%d\n", status);
        return 1;
    }
    /* Create LAG#2 */
    status = lag_api->create_lag((sai_object_id_t *)&lag_oid[1], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG, status=%d\n", status);
        return 1;
    }
    /* Create LAG_MEMBER#3 {LAG_ID:LAG#2, PORT_ID:PORT#3} */
    status = lag_api->create_lag_member((sai_object_id_t *)&lag_member_oid[2], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG_MEMBER, status=%d\n", status);
        return 1;
    }
    /* Create LAG_MEMBER#4 {LAG_ID:LAG#2, PORT_ID:PORT#4} */
    status = lag_api->create_lag_member((sai_object_id_t *)&lag_member_oid[3], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG_MEMBER, status=%d\n", status);
        return 1;
    }
    /* Get LAG#1 PORT_LIST [Expected: (PORT#1, PORT#2)] */
    status = lag_api->get_lag_attribute(lag_oid[0], 2, attr_list_lag1);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get a LAG_ATTRIBUTE, status=%d\n", status);
        return 1;
    }
    /* Get LAG#2 PORT_LIST [Expected: (PORT#3, PORT#4)] */
    status = lag_api->get_lag_attribute(lag_oid[1], 2, attr_list_lag2);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get a LAG_ATTRIBUTE, status=%d\n", status);
        return 1;
    }
    /* Get LAG_MEMBER#1 LAG_ID [Expected: LAG#1] */
    status = lag_api->get_lag_member_attribute(lag_member_oid[0], 1, &attr_list_lag_member[0]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get a LAG_MEMBER_ATTRIBUTE, status=%d\n", status);
        return 1;
    }
    /* Get LAG_MEMBER#3 PORT_ID [Expected: PORT#3] */
    status = lag_api->get_lag_member_attribute(lag_member_oid[2], 1, &attr_list_lag_member[2]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get a LAG_MEMBER_ATTRIBUTE, status=%d\n", status);
        return 1;
    }
    /* Remove LAG_MEMBER#2 */
    status = lag_api->remove_lag_member(lag_member_oid[1]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG_MEMBER, status=%d\n", status);
        return 1;
    }
    /* Get LAG#1 PORT_LIST [Expected: (PORT#1)] */
    status = lag_api->get_lag_attribute(lag_oid[0], 1, attr_list_lag1);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get a LAG_ATTRIBUTE, status=%d\n", status);
        return 1;
    }
    /* Remove LAG_MEMBER#3 */
    status = lag_api->remove_lag_member(lag_member_oid[2]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG_MEMBER, status=%d\n", status);
        return 1;
    }
    /* Get LAG#2 PORT_LIST [Expected: (PORT#4)] */
    status = lag_api->get_lag_attribute(lag_oid[1], 1, &attr_list_lag2[1]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get a LAG_ATTRIBUTE, status=%d\n", status);
        return 1;
    }
    /* Remove LAG_MEMBER#1 */
    status = lag_api->remove_lag_member(lag_member_oid[0]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG_MEMBER, status=%d\n", status);
        return 1;
    }
    /* Remove LAG_MEMBER#4 */
    status = lag_api->remove_lag_member(lag_member_oid[3]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG_MEMBER, status=%d\n", status);
        return 1;
    }
    /* Remove LAG#2 */
    status = lag_api->remove_lag(lag_oid[1]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG, status=%d\n", status);
        return 1;
    }
    /* Remove LAG#1 */
    status = lag_api->remove_lag(lag_oid[0]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG, status=%d\n", status);
        return 1;
    }
    status = sai_api_uninitialize();

    return 0;
}

