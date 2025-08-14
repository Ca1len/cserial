#include <cserial/circularbuffer.h>
#include <memory.h>
#include <stdio.h>

void init_buffer(CircularBuffer *cb) {
  cb->head = 0;
  cb->tail = 0;
  pthread_mutex_init(&cb->mutex, NULL);
  pthread_cond_init(&cb->not_empty, NULL);
  pthread_cond_init(&cb->not_full, NULL);
}

bool is_full(CircularBuffer *cb) {
  return ((cb->head + 1) % BUFFER_SIZE) == cb->tail;
}

bool is_empty(CircularBuffer *cb) { return cb->head == cb->tail; }

void write_buffer(CircularBuffer *cb, unsigned char value) {
  pthread_mutex_lock(&cb->mutex);
  while (is_full(cb)) {
    fprintf(stderr, "Buffer is full\n");
    pthread_cond_wait(&cb->not_full, &cb->mutex);
  }
  cb->buffer[cb->head] = value;
  cb->head = (cb->head + 1) % BUFFER_SIZE;
  pthread_cond_signal(&cb->not_empty);
  pthread_mutex_unlock(&cb->mutex);
}

unsigned char read_buffer(CircularBuffer *cb) {
  pthread_mutex_lock(&cb->mutex);
  while (is_empty(cb)) {
    pthread_cond_wait(&cb->not_empty, &cb->mutex);
  }
  unsigned char value = cb->buffer[cb->tail];
  cb->tail = (cb->tail + 1) % BUFFER_SIZE;
  pthread_cond_signal(&cb->not_full);
  pthread_mutex_unlock(&cb->mutex);
  return value;
}

void destroy_buffer(CircularBuffer *cb) {
  pthread_mutex_destroy(&cb->mutex);
  pthread_cond_destroy(&cb->not_empty);
  pthread_cond_destroy(&cb->not_full);
}
