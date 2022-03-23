# SCC
---
### SQL to Cypher QL Converter

**SCC** is a tool, which allows you to migrate data
from the relational to the graph database.
As the relational database language was chosen Microsoft SQL
for the [Microsoft SQL Server](https://www.microsoft.com/en-gb/sql-server/sql-server-2019?rtc=2).
The MS SQL is translated to the Cypher Query Language for the 
[neo4j](https://neo4j.com/) graph database.

It is important to note, that **SCC** translates one query per cycle.
It means, that our converter doesn't analyze all queries to know more
about your database structure.
Every query from the input file is translated apart from each other.
Keep this in mind when using our product.

#### Table of contents

1. [Opportunities. What queries can be converted?](#Opportunities.-What-queries-can-be-converted?)
2. [Operating principle](#Operating-principle)
3. [Installation](#Installation)
   1. [Linux](#Linux)
   2. [Windows](#Windows)
   3. [MacOS](#MacOS)
4. [Launch](#Launch)
   1. [First run](#First-run)
   2. [Usage](#Usage)
      1. [Configuration via file](#Configuration-via-file)
      2. [Configuration via console flags](#Configuration-via-console-flags)
5. [How to conduct testing?](#How-to-conduct-testing?)
   1. [GoogleTest](#GoogleTest)
   2. [Write your own unit-tests](#Write-your-own-unit-tests)
6. [Documentation](#Documentation)
7. [Authors](#Authors)
8. [Mentors](#Mentors)


## Opportunities. What queries can be converted?

---

At first, we planned, that the **SCC** will convert standard DDL and DML queries.
It can be the simplest query, for example:
```SQL
CREATE DATABASE [Zoo];
```
or
```SQL
CREATE TABLE [Aviaries] (
    [aviary_id] integer NOT NULL,
    [name] nvarchar(50) NOT NULL,
    PRIMARY KEY ([aviary_id])
)
```
as DDL queries. Also. you will have a possibility to convert the DML queries.
For example:
```SQL
ALTER TABLE [Aviaries] ADD [zone_id] integer;
```
etc.

## Operating principle

---
_Text..._

## Installation

---
We plan to make portable versions of the **SCC** to the all popular OS
along with accompanying installation instructions.

### Linux
_Text..._
### Windows
_Text..._
### MacOS
_Text..._

## Launch

---
### First run
Our project can be compiled via CMake. Use the following steps:
1. Build the **SCC**:
```shell
SCC/ $ cmake -S . -B build
```
2. Run the converter:
```shell
SCC/ $ cd build/
SCC/build/ $ ./SCC
```
You will see the following **INFO** messages:
```
[INFO]  <Time> main.cpp: main(): 4: Starting system...
[INFO]  <Time> main.cpp: main(): 6: System started
```

### Usage
The **SCC** can be configured by the `config.ini` file in the root directory
and via console flags. Let's look at these opportunities.

#### Configuration via file
_Text..._

#### Configuration via console flags
```
-h, --help          Prints Usage and information about acceptable flags.
-v, --version       Prints current version.
-i, --interactive   Starts the SCC in interactive mode.
                    (logs will be printed in console)
-d, --daemon        Starts the SCC in daemon mode.
                    (logs will be printed in the special log files into "log/")
--log=lvl           Sets logging level to "lvl".
                    Acceptable levels: FATAL, ERROR, INFO, TRACE, DEBUG.
--sql=path          Pointer to the file with SQL queries, which will be converted.
--cypher=path       Pointer to the file, where will be converted cypher queries.
```


## How to conduct testing?

---
### GoogleTest
We use [GoogleTest](https://github.com/google/googletest) 
framework for the unit-testing.
Go to the `build` folder, which was created previously,
and run the following command:
```shell
SCC/build/ $ ctest
```
you will see something like this:
```
Test project /path/to/sql2cypher/build
      Start  1: SetGetConfigTests.DefaultConfigTest
 1/17 Test  #1: SetGetConfigTests.DefaultConfigTest ..........   Passed    0.02 sec
      Start  2: SetGetConfigTests.ValidModeTest
 2/17 Test  #2: SetGetConfigTests.ValidModeTest ..............   Passed    0.02 sec
...
      Start 17: AddLogTests.AddInvalidLogTest
17/17 Test #17: AddLogTests.AddInvalidLogTest ................   Passed    0.04 sec

100% tests passed, 0 tests failed out of 16

Total Test time (real) =   0.47 sec
```
You can learn GoogleTest via this 
[User Guide](https://google.github.io/googletest/).

### Write your own unit-tests
If you want to test some feature, you should to write your own tests
and put your test file into the `test/` directory.
Don't forget to change `CMakeLists.txt`!

## Documentation

---
We plan to write some Wiki pages on the GitHub
and use Doxygen for generating documentation.
We think, it will be useful for you.

## Authors

---

Artyom Fartygin ([temikfart](https://github.com/temikfart)) —
Undergraduate student at [MIPT](https://mipt.ru) [PAMI](https://mipt.ru/education/departments/fpmi/)

Roman Korostinskiy ([RomanKorostinskiy](https://github.com/RomanKorostinskiy)) —
Undergraduate student at [MIPT](https://mipt.ru) [PRCT](https://mipt.ru/education/departments/frkt/)

## Mentors

---

Nikolay Efanov ([nefanov](https://github.com/nefanov)) —
Associate professor at [MIPT](https://mipt.ru)
