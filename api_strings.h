/*
author: Ray Garner 
email : raygarner13@gmail.com

api_strings  - public domain string handling - 
                                     no warranty implied; use at your own risk
                                     
                                     LICENSE
  See end of file for license information.
  
  */

#if !defined(API_STRINGS_H)

#ifdef API_STRING_STATIC
#define APIDEF static
#else
#ifdef __cplusplus
#define APIDEF extern "C"
#else
#define APIDEF extern
#endif
#endif

#ifndef _MSC_VER
#ifdef __cplusplus
#define api__inline inline
#else
#define api__inline
#endif
#else
#define api__inline __forceinline
#endif

#define MAX_FILENAME_LENGTH 50
#define MAX_FILE_EXTENSION_LENGTH 10

#include <stdint.h>
#include <stdio.h>
#include "api_memory.h"

struct string
{
    u32 NullTerminated;
    u32 Length;
    char* String;
};

struct fixed_element
{
    bool IsSentinal;
    string** Data;
    fixed_element* Next;
};

struct strings
{
    string* Strings;
    u32 StringCount;
    u32 IteratorIndex;
};

struct fixed_element_size_list
{
    fixed_element* Head;
    fixed_element* Sentinal;
    u32 Length;
    u32 UnitSize;
    
};

api__inline b32 IsDigit(char Char)
{
    if ((Char == ' ' || Char == '.' ||
         Char > '9' || Char < '0'))
    {
        return false;
    }
    return true;
}

//NOTE(ray):Assumes string is already null terminated.
APIDEF u32 String_GetLength_String(string* String)
{
    u32 Length = 0;
    char* At = String->String;
    while(*At)
    {
        Length++;
        At++;
    }
    String->Length = Length;
    return Length;
}

APIDEF u32 String_GetLengthSafely_String(string* String,u32 SafetyLength)
{
    u32 Length = 0;
    char* At = String->String;
    while(*At)
    {
        Length++;
        At++;
        if(Length > SafetyLength)
        {
            break;
        }
    }
    String->Length = Length;
    return Length;
}

//NOTE(ray):Assumes string is already null terminated.
APIDEF u32 String_GetLength_Char(char* String)
{
    u32 Length = 0;
    char* At = String;
    while(*At)
    {
        Length++;
        At++;
    }
    return Length;
}

APIDEF u32 String_GetLengthSafely_Char(char* String,u32 SafetyLength)
{
    u32 Length = 0;
    char* At = String;
    while(*At)
    {
        Length++;
        At++;
        if(Length > SafetyLength)
        {
            break;
        }
    }
    return Length;
}

APIDEF string NullTerminate(string Source)
{
    char* NullTerminatePoint = Source.String + Source.Length;
    *NullTerminatePoint = '\0';
    Source.NullTerminated = true;
    return Source;
}

//TODO(ray):Make a way to reclaim the memory from literals created here.
APIDEF string* CreateStringFromLiteral(char* String,memory_partition* Memory)
{
    string* Result = (string*)PushSize(Memory,sizeof(string));
    Result->Length = 0;
    char* At = String;
    void* StartPointer = GetPartitionPointer(*Memory);
    char* StringPtr;
    while (*At)
    {
        StringPtr = (char*)PushSize(Memory,1);
        *StringPtr = *At;
        Result->Length++;
        At++;
    }
    Result->String = (char*)StartPointer;
    return Result;
}

//TODO(Ray):Make a way to reclaim the memory from literals created here.
//TODO(Ray):Allow to have the option to do the length check safely.
//NOTE(Ray):This function requires you free your own memory once your done.
APIDEF string* String_Allocate(char* String)
{
    u32 Length = String_GetLength_Char(String);
    void* Mem = PlatformAllocateMemory(Length);
    string* Result = (string*)Mem;
    Result->Length = 0;
    char* At = String;
    void* StartPointer = Mem;
    char* StringPtr = (char*)Mem;
    while (*At)
    {
        *StringPtr++ = *At;
        Result->Length++;
        At++;
    }
    Result->String = (char*)StartPointer;
    Result->Length = Length;
    return Result;
}

APIDEF string* AllocatEmptyString(memory_partition* Partition)
{
    Assert(Partition);
    return CreateStringFromLiteral("",Partition);
}

APIDEF string* CreateStringFromToChar(char* String,char* End, memory_partition* Memory)
{
    string* Result = (string*)PushSize(Memory, sizeof(string));
    
    char* At = String;
    void* StartPointer = GetPartitionPointer(*Memory);
    char* StringPtr = 0;//(char*)Memory;
    while (*At != *End)
    {
        StringPtr = (char*)PushSize(Memory, 1);
        *StringPtr = *At;
        Result->Length++;
        At++;
    }
    Result->String = (char*)StartPointer;
    return Result;
}

APIDEF string* API_CreateStringFromToPointer_WithSplitMem(char* String, char* End,duel_memory_partition* Memory)
{
    string* Result = (string*)PushSize(Memory->FixedSized, sizeof(string));
    
    char* At = String;
    void* StartPointer = GetPartitionPointer(*Memory->VariableSized);
    char* StringPtr = 0;//(char*)Memory;
    while (At != End)
    {
        StringPtr = (char*)PushSize(Memory->VariableSized, 1);
        *StringPtr = *At;
        Result->Length++;
        At++;
    }
    //One more for a possible null char.
    (char*)PushSize(Memory->VariableSized, 1);
    Result->String = (char*)StartPointer;
    //*Result = NullTerminate(*Result);
    return Result;
}

APIDEF string* CreateStringFromToPointer(char* String, char* End, memory_partition* Memory)
{
    string* Result = (string*)PushSize(Memory, sizeof(string));
    
    char* At = String;
    void* StartPointer = GetPartitionPointer(*Memory);
    char* StringPtr = 0;//(char*)Memory;
    while (At != End)
    {
        StringPtr = (char*)PushSize(Memory, 1);
        *StringPtr = *At;
        Result->Length++;
        At++;
    }
    Result->String = (char*)StartPointer;
    return Result;
}
APIDEF string* CreateStringFromLength(char* String,u32 Length,memory_partition* Memory)
{
    string* Result = (string*)PushSize(Memory,sizeof(string));
    
    char* At = String;
    void* StartPointer = GetPartitionPointer(*Memory);
    char* StringPtr = 0;//(char*)Memory;
    u32 Iterator = 0;
    while (Iterator < Length)
    {
        StringPtr = (char*)PushSize(Memory,1);
        *StringPtr = *At;
        Result->Length++;
        At++;
        Iterator++;
    }
    Result->String = (char*)StartPointer;
    return Result;
}

APIDEF int Compare(string A, string B)
{
    if (A.NullTerminated && B.NullTerminated)
    {
        char* APtr = A.String;
        char* BPtr = B.String;
        while (*APtr && *BPtr)
        {
            if (*APtr != *BPtr)
                return false;
            
            APtr++;BPtr++;
        }
    }
    else
    {
        char* APtr = A.String;
        char* BPtr = B.String;
        
        u32 MaxIterations = (A.Length > B.Length) ? A.Length : B.Length;
        for(u32 Index = 0;Index < MaxIterations;++Index)
        {
            if (*APtr != *BPtr)
                return false;
            
            APtr++; BPtr++;
        }
    }
    return true;
}

//TODO(ray): Make sure this is never used in game.
APIDEF void PrintStringToConsole(string String)
{
    //for(u32 CharIndex = 0;CharIndex < String.Length;++CharIndex)
    {
        //char* Char = (String.String + CharIndex);
        printf("%.*s", String.Length, String.String);
        //std::cout << *Char;
    }
}

APIDEF string* GetExtension(string* FileNameOrPathWithExtension,memory_partition *StringMem,b32 KeepFileExtensionDelimiter = false)
{
    Assert(FileNameOrPathWithExtension->Length > 1)
    
        //walk back from end of string till we hit a '.'
    char* End = FileNameOrPathWithExtension->String + FileNameOrPathWithExtension->Length - 1;
    u32 LookBack = 1;
    if(KeepFileExtensionDelimiter)
    {
        LookBack = 0;
    }
    u32 StepsTaken = 1;
    while (*(End - LookBack) != '.')
    {
        --End;
        ++StepsTaken;
        if (StepsTaken > MAX_FILE_EXTENSION_LENGTH)
        {
            //TODO(ray):Log this as an error?
            break;
        }
    }
    string* ExtensionName = CreateStringFromLength(End, StepsTaken, StringMem);
    return ExtensionName;
}
APIDEF string* StripExtension(string* FileNameOrPathWithExtension,memory_partition *StringMem)
{
    Assert(FileNameOrPathWithExtension->Length > 1)
    
        //walk back from end of string till we hit a '.'
    char* End = FileNameOrPathWithExtension->String + FileNameOrPathWithExtension->Length - 1;
    u32 StepCount = 1;
    while (*End != '.')
    {
        --End;
        if (StepCount > MAX_FILENAME_LENGTH)
        {
            //TODO(ray):Log this as an error?
            break;
        }
    }
    return CreateStringFromToChar(&FileNameOrPathWithExtension->String[0], &End[0], StringMem);
}

APIDEF string* StripAndOutputExtension(string* FileNameOrPathWithExtension,string* Extension,memory_partition *StringMem,b32 KeepFileExtensionDelimeter = false)
{
    Assert(FileNameOrPathWithExtension->Length > 1)
    
        string* Result = StripExtension(FileNameOrPathWithExtension, StringMem);
    string* ExtensionName = GetExtension(FileNameOrPathWithExtension, StringMem,KeepFileExtensionDelimeter);
    //string TerminatedExtensionName = NullTerminate(*ExtensionName);
    *Extension = *ExtensionName;
    return Result;
}

APIDEF string* AppendString(string Front,string Back,memory_partition* Memory)
{
    string *Result = PushStruct(Memory,string);
    void* StartPointer = GetPartitionPointer(*Memory);
    char* StrPtr;
    char* At = Front.String;
    u32 Iterations = 0;
    while(*At && Iterations < Front.Length)
    {
        StrPtr = (char*)PushSize(Memory,1);
        *StrPtr = *At;
        Result->Length++;
        At++;
        Iterations++;
    }
    At = Back.String;
    Iterations = 0;
    while(*At && Iterations < Back.Length)
    {
        StrPtr = (char*)PushSize(Memory,1);
        *StrPtr = *At;
        Result->Length++;
        At++;
        Iterations++;
    }
    Result->String = (char*)StartPointer;
    *Result = NullTerminate(*Result);
    return Result;
}

APIDEF string* ElementIterator(fixed_element_size_list *Array)
{
    string* Result;
    
    if(Array->Head->IsSentinal)
    {
        Array->Head = Array->Head->Next;
        Result = *Array->Head->Data;
        
        return Result;
    }
    else
    {
        Array->Head = Array->Head->Next;
        if(!Array->Head)
        {
            return 0;
        }
        Result = *Array->Head->Data;
        
        return Result;
    }
    //return 0;
}
#include "api_tokenizer.h"

APIDEF string* GetFromStringsByIndex(strings Strings, u32 Index)
{
    Assert(Strings.StringCount > Index)
        return (Strings.Strings + Index);
}

APIDEF strings API_String_Split(string Source,char* Separator,duel_memory_partition* Memory)
{
    strings Result = {0};
    Source = NullTerminate(Source);
    char* At = Source.String;
    char* Start  = At;
    b32 HasLastString = false;
    string* StringStart;
    u32 CharIndex = 0;
    while(*At++ && Source.Length > CharIndex)
    {
        HasLastString = true;
        if(*At == *Separator)
        {
            while(IsWhiteSpace(*Start))
            {
                Start++;
            }
            u32 MovedBackCount = 0;
            while(IsWhiteSpace(*(At - 1)))
            {
                MovedBackCount++;
                At--;
            }
            if(Result.StringCount == 0)
            {
                Result.Strings = API_CreateStringFromToPointer_WithSplitMem(Start, At++, Memory);;
            }
            else
            {
                //TODO(Ray):After looking throught his i seen i left there here i think we can
                //safe remove this.
                string* R = API_CreateStringFromToPointer_WithSplitMem(Start, At++, Memory);
            }
            Result.StringCount++;
            At = At + MovedBackCount;
            Start = At;
            HasLastString = false;
        }
        CharIndex++;
    }
    if (HasLastString)
    {
        while(IsWhiteSpace(*Start))
        {
            Start++;
        }
        u32 MovedBackCount = 0;
        while(IsWhiteSpace(*(At - 1)))
        {
            MovedBackCount++;
            At--;
        }
        if (Result.StringCount == 0)
        {
            
            Result.Strings = API_CreateStringFromToPointer_WithSplitMem(Start, At, Memory);
            StringStart = Result.Strings;
        }
        else 
        {
            string* P = API_CreateStringFromToPointer_WithSplitMem(Start, At, Memory);
        }
        Result.StringCount++;
    }
    return Result;
}

APIDEF string* API_String_Iterator(strings* StringArray)
{
    Assert(StringArray->StringCount > 0)
        Assert(StringArray->Strings)
    
        if(StringArray->IteratorIndex > StringArray->StringCount - 1)
    {
        StringArray->IteratorIndex = 0;
        return 0;
    }
    else{
        string* Result = StringArray->Strings + StringArray->IteratorIndex;
        StringArray->IteratorIndex++;
        return Result;
    }
}


//TODO(ray): Old function can do this much better.  REDO THIS!
//TODO(ray): This will fail in the case there is no seperator present in the string.
//Note(ray): The data type fixed_element... does not make sense should rename rework.
APIDEF fixed_element_size_list SplitString(string Source,char* Separator,memory_partition *Partition,bool SeparatorIsNotLastChar = false)
{
    
    fixed_element_size_list Result = {0};
    Result.UnitSize = sizeof(string);
    
    char* At = Source.String;
    char* CurrentStart = Source.String;
    u32 Length = 0;
    u32 StringIndex = 0;
    while(*At && StringIndex < Source.Length)
    {
        if(*Separator == *At)
        {
            
            if(Result.Length == 0)
            {
                string** Temp = (string**)PushSize(Partition,sizeof(string**));
                
                fixed_element* Element = (fixed_element*)PushStruct(Partition,fixed_element);
                fixed_element* SentinalElement = (fixed_element*)PushStruct(Partition,fixed_element);
                Element->IsSentinal = false;
                
                *Temp = CreateStringFromLength(CurrentStart,Length,Partition);
                CurrentStart = At + 1;
                At++;//move past the separator
                StringIndex++;
                
                SentinalElement->IsSentinal = true;
                Result.Sentinal = SentinalElement;
                Element->Data = Temp;
                
                Result.Head = Result.Sentinal;
                Result.Head->Next = Element;
            }
            else
            {
                string** Temp = (string**)PushSize(Partition,sizeof(string**));
                fixed_element* Element = (fixed_element*)PushStruct(Partition,fixed_element);
                *Temp = CreateStringFromLength(CurrentStart,Length,Partition);
                Element->IsSentinal = false;
                
                CurrentStart = At+1;
                At++;//Move past the separator
                StringIndex++;
                
                Element->Data = Temp;
                
                Result.Head = Result.Head->Next;
                Result.Head->Next = Element;
                
            }
            
            Result.Length++;
            Length = 0;
        }
        StringIndex++;
        At++;
        Length++;
    }
    if(SeparatorIsNotLastChar)
    {
        string** Temp = (string**)PushSize(Partition,sizeof(string**));
        fixed_element* Element = (fixed_element*)PushStruct(Partition,fixed_element);
        Element->IsSentinal = false;
        
        if(Result.Length == 0)
        {
            fixed_element* SentinalElement = (fixed_element*)PushStruct(Partition,fixed_element);
            Element->IsSentinal = false;
            
            *Temp = CreateStringFromLength(CurrentStart,Length,Partition);
            
            SentinalElement->IsSentinal = true;
            Result.Sentinal = SentinalElement;
            Element->Data = Temp;
            
            Result.Head = Result.Sentinal;
            Result.Head->Next = Element;
        }
        else
        {
            *Temp = CreateStringFromLength(CurrentStart,Length,Partition);
            Element->Data = Temp;
            
            Result.Head = Result.Head->Next;
            Result.Head->Next = Element;
        }
        Result.Length++;
    }
    Result.Head = Result.Sentinal;
    return Result;
}

#define MAX_FORMAT_STRING_SIZE 500
APIDEF string* FormatToString(char* StringBuffer,memory_partition* StringMemory)
{
    string* Result;
    char CharBuffer[MAX_FORMAT_STRING_SIZE];
#if OSX
    sprintf(CharBuffer, StringBuffer);
#elif IOS
    sprintf(CharBuffer, StringBuffer);
#elif WINDOWS
    sprintf_s(CharBuffer,StringBuffer);
#endif
    Result = CreateStringFromLiteral(CharBuffer,StringMemory);
    return Result;
}
#include <stdarg.h>
//TODO(ray): Move this to a more proper place replace std::out
APIDEF void PlatformOutputToConsole(b32 UseToggle,const char* FormatString, u32 __Dummy, ...)
{
    if (UseToggle)
    {
        va_list List;
        va_start(List, __Dummy);
        
        char TextBuffer[100];
#if WINDOWS
        vsprintf_s(TextBuffer,
                   FormatString, List);
      std::cout << TextBuffer << std::endl;
//        OutputDebugStringA(TextBuffer);
#elif OSX
        //NOTE(ray):Untested.....
        vsprintf(TextBuffer, FormatString, List);
        std::cout << TextBuffer << std::endl;
#endif
        
        va_end(List);
    }
}

#define API_STRINGS_H
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
