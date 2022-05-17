# SCC
[![Build test](https://github.com/temikfart/sql2cypher/actions/workflows/build-test.yml/badge.svg)](https://github.com/temikfart/sql2cypher/actions/workflows/build-test.yml)
---
### SQL to Cypher QL Converter

**SCC** is a tool, which allows you to migrate data
from the relational to the graph database.
As the relational database language was chosen Microsoft SQL
for the [Microsoft SQL Server](https://www.microsoft.com/en-gb/sql-server/sql-server-2019?rtc=2).
The MS SQL is translated to the Cypher Query Language for the 
[neo4j](https://neo4j.com/) graph database.

It is important to note, that **SCC** translates one file with queries per 
cycle. It means, that our converter doesn't analyze all queries to know more
about your database structure.
Every query from the input file is translated apart from each other.
Keep this in mind when using our product.
h
#### Table of contents

1. [Opportunities. What queries can be converted?](#Opportunities.-What-queries-can-be-converted)
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
5. [How to conduct testing?](#How-to-conduct-testing)
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
CREATE DATABASE Zoo;
```
or
```SQL
CREATE TABLE Aviaries (
    aviary_id integer,
    square float,
    PRIMARY KEY (aviary_id)
);
CREATE TABLE Zones (
    zone_id integer,
    PRIMARY KEY (zone_id)
);
```
or
```SQL
ALTER TABLE Aviaries
    ADD
        zone_id integer,
        CONSTRAINT fk_aviaries_zones
        FOREIGN KEY (aviary_id)
        REFERENCES Zones (zone_id);
```
as DDL queries. Also, we plan to implement translation for the DML queries.

## Operating principle

---
Every SQL query from the input file goes through the following path:
1. **Tokenization** _(Parsing on the tokens with own type, like as `word`, `operator`, etc.);_

This step is the common parsing.
The query will be introduce as tokens array at the end of tokenization.

3. **Syntax Analyses** _(Analyzer introduces tokens array into AST);_

**SCC** analyses tokens array via recursive descent and builds AST of the query.

4. **AST handler** _(Preparing the AST to translation);_

_Notice: for now, this module is not implemented._

It is an intermediate step between the Analysis and Translation.
The handler leads the AST to the correct introduction for the translation.

5. **Query Assembler** _(AST analysing and translation into CQL)._

Translation.
The Assembler analyses the AST and searches for constructions for translation.

## Installation

---
We plan to make portable versions of the **SCC** to the all popular OS
along with accompanying installation instructions.

### Linux
1. Go to the Release page and download a `.deb` package;
2. Go to package directory and use the following command for installation:
```shell
$ sudo dpkg -i package_name.deb
```
This command will install the SCC on your linux.
3. Use the SCC with
```shell
$ scc -v
```
You will see the following message:
```shell
$ scc -v
<===| SCC (The SQL to CypherQL Converter) |===>
Version:     0.9.0
Developers:  Artyom Fartygin, Roman Korostinskiy
```
> Save your time with the SCC!
### Windows
1. Go to the Release page and download a `.exe` package;
2. Go to package directory and run installer. You will see NSIS window with
with the installation steps;
3. Use the SCC with the Windows' command line:
```shell
$ scc -v
```
You will see the following message:
```shell
$ scc -v
<===| SCC (The SQL to CypherQL Converter) |===>
Version:     0.9.0
Developers:  Artyom Fartygin, Roman Korostinskiy
```
> Save your time with the SCC!
### MacOS
For now, the SCC can't be installed in the MacOS via installer, but you can use
local application. See [**Launch**](#Launch).

## Launch

---
### First run
Our project can be compiled via CMake. Use the following steps:
1. Build the **SCC**:
```shell
sql2cypher/ $ cmake -S . -B build [options]
```
where options:

* `-DPACKAGE=0`, if you want to use the SCC locally;
* `-DPACKAGE=1`, if you want to create package;

If you will create a package, you can manually set an OS:
* `-DWIN32=1` for Windows package (`.exe`). With this option you must have
`MinGW` and add new option: `-G "MinGW Makefiles"`;
* `-DUNIX=1` for Linux package (`.deb`).
2. Run the converter locally:
```shell
sql2cypher/ $ cd build/
sql2cypher/build/ $ ./SCC
```
You will see the following **INFO** messages:
```
[INFO]  <Time> main.cpp: main(): 4: Starting system...
[INFO]  <Time> main.cpp: main(): 6: System started
```
3. If you are creating a package for Linux or Windows you should go to `build`
directory and run the following command:
```shell
sql2cypher/build/ $ make package
```
You will see new file with `.deb` or `.exe` extension.

### Usage

The **SCC** can be configured by the `config.ini` file in the root directory
and via console flags. Let's look at these opportunities.

#### Configuration via file

_Notice: will be implemented in the future patches._

#### Configuration via console flags
```
Usage:
   scc --sql=file1 --cypher=file2 [options]...
Options:
-h, --help          Prints Usage and information about acceptable flags.
-v, --version       Prints current version.
-m, --mode=[mode]   Starts the SCC in special mode (by default: interactive)
-i, --interactive   Starts the SCC in interactive mode.
--dump=[path]       Creates Tree Dump image of the AST in the [path].
-l, --loglvl=lvl    Sets logging level to "lvl".
                    Acceptable levels: SILENT, FATAL, ERROR, INFO, TRACE, DEBUG.
--logdir=[path]     Path to the log directory, where will be created log file
--sql=[path]        Path to the file with SQL queries, which will be converted (SQL).
--cypher=[path]     Path to the file, where will be converted queries (CQL).
--dump=[path]       Path to the file, where will be created image of AST of SQL queries
                    (by default, the image is not created)
```


## How to conduct testing?

---
### GoogleTest
We use [GoogleTest](https://github.com/google/googletest) 
framework for the unit-testing.
Go to the `build` folder, which was created previously,
and run the following command:
```shell
sql2cypher/build/ $ ctest
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

For now you can generate docs with Doxygen. Go to the root of the app directory
and use the following command:
```shell
sql2cypher/ $ doxygen doc/doxygen-config.dox
```

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
