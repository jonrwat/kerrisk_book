// Note: malloc.h is a depricated header file - instead stdlib.h is used
// to access dynamic memory allocation functions, e.g. malloc and free.
// malloc.h is still needed though for accessing non-standard dynamic
// memory functions - e.g. mallinfo(3).

#include <malloc.h>
#include <mcheck.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

static void printHeap(void* ptr, uint8_t size);

extern char etext, edata, end;

// https://linux-audit.com/linux-aslr-and-kernelrandomize_va_space-setting/
//
// Newer version of Linux (i.e. Raspbian) have Address Space Layout
// Randomization (ASLR) enabled by default as a computer security
// technique.  This includes random placement of the executable, stack,
// heap, and libraries within the virtual address space.  This setting
// therefore does allow for easy comparing to process memory segments to
// the book "The Linux Programming Interface (Figure 6-1)".  To temporarily
// disable ASLR, use terminal command:
//
//		sudo sysctl -w kernel.randomize_va_space=0		(@see https://linux-audit.com/linux-aslr-and-kernelrandomize_va_space-setting/)
//
// Inspection of program output still shows that the program break
// (i.e. heap end) does not immediately follow the uninitialized data
// segment (i.e. bss).  The reason for this can be identified by inspecting
// the memory consumption for each of the process's mappings, using 
// terminal command:
//
//		cat /proc/[pid]/smaps	(@see https://man7.org/linux/man-pages/man5/proc.5.html)
//
// This shows that the heap follows the BSS segment, but starts on the next
// 4KB page.  Also, the heap has automatically been allocated 132KB, so
// 'malloc' won't grow the program break until more than this storage
// allocation is achieved.

int main(int argc, char* argv[])
{
	char* c_arr_brk;
	char* c_arr_malloc1;
	char* c_arr_malloc2;
	char* c_arr_malloc3;
	
    //-----------------------------
    // Enable tracing malloc functions.
    //
    // This will write tracing information to the file specified by
    // environment variable MALLOC_TRACE.  Identically named script
    // mtrace can be used to decode the generated trace file.
    //
    // Inspection of the generated "trace.txt" and decoding with mtrace
    // (built in terminal command) shows the 3 malloc allocations and
    // them being freed.  Only one additional malloc exists (within
    // the standard library) which is not freed.
    //-----------------------------
    (void) setenv("MALLOC_TRACE", "trace.txt", 1);
	mtrace();
	
    //-----------------------------
    // Process Memory
    //-----------------------------
    {
        // Print processes virtual memory layout.
        //  - etext: Byte address after end of text segment (i.e. program text).
        //  - edata: Byte address after end of data segment (i.e. initialized global and static variables).
        //  - end:   Byte address after end of bss segment (i.e. uninitialized global and static variables).
        printf("etext: %p, edata: %p, end: %p\n", &etext, &edata, &end);
	}
	
	//----------------------
	// Use 'brk/sbrk' to increase heap size.
	//----------------------
	{
		
		printf("\n");
		printf("Using 'brk' to increase heap size\n");
		printf("Program break: %p\n", sbrk(0));
		printf("Allocating 16 bytes...\n");
		c_arr_brk = sbrk(0);
		(void) brk(c_arr_brk + 16);
		printf("Program break: %p\n", sbrk(0));
		
		printf("Using 'sbrk' to increase heap size\n");
		printf("Program break: %p\n", sbrk(0));
		printf("Allocating 16 bytes...\n");
		(void) sbrk(16);
		printf("Program break: %p\n", sbrk(0));
	}
	
	//----------------------
	// Use 'malloc' to allocate variables to the heap.
	//
	// https://sourceware.org/glibc/wiki/MallocInternals
	//
	// Implementation of 'malloc' includes a header which preceedes each
	// allocation payload.  This header defines the length of the entire
	// block (header + payload) and some flags.
	//
	// On this raspbian build:
	//	- The header is four bytes in length.
	//	- The flags occupy the three least significant bits of the header.
	//	- All 'malloc' allocations (header + payload) are a multiple of 
	//    eight - which allows the three LSBs to be used as flags since
	//    these would be known by the implementation to always be zero.
	//----------------------
	{
		printf("\n");
		printf("Using 'malloc' to allocating three regions...\n");
		c_arr_malloc1 = malloc(12);
		printf("Malloc block 1 (addr: %p): \n", c_arr_malloc1);
		printHeap(c_arr_malloc1, 12);
		c_arr_malloc2 = malloc(12);
		printf("Malloc block 2 (addr: %p): \n", c_arr_malloc2);
		printHeap(c_arr_malloc2, 12);
		c_arr_malloc3 = malloc(12);
		printf("Malloc block 3 (addr: %p): \n", c_arr_malloc3);
		printHeap(c_arr_malloc3, 12);
	}
	
	//----------------------
	// Use 'mallinfo' to print information about dynamic
	// memory allocation.
	//
	// hhttps://www.gnu.org/software/libc/manual/html_node/Statistics-of-Malloc.html
	//
	// The output shows:
	//	- minfo.arena: the memory allocated using sbrk within malloc
	//    matches the 132KB pre-allocation.  The three calls to malloc
	//    haven't caused to allocation to change so it's still just
	//    the 132KB value.
	// 	- minfo.uordblks: I'm not sure about this value.  It grows as
	//    expected with the addition of "malloc" allocations; it has a
	//    size of 2472 bytes even with no "malloc" operations performed.
	//    It looks like this value corresponds to the memory start of
	//    "malloc" allocations when present; therefore the value must
	//    be correct and identify malloc allocations by the stadard
	//    library (even though this isn't apparent from the generated
	//    mtrace file).
	//----------------------
	{
		struct mallinfo minfo = mallinfo();
		printf("\n");
		printf("mallinfo statistics:\n");
		printf("- sbrk allocated size: %d\n", minfo.arena);
		printf("- malloc allocated size: %d\n", minfo.uordblks);
	}
	
	//----------------------
	// Use 'free' to unallocate variables on the heap.
	//
	// https://sourceware.org/glibc/wiki/MallocInternals
	//
	// Implementation of 'free' includes a fields stamped to every
	// freed region.  The first value is the same header from the
	// 'malloc' storage, while the next two fields make a doubly linked
	// list to the previous and next storage block.
	//
	// On this raspbian build:
	//	- The 'malloc' header is four bytes in length.
	//	- The flags occupy the three least significant bits of the header.
	//	- The pointers to the previous and next storage are four bytes in length.
	//----------------------
	{
		printf("\n");
		printf("Free malloc-ed blocks...\n");
		free(c_arr_malloc1);
		free(c_arr_malloc2);
		free(c_arr_malloc3);
		printf("Blocks 1:\n");
		printHeap(c_arr_malloc1, 12);
		printf("Blocks 2:\n");
		printHeap(c_arr_malloc2, 12);
		printf("Blocks 3:\n");
		printHeap(c_arr_malloc3, 12);
	}
	
	//----------------------
	// Use 'alloca' to dynamically allocate memory on the stack.
	//
	// Inspection of file `proc/[pid]/smaps` shows the variable allocation
	// is located on the stack as expected.  And the stack grows "down"
	// while the heap grows "up" - see Figure 6-1 from book.
	//----------------------
	{
		char* alloca_1;
		char* alloca_2;
		
		printf("\n");
		printf("Using 'alloca' to allocating two regions...\n");
		alloca_1 = alloca(16);
		printf("alloca block 1 (addr: %p): \n", alloca_1);
		alloca_2 = alloca(16);
		printf("alloca block 2 (addr: %p): \n", alloca_2);
	}
	
	// Increase sleep time when inspecition the processes' memory
	// map (proc/[pid]/smaps), so that it can be viewed before
	// the process finishes.
	sleep(2);
	
	muntrace();
	return 0;
}

static void printHeap(void* ptr, uint8_t size)
{
	uint32_t* ptr_u32 = ((uint32_t*)ptr) - 1;
	uint8_t   size_u32 = (size / 4) + 1;
	
	uint8_t idx;
	for (idx = 0; idx < size_u32; idx++)
	{
		printf("  %#010x\n", ptr_u32[idx]);
	}
}
