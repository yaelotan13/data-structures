typedef struct CBUF cbuf_t;

cbuf_t *CBUFCreate(size_t nbytes);
void CBUFDestroy(cbuf_t *cbuf);
size_t CBUFWrite(cbuf_t *cbuf, const void *data, size_t nbytes);
size_t CBUFRead(cbuf_t *cbuf, void *dest, size_t nbytes);
size_t CBUFCapacity(cbuf_t *cbuf);
int CBUFIsEmpty(const cbuf_t *cbuf);
size_t CBUFFreeSpace(cbuf_t *cbuf);
