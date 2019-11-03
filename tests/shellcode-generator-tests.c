#include "CUnit/Basic.h"

int main() {
  // Initialize the CUnit test registry
  if (CUE_SUCCESS != CU_initialize_registry()) {
    return CU_get_error();
  }
  // CU_BRM_VERBOSE will show maximum output of run details
  CU_basic_set_mode(CU_BRM_VERBOSE);
  // Run the tests and show the run summary
  CU_basic_run_tests();
  return CU_get_error();
}
