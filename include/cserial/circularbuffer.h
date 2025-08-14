#pragma once
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE (0x100000)
#endif

typedef struct {
  unsigned char buffer[BUFFER_SIZE];
  size_t head;
  size_t tail;
  pthread_mutex_t mutex;
  pthread_cond_t not_empty;
  pthread_cond_t not_full;
} CircularBuffer;

void init_buffer(CircularBuffer *cb);

bool is_full(CircularBuffer *cb);

bool is_empty(CircularBuffer *cb);

void write_buffer(CircularBuffer *cb, unsigned char value);

unsigned char read_buffer(CircularBuffer *cb);

void destroy_buffer(CircularBuffer *cb);
