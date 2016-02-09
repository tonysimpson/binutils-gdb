#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "jit-fake.h"

/* BUILD: 
 * 
 * gcc -s -c jit-fake-code.c 
 * gcc -g jit-fake-code.o jit-fake.c -o jit-fake 
 * 
 * */
/******************************************/
/**** Start GDB JIT Hook Specific Bits ****/
/******************************************/
typedef enum
{
  JIT_NOACTION = 0,
  JIT_REGISTER_FN,
  JIT_UNREGISTER_FN
} jit_actions_t;

struct jit_code_entry
{
  struct jit_code_entry *next_entry;
  struct jit_code_entry *prev_entry;
  const char *symfile_addr;
  uint64_t symfile_size;
};

struct jit_descriptor
{
  uint32_t version;
  /* This type should be jit_actions_t, but we use uint32_t
     to be explicit about the bitwidth.  */
  uint32_t action_flag;
  struct jit_code_entry *relevant_entry;
  struct jit_code_entry *first_entry;
};

void __attribute__((noinline)) __jit_debug_register_code() { };

struct jit_descriptor __jit_debug_descriptor = { 1, 0, 0, 0 };
/****************************************/
/**** End GDB JIT Hook Specific Bits ****/
/****************************************/


void register_symbol_data(void *symfile_addr, size_t symfile_size) 
{
    struct jit_code_entry *entry = malloc(sizeof(struct jit_code_entry));
    entry->symfile_addr = symfile_addr;
    entry->symfile_size = symfile_size;
    entry->prev_entry = NULL;
    entry->next_entry = __jit_debug_descriptor.first_entry;
    if(entry->next_entry != NULL) 
    {
        entry->next_entry->prev_entry = entry;
    }
    __jit_debug_descriptor.first_entry = entry;
    __jit_debug_descriptor.relevant_entry = entry;
    __jit_debug_descriptor.action_flag = JIT_REGISTER_FN;
    __jit_debug_register_code();
}


symbol_data_t * create_symbol_data(char *filename, char *func_name, 
                                   int line_offset, int num_lines, 
                                   void ** line_addresses)
{
    symbol_data_t *symbol_data = malloc(sizeof(symbol_data_t));
    strncpy(symbol_data->filename, filename, SYMBOL_DATA_MAX_STR_LEN);
    strncpy(symbol_data->func_name, func_name, SYMBOL_DATA_MAX_STR_LEN);
    symbol_data->line_offset = line_offset;
    symbol_data->num_lines = num_lines;
    memcpy(symbol_data->line_addresses, line_addresses, 
           sizeof(void *) * num_lines);
    return symbol_data;
}


void register_fake(char *filename, char *func_name, int line_offset, 
                   int num_lines, void * line_addresses[])
{
    symbol_data_t *symbol_data = create_symbol_data(filename, func_name, 
                                                    line_offset, 
                                                    num_lines, 
                                                    line_addresses);
    
    register_symbol_data(symbol_data, sizeof(symbol_data_t));
}

/* provided by jit-fake-code.so from jit-fake-code.c */
void ** jit_code_fake(void);

int main(int argc, char **argv) {
    register_fake("jit-fake-source.txt", "fake_function", 1, 7, jit_code_fake());
    jit_code_fake();
    jit_code_fake();
    jit_code_fake();
    return 0;
}