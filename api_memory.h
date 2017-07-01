/*
author: Ray Garner 
email : raygarner13@gmail.com

api_vector  - public domain  emory allocing and deallocing - 
                                     no warranty implied; use at your own risk
                                     
                                     
                                     LICENSE
  See end of file for license information.
  
  */

#if !defined(API_MEMORY_H)

#if OSX
#include <mach/mach_init.h>
#include <mach/mach_vm.h>

static void*
OSXAllocateMemory(memory_index Size)
{
    mach_vm_address_t address;
    kern_return_t kr;
    mach_vm_size_t size = (mach_vm_size_t)Size;
    kr = mach_vm_allocate(mach_task_self(), &address, size, VM_FLAGS_ANYWHERE);
    //TODO(ray):Make sure this actually casts properly.
    return (void*)address;
}

static void OSXDeAllocateMemory(void* Memory,s64 Size)
{
    mach_vm_address_t Address = (mach_vm_address_t)Memory;
    //mach_vm_size_t MachSize = (mach_vm_size_t)Size;
    mach_vm_deallocate(mach_task_self(), Address, Size);
}

#endif

#if WINDOWS
#include <windows.h>

static void*
Win32AllocateMemory(memory_index Size)
{
    // TODO(ray): Verify the type we are passing in make sure
    //we are getting the proper page size back.
    void* Memory = VirtualAlloc(
        0,
        Size,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
        );
    if(!Memory)
    {
        // TODO(ray): Some error handling because we couldnt get the memory we need.
    }
    return Memory;
}

static void
Win32DeAllocateMemory(void* Memory,s64 Size)
{
    VirtualFree(
        Memory,
        Size,
        MEM_RELEASE
        );
}
#endif

enum partition_push_flag
{
    PartitionFlag_ClearToZero = 0x1,
};

struct partition_push_params
{
    u32 Flags;
    u32 Alignment;
};



inline partition_push_params
DefaultPartitionParams(void)
{
    partition_push_params Params;
    Params.Flags = PartitionFlag_ClearToZero;
    Params.Alignment = 4;
    return(Params);
}

struct memory_partition
{
    void* Base;
    u32 Size;
    u32 Used;
    u32 TempCount;
};

struct temp_memory
{
    memory_partition *Partition;
    u32 Used;
};

inline void*
PlatformAllocateMemory(memory_index Size)
{
    
#if OSX
    return OSXAllocateMemory(Size);
#elif WINDOWS
    return Win32AllocateMemory(Size);
#endif
}

inline void
PlatformDeAllocateMemory(void* Memory, memory_index Size)
{
#if OSX
    OSXDeAllocateMemory(Memory, Size);
#elif WINDOWS
    Win32DeAllocateMemory(Memory,Size);
#endif
}

inline memory_index
GetAlignmentOffset(memory_partition *Arena, memory_index Alignment)
{
    memory_index AlignmentOffset = 0;
    
    memory_index ResultPointer = (memory_index)Arena->Base + Arena->Used;
    memory_index AlignmentMask = Alignment - 1;
    if(ResultPointer & AlignmentMask)
    {
        AlignmentOffset = Alignment - (ResultPointer & AlignmentMask);
    }
    
    return(AlignmentOffset);
}

static void AllocatePartition(memory_partition *Partition, u32 Size, void* Base)
{
    //Assert
    Partition->Base = Base;
    Partition->Size = Size;
    Partition->Used = 0;
    Partition->TempCount = 0;
}

//TODO(ray):Fix this to clear more effeciently. or give option for clearing  method
static void ClearToZero(memory_partition *Partition)
{
    for (u32 ByteIndex = 0; ByteIndex < Partition->Size; ++ByteIndex)
    {
        uint8_t* Byte = (uint8_t*)Partition->Base + ByteIndex;
        *Byte = 0;
    }
}

static void DeAllocatePartition(memory_partition *Partition, bool ClearMemToZero = true)
{
    //Assert
    Partition->Used = 0;
    if (ClearMemToZero)
    {
        ClearToZero(Partition);
    }
}

static temp_memory BeginTempMemory(memory_partition *Partition)
{
    temp_memory Result;
    Result.Partition = Partition;
    Partition->TempCount++;
    Result.Used = Partition->Used;
    return Result;
}

static void EndTempMemory(temp_memory TempMem)
{
    TempMem.Partition->Used = TempMem.Used;
    TempMem.Partition->TempCount--;
}

static void ValidateTempMemory(temp_memory TempMem)
{
    Assert(TempMem.Partition->TempCount < 1);
}

static b32 TestFlag(u32 Flag, u32 TestAgaist)
{
    if ((Flag & TestAgaist) == TestAgaist)
    {
        return true;
    }
    return false;
}

static void ClearSize(memory_partition *Partition,u32 Size)
{
    Assert(Size > 0)
        u8* Byte = (u8*)Partition->Base + Partition->Used;
    while (Size--)
    {
        *Byte++ = 0;
    }
}

static void ClearToZero(void* Mem,u32 Size)
{
    Assert(Size > 0)
        u8* Byte = (u8*)Mem;
    while (Size--)
    {
        *Byte++ = 0;
    }
}

//TODO(ray):Add memory alignment options here!!

#define PushArray(Partition,Type,Count,...) PushSize_(Partition,sizeof(Type)*Count,## __VA_ARGS__)
#define PushStruct(Partition,Type,...) (Type*)PushSize_(Partition,sizeof(Type),## __VA_ARGS__)
#define PushSize(Partition,Size,...) PushSize_(Partition,Size,## __VA_ARGS__)
static void* PushSize_(memory_partition*Partition, u32 Size,partition_push_params PushParams = DefaultPartitionParams())
{
    //Assert Used < Size
    Assert(Partition->Used + Size <= Partition->Size)
        void* Result;
    Result = (uint8_t*)Partition->Base + Partition->Used;
    if (TestFlag(PushParams.Flags, PartitionFlag_ClearToZero))
    {
        ClearSize(Partition, Size);
    }
    Partition->Used = Partition->Used + Size;
    
    return Result;
}


#define API_MEMORY_H
#endif


/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2017 Sean Barrett
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/