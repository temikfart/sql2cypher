CREATE TABLE test_table_1 (
    test_column int,
    PRIMARY KEY (test_column)
);
    CREATE TABLE test_table_2 (
    test_column_FK int,
    CONSTRAINT test_constraint_FK
        FOREIGN KEY (test_column_FK)
            REFERENCES test_table_1 (test_column)
);
