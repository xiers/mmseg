/*
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
 */


#if !defined(_DICTSCHEMA_H)
#define _DICTSCHEMA_H
#include "csr_typedefs.h"
#include "mm_dict_schema_column.h"

namespace mm {

class EntryData;

class DictSchema {
public:
    int InitString(const char* schema_define);
    std::string GetColumnDefine();
    const DictSchemaColumn& GetColumn(u2 idx);
    void SetDefault(const EntryData& entry_default);
    const DictSchemaColumn& GetColumn(const char* column_name);
	u4 GetSize();
	u2 GetEntryDataSize();

	// if some column missing, @return will >0, the count is the missing columns.
    // if no suite column found, will return 0; the mask used by select data, eg. select id, pinyin from dictionary ...
	int GetFieldMask(string columns, u2* mask);

protected:
    std::vector<DictSchemaColumn> _columns;
};

} // namespace mm



#endif  //_DICTSCHEMA_H
