/**
 * @file circular_buffer.h
 * @author your name (dean@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CBUF_H
#define CBUF_H

#include <stdint.h>
#include <stdbool.h>

/** 
 * @brief Structure containing circular buffer metadata and a pointer to the underlying buffer
*/
typedef struct circular_buffer_t
{
    uint8_t * buffer;
    uint32_t head;
    uint32_t tail;
    uint32_t capacity;
    uint32_t size;
} circular_buffer_t;

/**
 * Status codes for various circular buffer operations
*/
typedef enum cb_status_t
{
    CB_OK,
    CB_BUFFER_FULL,
    CB_BUFFER_EMPTY,
    CB_ZERO_CAPACITY
} cb_status_t;

/**
 * @brief Initialise a new circular buffer. You must provide the backing memory.
 * 
 * @param cbuf Pointer to circular_buffer_t object to initialise.
 * @param _buffer Pointer to memory to use as a backing buffer for the circular buffer.
 * @param _capacity Maximum capacity of the circular buffer (typically the size of the backing buffer). Must be > 0.
 * 
 * @return Status of initialisation operation. Will return CB_ZERO_CAPACITY if you attempt to initialise a buffer with zero capacity.
 * 
*/
cb_status_t circular_buffer_init(circular_buffer_t * cbuf, uint8_t * _buffer, uint32_t _capacity);

/**
 * @brief Check if a circular buffer is empty.
 * 
 * @param cbuf Buffer to check.
 * 
 * @return True if empty, false otherwise.
*/
bool circular_buffer_empty(circular_buffer_t * cbuf);

/**
 * @brief Check if a circular buffer is full.
 * 
 * @param cbuf Buffer to check
 * 
 * @return True if full, false otherwise
*/
bool circular_buffer_full(circular_buffer_t * cbuf);

/**
 * @brief Retreive one element from the tail of the buffer. This removes the element.
 * 
 * @param cbuf Buffer to get from
 * @param byte Pointer to store byte in
 * 
 * @return Status of operation. Will return CB_BUFFER_EMPTY if you attempt to get from an empty buffer.
*/
cb_status_t circular_buffer_get(circular_buffer_t * cbuf, uint8_t * byte);

/**
 * @brief Get the current number of elements contained within the circular buffer.
 * 
 * @param cbuf Buffer to get size of.
 * 
 * @return Number of elements within buffer.
*/
uint32_t circular_buffer_size(circular_buffer_t * cbuf);

/**
 * @brief Insert one element at the head of the buffer.
 * 
 * @param cbuf Buffer to insert into
 * @param byte Byte to insert
 * 
 * @return Status of operation. Will return CB_BUFFER_FULL if you attempt to put into an empty buffer.
*/
cb_status_t circular_buffer_put(circular_buffer_t * cbuf, uint8_t byte);

#endif