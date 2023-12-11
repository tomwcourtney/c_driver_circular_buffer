# Circular Buffer Implementation

## Set Up
```C
#define BUF_CAPACITY 512
// create a regular array
uint8_t backing_buffer[BUF_CAPACITY];

// initialise circular buffer object with regular array
circular_buffer_t my_buffer;
circular_buffer_init(&my_buffer, backing_buffer, BUF_CAPACITY);
```

## Usage
```C
// you can now access your backing buffer through a circular FIFO interface
circular_buffer_put(&my_buffer, 0xff);
uint8_t byte;
if (circular_buffer_get(&my_buffer, &byte) == CB_OK)
{
  // do stuff with our byte
}
```
