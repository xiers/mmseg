﻿/*
 * Copyright 2014 Li Monan <limn@coreseek.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY CYBOZU LABS, INC. ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL CYBOZU LABS, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Cybozu Labs, Inc.
 *
 */


#include "stringpool_memory.h"

namespace mm {

int StringPoolMemory::STATUS_OK = 0;
int StringPoolMemory::STATUS_STRING_TOO_BIG = -413;

StringPoolMemory::StringPoolMemory():_current(NULL){
    /*
     * 简化的 String Pool , 可修改的内存形态.
     */
    _begin = new StringPoolMemoryEntry();
    _current = _begin;
    _total_size = 0;
}

StringPoolMemory::~StringPoolMemory(){
    StringPoolMemoryEntry* entry_ptr = _begin;
    do {
        if(entry_ptr->_ptr)
            free(entry_ptr->_ptr);
        StringPoolMemoryEntry* prev_ptr = entry_ptr;
        entry_ptr = entry_ptr->_next;
        delete prev_ptr;
    }while(entry_ptr);

    _begin = NULL;
}

i4 StringPoolMemory::AllocString(const char* buf, u2 length){
    /*
     *  1 如果当前活跃的 pool 有空闲, 则分配之;
     *  2 如果没有空闲, 这分配新的 entry, 长度不能大于 65534,
     *
     *  假定是用于词典等大量短句的应用. return the offset of string in a continue string pool.
     */
    if(length >= STRING_POOL_ENTRY_DATA_SIZE - 2)
        return STATUS_STRING_TOO_BIG;
    // check current pool size.
    if((u2)(_current->_size - _current->_used) < length + 2) {
        // create new entry.
        MakeNewEntry();
    }
    i4 ret_offset = _total_size;
    {
        // make new string.
        _current->push_string(buf, length);
        _total_size += sizeof(u2)+length;
    }
    return ret_offset;
}

// return the string's ptr by string 's offset
const char* StringPoolMemory::GetString(u4 offset, u4* data_length){
    /*
     *  offset 处指示的格式
     *  数据长度[2b] 实际数据
     */
    return NULL;
}

u4 StringPoolMemory::GetSize(){
    return 0;
}

int StringPoolMemory::Dump(u1* ptr, u4 size) {
    return 0;
}

} // namespace mm