#ifndef JIT_FAKE_H
#define JIT_FAKE_H

#define SYMBOL_DATA_MAX_STR_LEN 2048
#define SYMBOL_DATA_MAX_LINES 20

typedef struct {
    char filename[SYMBOL_DATA_MAX_STR_LEN];
    char func_name[SYMBOL_DATA_MAX_STR_LEN];
    int line_offset;
    int num_lines;
    void *line_addresses[SYMBOL_DATA_MAX_LINES];
} symbol_data_t;

#endif /* JIT_FAKE_H */