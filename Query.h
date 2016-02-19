/**
 *	Query.h
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2016  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#ifndef _QUERY_H_SQLITE
#define _QUERY_H_SQLITE

#include <sqlite3.h>

#include <iostream>
#include <ctime>
#include <string>
#include <map>
#include <vector>

#include <stdint.h>


namespace SQLW
{
    /** SQL Statement execute / result. */
    class Query
    {
    public:
        /** Constructor accepting reference to database object. */
        Query(Database& dbin);
        /** Constructor accepting reference to database object
        	and query string to execute. */
        Query(Database& dbin,const std::string& sql);
        ~Query();

        /** Check if database object is connectable. */
        bool isConnected();
        /** Return reference to database object. */
        Database& getDatabase() const;

        /** Return string containing last query executed. */
        const std::string& getLastQuery();

        /** execute() returns true if query is successful,
        	does not store result. */
        bool execute(const std::string& sql);

        /** Execute query and store result. */
        sqlite3_stmt *getResult(const std::string& sql);
        /** Free stored result, must be called after get_result() before calling
        	execute()/get_result() again. */

        void freeResult();
        /** Fetch next result row.
        	\return false if there was no row to fetch (end of rows) */

        // Fill the Resource with the current row, also incriments to next row.
        bool fetchRow();

        /** Get id of last insert. */
        sqlite_int64 getInsertId();

        /** Returns 0 if there are no rows to fetch. */
        long getNumRows();

        /** Number of columns in current result. */
        int getNumCols();

        /** Last error string. */
        std::string GetError();

        /** Last error code. */
        int GetErrno();

        /** Check if column x in current row is null. */
        bool isNull(int x);

        /**
         * @brief Execute Query and Get first Result as the following.
         */

        /** Execute query and return first result as a string. */
        const char *exeGetCharString(const std::string& sql);

        /** Execute query and return first result as a long integer. */
        long exeGetResultLong(const std::string& sql);

        /** Execute query and return first result as a double. */
        double exeGetResultDouble(const std::string& sql);

        /**
         * @brief On FetchRow, get the current column values as.
         */

        /** Return column named x as a string value. */
        const char *getstr(const std::string& x);

        /** Return column x as a string value. */
        const char *getstr(int x);

        /** Return next column as a string value - see rowcount. */
        const char *getstr();

        /**
         * @brief On FetchRow, get the current column values as.
         */

        /** Return column named x as a long integer. */
        long getval(const std::string& x);

        /** Return column x as a long integer. */
        long getval(int x);

        /** Return next column as a long integer - see rowcount. */
        long getval();

        /**
         * @brief On FetchRow, get the current column values as.
         */

        /** Return column named x as an unsigned long integer. */
        unsigned long getuval(const std::string& x);

        /** Return column x as an unsigned long integer. */
        unsigned long getuval(int x);

        /** Return next column as an unsigned long integer. */
        unsigned long getuval();

        /**
         * @brief On FetchRow, get the current column values as.
         */

        /** Return column named x as a 64-bit integer value. */
        int64_t getbigint(const std::string& x);

        /** Return column x as a 64-bit integer value. */
        int64_t getbigint(int x);

        /** Return next column as a 64-bit integer value. */
        int64_t getbigint();

        /** Return column named x as an unsigned 64-bit integer value. */
        uint64_t getubigint(const std::string& x);

        /** Return column x as an unsigned 64-bit integer value. */
        uint64_t getubigint(int x);

        /** Return next column as an unsigned 64-bit integer value. */
        uint64_t getubigint();

        /**
         * @brief On FetchRow, get the current column values as.
         */

        /** Return column named x as a double. */
        double getnum(const std::string& x);

        /** Return column x as a double. */
        double getnum(int x);

        /** Return next column as a double. */
        double getnum();

        /**
         * @brief On FetchRow, Get Field Value by Column Name and Type
         */

        // Int
        int getFieldValue(int type, int index)
        {
            UNUSED(type);
            int result = static_cast<int>(sqlite3_column_int(res, index));
            return result;
        }

        // Long or Time_T for Date/Time.
        long getFieldValue(long type, int index)
        {
            UNUSED(type);
            long result = static_cast<long>(sqlite3_column_int64(res, index));
            return result;
        }

        // Long Long
        long long getFieldValue(long long type, int index)
        {
            UNUSED(type);
            long long result = static_cast<long long>(sqlite3_column_int64(res, index));
            return result;
        }

        // Double
        double getFieldValue(double type, int index)
        {
            UNUSED(type);
            double result = static_cast<double>(sqlite3_column_double(res, index));
            return result;
        }

        // Float
        float getFieldValue(float type, int index)
        {
            UNUSED(type);
            float result = static_cast<float>(sqlite3_column_double(res, index));
            return result;
        }

        // Long Double
        long double getFieldValue(long double type, int index)
        {
            UNUSED(type);
            long double result = static_cast<long double>(sqlite3_column_double(res, index));
            return result;
        }

        // Std::String
        std::string getFieldValue(std::string type, int index)
        {
            UNUSED(type);

            std::string newString = "";
            try
            {
                const char *text_result = reinterpret_cast<const char *>(sqlite3_column_text(res , index));
                newString = text_result;
                return newString;
            }
            catch (std::exception ex)
            {
                std::string message = "string getFieldValue: ";
                message.append(ex.what());
                queryError(message.c_str());
            }
            return newString;
        }

        // Single Character
        char getFieldValue(char type, int index)
        {
            UNUSED(type);
            const char *result = reinterpret_cast<const char *>(sqlite3_column_text(res , index));
            return result[0];
        }

        // Char *, or BLOB. sqlite3_column_blob ?  test this lateron!
        const char *getFieldValue(char *type, int index)
        {
            UNUSED(type);
            /*
            if (strcmp(type, "BLOB") == 0)
            {
                // Test if we need to get len in bytes then cutoff!
                const char *result = reinterpret_cast<const char *>(sqlite3_column_blob(res , index));
                return result;
            }
            else
            {*/
            const char *result = reinterpret_cast<const char *>(sqlite3_column_text(res , index));
            return result;
            //}
            //return "\0";
        }

        // Handle Boolean Values.
        bool getFieldValue(bool type, int index)
        {
            UNUSED(type);
            int result = static_cast<int>(sqlite3_column_int(res , index));
            return (result == 1) ? true : false;
        }

        /**
        * @brief Template to Get Field Value by Column Name and Type.
        */
        template <typename TT, typename T>
        T getFieldByName(TT tt, T t)
        {
            // Grab the index of the Matching Field Name
            int index = m_nmap[tt] - 1;

            T result;
            switch(sqlite3_column_type(res, index))
            {
                case SQLITE_NULL:
                    return t;
                    break;

                default :
                    // Template to pull field value dynamically from overloads.
                    result = getFieldValue(t, index);
                    break;
            }
            return result;
        }

    private:
        /** Hide the copy constructor. */
        Query(const Query& q)
            : m_db(q.getDatabase())
        {
            std::cout << "Query Constructor: q.getDatabase()" << std::endl;
        }

        /** Hide the assignment operator. */
        Query& operator=(const Query&)
        {
            std::cout << "Query Constructor: return *this" << std::endl;
            return *this;
        }

        /** Print current result to stdout. */
        void printResults();

        /** Print error to debug class. */
        void queryError(const std::string&);


        Database&                  m_db;           ///< Reference to database object
        Database::DatabasePool    *odb;            ///< Connection pool handle

        sqlite3_stmt              *res;            ///< Stored result
        bool                       row;            ///< true if fetch_row succeeded
        short                      rowcount;       ///< Current column pointer in result
        std::string                m_tmpstr;       ///< Used to store result in get_string() call
        std::string                m_last_query;   ///< Last query executed
        int                        cache_rc;       ///< Cached result after call to get_result()
        bool                       cache_rc_valid; ///< Indicates cache_rc is valid
        int                        m_row_count;    ///< 0 if get_result() returned no rows

        std::map<std::string, int> m_nmap;         ///< map translating column names to index
        int                        m_num_cols;     ///< number of columns in result

    };

} // namespace SQLW


#endif // _QUERY_H
