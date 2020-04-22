/* Set the value of a specific bit in a number to 0 or 1.
Return value: the number after the requested change to the bit.
Bit index is expected to be at the range of the number.
Value to be set is expected to be either 0 or 1. */
size_t BARRSetBit(size_t arr, int index, int value);
/* Set a specific bit in a number to be on (have the value of 1).
Return value: the number after the requested change to the bit.
Bit index is expected to be at the range of the number. */
size_t BARRSetOn(size_t arr, int index);
/* Set a specific bit in a number to be off (have the value of 0).
Return value: the number after the requested change to the bit.
Bit index is expected to be at the range of the number. */
size_t BARRSetOff(size_t arr, int index);
/* Set a specific bit in a number to be have the opposite value. 
Return value: the number after the requested change to the bit.
Bit index is expected to be at the range of the number. */
size_t BARRFlip(size_t arr, int index);
/* Change the number so that the new number's bits are a mirror of the givan number.
Number is expected to be positive. */
size_t BARRMirror(size_t arr);
/* Check whether a specific bit is on. 
Return value: 1 if it is on, 0 if it is off.
Bit index is expected to be at the range of the number. */
int BARRIsOn(size_t arr, int index);
/* Check whether a specific bit is off. 
Return value: 1 if it is off, 0 if it is on.
Bit index is expected to be at the range of the number. */
int BARRIsOff(size_t arr, int index);
/* Change the number so that all the bit rotates amount of time left. 
Return value: the rotated number.
Amount of time to rotate is expected to be at max the size of the number. */
size_t BARRRotateLeft(size_t arr, size_t amount);
/* Change the number so that all the bit rotates amount of time right. 
Return value: the rotated number.
Amount of time to rotate is expected to be at max the size of the number. */
size_t BARRRotateRight(size_t arr, size_t amount);
/* Counts the number of on bits.
Return value: the number of on bits. */
size_t BARRCountOn(size_t arr);
/* Counts the number of off bits.
Return value: the number of off bits. */
size_t BARRCountOff(size_t arr);
/* Change the number so that the new number's bits are a mirror of the givan number.
Number is expected to be positive. */
size_t BARRMirrorLUT(size_t arr);
/* Counts the number of on bits.
Return value: the number of on bits. */
size_t BARRCountOnLUT(size_t arr);

