/*
 * (c) 2005-2018  Copyright, Real-Time Innovations, Inc. All rights reserved.
 * Subject to Eclipse Public License v1.0; see LICENSE.md for details.
 */
#ifdef RTI_CUSTOM_TYPE
#include "CustomType.h"

/*
 * This is the source code file that contains the implementation of the API
 * required to work with the Custom type.
 */

DDS_OctetSeq octectSet;
int oldDynamicTargetDataLen = 0;

bool initialize_custom_type_data(RTI_CUSTOM_TYPE &data)
{
    return true;
}

void register_custom_type_data(RTI_CUSTOM_TYPE &data, unsigned long key)
{
}

bool set_custom_type_data(
        RTI_CUSTOM_TYPE &data,
        unsigned long key,
        int targetDataLen)
{
    bool success = true;
    if (!data.test_seq.ensure_length(targetDataLen, targetDataLen)) {
        success = false;
    }
    return success;
}

bool finalize_custom_type_data(RTI_CUSTOM_TYPE &data)
{
    bool success = data.test_seq.ensure_length(0, 0);
    return success;
}

bool initialize_custom_type_dynamic_data(DDS_DynamicData &data)
{
    bool success = octectSet.maximum(0);
    if (!success) {
        fprintf(stderr, "octectSet.maximum failed.\n");
    }
    return success;
}

void register_custom_type_dynamic_data(DDS_DynamicData &data, unsigned long key)
{
}

bool set_custom_type_dynamic_data(
        DDS_DynamicData &data,
        unsigned long key,
        int targetDataLen)
{
    DDS_ReturnCode_t retcode;
    DDS_DynamicData customTypeData(NULL, DDS_DYNAMIC_DATA_PROPERTY_DEFAULT);
    bool success = true;
    if (oldDynamicTargetDataLen != targetDataLen) {
        success = octectSet.ensure_length(targetDataLen, targetDataLen);
        if (!success) {
            fprintf(stderr, "octectSet.ensure_length failed.\n");
        }
        oldDynamicTargetDataLen = targetDataLen;
    }

    retcode = data.bind_complex_member(customTypeData, "custom_type",
            DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED);
    if (retcode != DDS_RETCODE_OK) {
        fprintf(stderr,
                "bind_complex_member(custom_type) failed: %d.\n",
                retcode);
        success = false;
    }
    retcode = customTypeData.set_octet_seq(
            "test_seq",
            DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED,
            octectSet);
    if (retcode != DDS_RETCODE_OK) {
        fprintf(stderr, "set_octet_seq(test_seq) failed: %d.\n", retcode);
        success = false;
    }
    retcode = data.unbind_complex_member(customTypeData);
    if (retcode != DDS_RETCODE_OK) {
        fprintf(stderr,
                "unbind_complex_member(custom_type) failed: %d.\n",
                retcode);
        success = false;
    }
    return success;
}

bool finalize_custom_type_dynamic_data(DDS_DynamicData &data)
{
    bool success = octectSet.ensure_length(0, 0);
    if (!success) {
        fprintf(stderr, "octectSet.ensure_length failed.\n");
    }
    DDS_ReturnCode_t retcode = data.clear_all_members();
    if (retcode != DDS_RETCODE_OK) {
        fprintf(stderr, "clear_all_members failed: %d.\n", retcode);
        success = false;
    }
    return success;
}

#endif // RTI_CUSTOM_TYPE
