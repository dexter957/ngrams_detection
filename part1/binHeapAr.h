#define TRUE 1
#define FALSE 0


typedef struct binaryHeapArray* pointerToBinHeapArray;

int binaryHeapArrayInit(pointerToBinHeapArray *heap);
void set_HeapLength(pointerToBinHeapArray heap, int length);
void set_HeapOccupied(pointerToBinHeapArray heap, int occupied);
void set_HeapArray(pointerToBinHeapArray heap);
int get_HeapLength(pointerToBinHeapArray heap);
int get_HeapOccupied(pointerToBinHeapArray heap);
int insertInBinaryHeapArray(pointerToBinHeapArray heap, void* pointer);
void printBinaryHeapArray(pointerToBinHeapArray heap);
void doubleHeapArrayCapacity(pointerToBinHeapArray heap);
//int searchBinaryHeapArray(pointerToBinHeapArray heap, void* elem);
void destroyBinaryHeapArray(pointerToBinHeapArray heap);
int searchBinaryHeapArray(pointerToBinHeapArray heap, void* elem);
int searchSubTree(pointerToBinHeapArray heap, void* elem,int i);