#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "structures.h"
#include "print.h"
#include "tools.h"

FILE* file;

void write_int(int value);

void write_id(long ID);

void write_int_list(int* list, int size);

void write_position(struct position p);

void write_item(struct item i);

void write_item_list(struct item *list, int size);

void write_container(struct container c);

void write_container_list(struct container *list, long size);

void write_request(struct request r);

void write_request_list(struct request *list, int size);

void write_map(struct map m);

void write_map_list(struct map *list, int size);

void write_project(struct project p);

char save_project(struct project p);