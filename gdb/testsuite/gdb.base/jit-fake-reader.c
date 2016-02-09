#include <stdlib.h>
#include <stdio.h>
#include <gdb/jit-reader.h>

#include "jit-fake.h"

/*
 * BUILD: gcc -g -fPIC -shared -o jit-fake-reader.so jit-fake-reader.c 
 * 
 * load in gdb: 
 * jit-reader-load [YOUR SOURCE LOCATION]/gdb/testsuite/gdb.base/jit-fake-reader.so
 */
 
/* 
 * Required for gdb License compliance, this code is MIT and therefore
 * GPL compatible.
 */
GDB_DECLARE_GPL_COMPATIBLE_READER


enum gdb_status jit_reader_read_debug_info(struct gdb_reader_funcs *self,
                                       struct gdb_symbol_callbacks *cb,
                                       void *memory, long memory_sz) {
    struct gdb_object * obj;
    struct gdb_symtab *symtab;
    symbol_data_t * data;
    
    printf("jit-fake-reader: jit_reader_read_debug_info being called\n");
    data = (symbol_data_t *)memory;
    obj = cb->object_open(cb);
    printf("jit-fake-reader: open symtab for %s\n", data->filename);
    symtab = cb->symtab_open(cb, obj, data->filename);
    {
        void * start_address = data->line_addresses[0];
        void * end_address = data->line_addresses[data->num_lines-1];
        printf("jit-fake-reader: open block for %s %p->%p\n", 
               data->func_name, 
               start_address, 
               end_address);
        cb->block_open(cb, symtab, NULL, (unsigned long)start_address, 
                       (unsigned long)end_address, data->func_name);
    }
    {
        int i;
        struct gdb_line_mapping *line_mappings;
        line_mappings = (struct gdb_line_mapping *)calloc(
                                       data->num_lines, 
                                       sizeof(struct gdb_line_mapping));
        for(i = 0; i < data->num_lines; i++) 
        {        
            line_mappings[i].line = data->line_offset + i;
            line_mappings[i].pc = 
                        (unsigned long)data->line_addresses[i];
            printf("jit-fake-reader: line mapping %d = %p\n", 
               line_mappings[i].line, 
               (void *)line_mappings[i].pc);
        }
        cb->line_mapping_add(cb, symtab, data->num_lines, 
                             line_mappings);
        free(line_mappings);
    }
    printf("jit-fake-reader: closing symtab\n");
    cb->symtab_close(cb, symtab);
    printf("jit-fake-reader: closing object\n");
    cb->object_close(cb, obj);
    printf("jit-fake-reader: jit_reader_read_debug_info returning "
           "GDB_SUCCESS\n");
    return GDB_SUCCESS;
}


void jit_reader_destroy_reader(struct gdb_reader_funcs *self) {
    printf("jit-fake-reader: jit_reader_destroy_reader being called\n");
}

struct gdb_frame_id jit_reader_get_frame_id(struct gdb_reader_funcs *self,
                                        struct gdb_unwind_callbacks *c)
{
    static struct gdb_frame_id id;
    printf("jit-fake-reader: jit_reader_get_frame_id being called\n");
    id.code_address = 0;
    id.stack_address = 0;
    return id;
}
                                        

enum gdb_status jit_reader_unwind_frame(struct gdb_reader_funcs *self,
                                    struct gdb_unwind_callbacks *cb)
{
    printf("jit-fake-reader: jit_reader_unwind_frame being called\n");
    return GDB_FAIL;
}


static struct gdb_reader_funcs jit_reader_funcs;


struct gdb_reader_funcs *gdb_init_reader (void) {
    jit_reader_funcs.reader_version = GDB_READER_INTERFACE_VERSION;
    jit_reader_funcs.read = &jit_reader_read_debug_info;
    jit_reader_funcs.unwind = &jit_reader_unwind_frame;
    jit_reader_funcs.get_frame_id = &jit_reader_get_frame_id;
    jit_reader_funcs.destroy = &jit_reader_destroy_reader;
    return &jit_reader_funcs;
}