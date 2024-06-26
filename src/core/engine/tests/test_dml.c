#include <stdlib.h>
#include <stdio.h>

#include "../DDL/create.h"
#include "../DDL/drop.h"
#include "../DML/insert.h"
#include "../DML/select.h"

#include "../column.h"
#include "../tablespace.h"

char* ColumnEnumMapper[] = {"QF_INT", "QF_UINT", "QF_CHAR", "QF_FLOAT", "QF_TEXT"};


int main() {
    printf("CREATE TABLE users (uid INT, name TEXT, age TEXT, salary UINT, exp UINT);\n");

    create(
        "users",
        (char*[]) {"uid", "name", "surname", "age", "salary", "exp"},
        (enum ColumnsTypes[]) {QF_INT, QF_TEXT, QF_TEXT, QF_UINT, QF_UINT, QF_UINT},
        (size_t) 6, false
    );

    printf("CREATE TABLE transactions (ID INT, username TEXT, value UINT,  UINT, date TEXT);\n");

    create(
        "transactions",
        (char*[]) {"ID", "username", "value", "date"},
        (enum ColumnsTypes[]) {QF_INT, QF_TEXT, QF_UINT, QF_TEXT},
        (size_t) 6, false
    );

    // ------------------------------------------------------------------------------------
    table_t* users = select("users", (char*[]){"uid", "name"});

    printf("\n");
    printf("Table: %s\n", table->t_name);
    printf("---------------------------------\n");
    printf("|\tcolumn\t|\ttype\t|\n");
    printf("---------------------------------\n");
    for (size_t i = 0; i < table->cols; ++i) {
        printf("|\t%s\t|\t%s\t|\n", table->columns[i]->c_name, ColumnEnumMapper[table->columns[i]->c_stype]);
    }
    printf("---------------------------------\n");
    // ------------------------------------------------------------------------------------

    printf("\nDROP TABLE users;\n");
    drop("users");
    
    return 0;
}