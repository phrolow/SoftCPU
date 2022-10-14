#define DEF_CMD(name, num, argc, ...) \
    name##_CMD = num,

enum CMD {
    #include "cmd.h"
};

#undef DEF_CMD

const char VERSION = 2;
const size_t SIGNSIZE = 3 + sizeof(size_t);