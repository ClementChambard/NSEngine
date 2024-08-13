#include "testing/Testing.hpp"

TEST_BATCH_BEGIN(run_testing_tests, "testing system")

TEST("Is OK"){TEST_OK}

TEST("Is KO"){TEST_KO}

TEST("Skipped"){SKIP_TEST}

TEST_BATCH_END
