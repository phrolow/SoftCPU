#define DEF_CMD(name, num, argc, ...) \
    name##_CMD = num,

enum CMD {
    #include "cmd.h"
};

#undef DEF_CMD

#define DEBUG_MODE
#ifdef DEBUG_MODE
#define ON_DEBUG_MODE(...) __VA_ARGS__
#else
#define ON_DEBUG_MODE(...)
#endif

const char VERSION = 2;
const size_t SIGNSIZE = 3 + sizeof(size_t);

unsigned const char ARG_IMMED = 0x20;
unsigned const char ARG_REG   = 0x40;
unsigned const char ARG_RAM   = 0x80;