#include "circular_buffer.h"

cb_status_t circular_buffer_init(circular_buffer_t * cbuf, uint8_t * _buffer, uint32_t _capacity)
{
    if (_capacity == 0)
    {
        return CB_ZERO_CAPACITY;
    }

    cbuf->capacity = _capacity;
    cbuf->buffer = _buffer;
    cbuf->tail = 0;
    cbuf->head = 0;
    cbuf->size = 0;

    return CB_OK;
}

bool circular_buffer_empty(circular_buffer_t * cbuf)
{
    return cbuf->size == 0;
}

bool circular_buffer_full(circular_buffer_t * cbuf)
{
    return cbuf->size == cbuf->capacity;
}

cb_status_t circular_buffer_get(circular_buffer_t * cbuf, uint8_t * byte)
{
    if (circular_buffer_empty(cbuf))
    {
        return CB_BUFFER_EMPTY;
    }
    *byte = cbuf->buffer[cbuf->tail];
    cbuf->tail++;
    cbuf->tail %= cbuf->capacity;
    cbuf->size--;
    return CB_OK;
}

uint32_t circular_buffer_size(circular_buffer_t * cbuf)
{
    return cbuf->size;
}

cb_status_t circular_buffer_put(circular_buffer_t * cbuf, uint8_t byte)
{
    if (circular_buffer_full(cbuf))
    {
        return CB_BUFFER_FULL;
    }
    cbuf->buffer[cbuf->head] = byte;
    cbuf->head++;
    cbuf->head %= cbuf->capacity;
    cbuf->size++;
    return CB_OK;
}