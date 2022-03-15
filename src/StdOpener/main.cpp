#include "stdOpener.h"

int main(int argc, char *argv[]) {

    if (argc < 2) return 1;

    std::string file = argv[1];
    auto std = StdOpener::std_read_file(file);

    StdOpener::std_dump(std);

    StdOpener::std_free(std);

    return 0;
}
