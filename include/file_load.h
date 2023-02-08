#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "structures.h"
#include "print.h"
#include "tools.h"

#include <stdlib.h>

extern char file_loaded;

FILE* file;

char check_endline();

char load_int(int *value);

char load_id(long *ID);

char load_int_list(int* list, int size);

char load_position(struct position *p);

char load_item(struct item *i);

char load_item_list(struct item *list, int size);

char load_container(struct container *c);

char load_container_list(struct container *list, long size);

char load_request(struct request *r);

char load_request_list(struct request *list, int size);

char load_map(struct map *m);

char load_map_list(struct map *list, int size);

char load_project(struct project *p);

char open_project(struct project *p);