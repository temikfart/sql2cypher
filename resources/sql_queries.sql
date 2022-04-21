CREATE DATABASE my_db_1;
CREATE DATABASE my_db_2;
CREATE DATABASE my_db_3;
CREATE TABLE my_table_1 (
                            my_col_1_1 integer,
                            my_col_1_2 varchar,
                            my_col_1_3 char,
                            my_col_1_4 float,
                            PRIMARY KEY (my_col_1_1),
                            CONSTRAINT my_constraint_1 PRIMARY KEY (my_col_1_2)
);
CREATE TABLE my_table_2 (
                            my_col_2_1 integer,
                            my_col_2_2 varchar,
                            my_col_2_3 char,
                            PRIMARY KEY (my_col_2_1),
                            FOREIGN KEY (my_col_2_2) REFERENCES my_table_1 (my_col_1_2),
                            FOREIGN KEY (my_col_2_3) REFERENCES my_table_1
);
CREATE TABLE my_table_3 (
    my_col_3_1 integer
);
ALTER TABLE my_table_1
    ADD
        my_col_1_5 integer,
        my_col_1_6 varchar,
        CONSTRAINT my_constraint_2 PRIMARY KEY (my_col_1_5),
        PRIMARY KEY (my_col_1_5)
;
ALTER TABLE my_table_1
DROP
COLUMN my_col_1_5, my_col_1_6
;
DROP TABLE my_table_1;
DROP TABLE my_table_2, my_table_3;
DROP DATABASE my_db_1;
DROP DATABASE my_db_2, my_db_3;
