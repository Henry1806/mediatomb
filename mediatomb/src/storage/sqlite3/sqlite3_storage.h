/*  sqlite3_storage.h - this file is part of MediaTomb.

    Copyright (C) 2005 Gena Batyan <bgeradz@deadlock.dhs.org>,
                       Sergey Bostandzhyan <jin@deadlock.dhs.org>

    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __SQLITE3_STORAGE_H__
#define __SQLITE3_STORAGE_H__

#include <sqlite3.h>

#include "storage/sql_storage.h"
#include "sync.h"

class Sqlite3Storage;
class Sqlite3Result;

class Sqlite3Row : public SQLRow
{
public:
    Sqlite3Row(char **row);
    virtual zmm::String col(int index);
protected:
    char **row;
    zmm::Ref<Sqlite3Result> res;
    friend class Sqlite3Result;
};

class Sqlite3Result : public SQLResult
{
public:
    Sqlite3Result();
    virtual ~Sqlite3Result();
    virtual zmm::Ref<SQLRow> nextRow();
protected:
    char **table;
    char **row;

    int cur_row;

    int nrow;
    int ncolumn;

    zmm::Ref<Sqlite3Storage> storage;
    
    friend class Sqlite3Storage;
};


void unlock_func(void *data);

class Sqlite3Storage : public SQLStorage
{
public:
    Sqlite3Storage();
    virtual ~Sqlite3Storage();

    virtual void init();
    virtual zmm::String quote(zmm::String str);
    virtual zmm::Ref<SQLResult> select(zmm::String query);
    virtual void exec(zmm::String query);
    virtual int lastInsertID();

protected:
    sqlite3 *db;

    zmm::Ref<Mutex> mutex;
    
    void reportError(zmm::String query);

    inline void lock() { mutex->lock(); }
    inline void unlock() { mutex->unlock(); }

    friend void unlock_func(void *data);
};


#endif // __SQLITE3_STORAGE_H__
