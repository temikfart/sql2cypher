CREATE TABLE test_table_1 (
    test_column char,
    CONSTRAINT test_constraint_PK
        PRIMARY KEY (test_column)
);
CREATE TABLE test_table_2 (
    test_column_1 int,
    test_column_2 char,
    test_column_3 varchar,
    PRIMARY KEY (test_column_1, test_column_3),
    CONSTRAINT test_constraint_FK
        FOREIGN KEY (test_column_2)
            REFERENCES test_table_1 (test_column)
);
