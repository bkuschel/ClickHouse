#include <IO/WriteBufferFromOStream.h>
#include <Parsers/ASTCreateQuery.h>
#include <Parsers/ASTFunction.h>
#include <Parsers/ASTIdentifier.h>
#include <Parsers/ParserAlterQuery.h>
#include <Parsers/ParserCreateQuery.h>
#include <Parsers/ParserOptimizeQuery.h>
#include <Parsers/ParserQueryWithOutput.h>
#include <Parsers/formatAST.h>
#include <Parsers/parseQuery.h>

#include <string_view>

#include <gtest/gtest.h>

namespace
{
using namespace DB;
using namespace std::literals;
}

struct ParserTestCase
{
    const std::string_view input_text;
    const char * expected_ast = nullptr;
};

std::ostream & operator<<(std::ostream & ostr, const std::shared_ptr<IParser> parser)
{
    return ostr << "Parser: " << parser->getName();
}

std::ostream & operator<<(std::ostream & ostr, const ParserTestCase & test_case)
{
    return ostr << "ParserTestCase input: " << test_case.input_text;
}

class ParserTest : public ::testing::TestWithParam<std::tuple<std::shared_ptr<IParser>, ParserTestCase>>
{};

TEST_P(ParserTest, parseQuery)
{
    const auto & parser = std::get<0>(GetParam());
    const auto & [input_text, expected_ast] = std::get<1>(GetParam());

    ASSERT_NE(nullptr, parser);

    if (expected_ast)
    {
        ASTPtr ast;
        ASSERT_NO_THROW(ast = parseQuery(*parser, input_text.begin(), input_text.end(), 0, 0));
        EXPECT_EQ(expected_ast, serializeAST(*ast->clone(), false));
    }
    else
    {
        ASSERT_THROW(parseQuery(*parser, input_text.begin(), input_text.end(), 0, 0), DB::Exception);
    }
}

struct TableOverrideTestCase
{
    DB::String create_database_query;
    DB::String create_table_query;
    DB::String expected_create_table_query;
};

std::ostream & operator<<(std::ostream & ostr, const TableOverrideTestCase & test_case)
{
    return ostr << "database: " << test_case.create_database_query << ", table: " << test_case.create_table_query
                << ", expected: " << test_case.expected_create_table_query;
}

class TableOverrideTest : public ::testing::TestWithParam<TableOverrideTestCase>
{};

TEST_P(TableOverrideTest, applyOverrides)
{
    const auto & [database_query, table_query, expected_query] = GetParam();
    ParserCreateQuery parser;
    ASTPtr database_ast;
    ASSERT_NO_THROW(database_ast = parseQuery(parser, database_query, 0, 0));
    auto * database = database_ast->as<ASTCreateQuery>();
    ASSERT_NE(nullptr, database);
    ASTPtr table_ast;
    ASSERT_NO_THROW(table_ast = parseQuery(parser, table_query, 0, 0));
    auto * table = table_ast->as<ASTCreateQuery>();
    ASSERT_NE(nullptr, table);
    auto table_name = table->table->as<ASTIdentifier>()->name();
    if (database->table_overrides)
    {
        auto override_ast = database->table_overrides->tryGetTableOverride(table_name);
        ASSERT_NE(nullptr, override_ast);
        auto * override_table_ast = override_ast->as<ASTTableOverride>();
        ASSERT_NE(nullptr, override_table_ast);
        override_table_ast->applyToCreateTableQuery(table);
    }
    EXPECT_EQ(expected_query, serializeAST(*table));
}

INSTANTIATE_TEST_SUITE_P(ParserOptimizeQuery, ParserTest,
    ::testing::Combine(
        ::testing::Values(std::make_shared<ParserOptimizeQuery>()),
        ::testing::ValuesIn(std::initializer_list<ParserTestCase>
        {
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY COLUMNS('a, b')",
                "OPTIMIZE TABLE table_name DEDUPLICATE BY COLUMNS('a, b')"
            },
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY COLUMNS('[a]')",
                "OPTIMIZE TABLE table_name DEDUPLICATE BY COLUMNS('[a]')"
            },
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY COLUMNS('[a]') EXCEPT b",
                "OPTIMIZE TABLE table_name DEDUPLICATE BY COLUMNS('[a]') EXCEPT b"
            },
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY COLUMNS('[a]') EXCEPT (a, b)",
                "OPTIMIZE TABLE table_name DEDUPLICATE BY COLUMNS('[a]') EXCEPT (a, b)"
            },
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY a, b, c",
                "OPTIMIZE TABLE table_name DEDUPLICATE BY a, b, c"
            },
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY *",
                "OPTIMIZE TABLE table_name DEDUPLICATE BY *"
            },
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY * EXCEPT a",
                "OPTIMIZE TABLE table_name DEDUPLICATE BY * EXCEPT a"
            },
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY * EXCEPT (a, b)",
                "OPTIMIZE TABLE table_name DEDUPLICATE BY * EXCEPT (a, b)"
            }
        }
)));

INSTANTIATE_TEST_SUITE_P(ParserOptimizeQuery_FAIL, ParserTest,
    ::testing::Combine(
        ::testing::Values(std::make_shared<ParserAlterCommand>()),
        ::testing::ValuesIn(std::initializer_list<ParserTestCase>
        {
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY",
            },
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY COLUMNS('[a]') APPLY(x)",
            },
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY COLUMNS('[a]') REPLACE(y)",
            },
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY * APPLY(x)",
            },
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY * REPLACE(y)",
            },
            {
                "OPTIMIZE TABLE table_name DEDUPLICATE BY db.a, db.b, db.c",
            }
        }
)));


INSTANTIATE_TEST_SUITE_P(ParserAlterCommand_MODIFY_COMMENT, ParserTest,
    ::testing::Combine(
        ::testing::Values(std::make_shared<ParserAlterCommand>()),
        ::testing::ValuesIn(std::initializer_list<ParserTestCase>
        {
            {
                // Empty comment value
                "MODIFY COMMENT ''",
                "MODIFY COMMENT ''",
            },
            {
                // Non-empty comment value
                "MODIFY COMMENT 'some comment value'",
                "MODIFY COMMENT 'some comment value'",
            }
        }
)));


INSTANTIATE_TEST_SUITE_P(ParserCreateQuery_DICTIONARY_WITH_COMMENT, ParserTest,
    ::testing::Combine(
        ::testing::Values(std::make_shared<ParserCreateQuery>()),
        ::testing::ValuesIn(std::initializer_list<ParserTestCase>{
        {
            R"sql(CREATE DICTIONARY 2024_dictionary_with_comment
(
    id UInt64,
    value String
)
PRIMARY KEY id
SOURCE(CLICKHOUSE(HOST 'localhost' PORT tcpPort() TABLE 'source_table'))
LAYOUT(FLAT())
LIFETIME(MIN 0 MAX 1000)
COMMENT 'Test dictionary with comment';
sql",
        R"sql(CREATE DICTIONARY `2024_dictionary_with_comment`
(
    `id` UInt64,
    `value` String
)
PRIMARY KEY id
SOURCE(CLICKHOUSE(HOST 'localhost' PORT tcpPort() TABLE 'source_table'))
LIFETIME(MIN 0 MAX 1000)
LAYOUT(FLAT())
COMMENT 'Test dictionary with comment')sql"
    }}
)));

INSTANTIATE_TEST_SUITE_P(ParserCreateDatabaseQuery, ParserTest,
    ::testing::Combine(
        ::testing::Values(std::make_shared<ParserCreateQuery>()),
        ::testing::ValuesIn(std::initializer_list<ParserTestCase>{
        {
            "CREATE DATABASE db ENGINE=MaterializeMySQL('addr:port', 'db', 'user', 'pw')",
            "CREATE DATABASE db\nENGINE = MaterializeMySQL('addr:port', 'db', 'user', 'pw')"
        },
        {
            "CREATE DATABASE db ENGINE=MaterializeMySQL('addr:port', 'db', 'user', 'pw') TABLE OVERRIDE `tbl`\n(PARTITION BY toYYYYMM(created))",
            "CREATE DATABASE db\nENGINE = MaterializeMySQL('addr:port', 'db', 'user', 'pw')\nTABLE OVERRIDE `tbl`\n(\n    PARTITION BY toYYYYMM(`created`)\n)"
        },
        {
            "CREATE DATABASE db ENGINE=Foo TABLE OVERRIDE `tbl` (), TABLE OVERRIDE a (COLUMNS (_created DateTime MATERIALIZED now())), TABLE OVERRIDE b (PARTITION BY rand())",
            "CREATE DATABASE db\nENGINE = Foo\nTABLE OVERRIDE `tbl`,\nTABLE OVERRIDE `a`\n(\n    COLUMNS\n    (\n        `_created` DateTime MATERIALIZED now()\n    )\n),\nTABLE OVERRIDE `b`\n(\n    PARTITION BY rand()\n)"
        },
        {
            "CREATE DATABASE db ENGINE=MaterializeMySQL('addr:port', 'db', 'user', 'pw') TABLE OVERRIDE tbl (COLUMNS (id UUID) PARTITION BY toYYYYMM(created))",
            "CREATE DATABASE db\nENGINE = MaterializeMySQL('addr:port', 'db', 'user', 'pw')\nTABLE OVERRIDE `tbl`\n(\n    COLUMNS\n    (\n        `id` UUID\n    )\n    PARTITION BY toYYYYMM(`created`)\n)"
        },
        {
            "CREATE DATABASE db TABLE OVERRIDE tbl (COLUMNS (INDEX foo foo TYPE minmax GRANULARITY 1) PARTITION BY if(_staged = 1, 'staging', toYYYYMM(created)))",
            "CREATE DATABASE db\nTABLE OVERRIDE `tbl`\n(\n    COLUMNS\n    (\n        INDEX foo `foo` TYPE minmax GRANULARITY 1\n    )\n    PARTITION BY if(`_staged` = 1, 'staging', toYYYYMM(`created`))\n)"
        },
        {
            "CREATE DATABASE db TABLE OVERRIDE t1 (TTL inserted + INTERVAL 1 MONTH DELETE), TABLE OVERRIDE t2 (TTL `inserted` + INTERVAL 2 MONTH DELETE)",
            "CREATE DATABASE db\nTABLE OVERRIDE `t1`\n(\n    TTL `inserted` + toIntervalMonth(1)\n),\nTABLE OVERRIDE `t2`\n(\n    TTL `inserted` + toIntervalMonth(2)\n)"
        },
        {
            "CREATE DATABASE db ENGINE = MaterializeMySQL('127.0.0.1:3306', 'db', 'root', 'pw') SETTINGS allows_query_when_mysql_lost = 1 TABLE OVERRIDE tab3 (COLUMNS (_staged UInt8 MATERIALIZED 1) PARTITION BY (c3) TTL c3 + INTERVAL 10 minute), TABLE OVERRIDE tab5 (PARTITION BY (c3) TTL c3 + INTERVAL 10 minute)",
            "CREATE DATABASE db\nENGINE = MaterializeMySQL('127.0.0.1:3306', 'db', 'root', 'pw')\nSETTINGS allows_query_when_mysql_lost = 1\nTABLE OVERRIDE `tab3`\n(\n    COLUMNS\n    (\n        `_staged` UInt8 MATERIALIZED 1\n    )\n    PARTITION BY `c3`\n    TTL `c3` + toIntervalMinute(10)\n),\nTABLE OVERRIDE `tab5`\n(\n    PARTITION BY `c3`\n    TTL `c3` + toIntervalMinute(10)\n)"
        },
        {
            "CREATE DATABASE db TABLE OVERRIDE tbl (PARTITION BY toYYYYMM(created) COLUMNS (created DateTime CODEC(Delta)))",
            "CREATE DATABASE db\nTABLE OVERRIDE `tbl`\n(\n    COLUMNS\n    (\n        `created` DateTime CODEC(Delta)\n    )\n    PARTITION BY toYYYYMM(`created`)\n)"
        },
        {
            "CREATE DATABASE db ENGINE = Foo() SETTINGS a = 1",
            "CREATE DATABASE db\nENGINE = Foo\nSETTINGS a = 1"
        },
        {
            "CREATE DATABASE db ENGINE = Foo() SETTINGS a = 1, b = 2",
            "CREATE DATABASE db\nENGINE = Foo\nSETTINGS a = 1, b = 2"
        },
        {
            "CREATE DATABASE db ENGINE = Foo() SETTINGS a = 1, b = 2 TABLE OVERRIDE a (ORDER BY (id, version))",
            "CREATE DATABASE db\nENGINE = Foo\nSETTINGS a = 1, b = 2\nTABLE OVERRIDE `a`\n(\n    ORDER BY (`id`, `version`)\n)"
        },
        {
            "CREATE DATABASE db ENGINE = Foo() SETTINGS a = 1, b = 2 COMMENT 'db comment' TABLE OVERRIDE a (ORDER BY (id, version))",
            "CREATE DATABASE db\nENGINE = Foo\nSETTINGS a = 1, b = 2\nTABLE OVERRIDE `a`\n(\n    ORDER BY (`id`, `version`)\n)\nCOMMENT 'db comment'"
        }
})));

INSTANTIATE_TEST_SUITE_P(ApplyTableOverrides, TableOverrideTest,
    ::testing::ValuesIn(std::initializer_list<TableOverrideTestCase>{
    {
        "CREATE DATABASE db",
        "CREATE TABLE db.t (id Int64) ENGINE=Log",
        "CREATE TABLE db.t (`id` Int64) ENGINE = Log"
    },
    {
        "CREATE DATABASE db TABLE OVERRIDE t (PARTITION BY tuple())",
        "CREATE TABLE db.t (id Int64) ENGINE=MergeTree",
        "CREATE TABLE db.t (`id` Int64) ENGINE = MergeTree PARTITION BY tuple()"
    },
    {
        "CREATE DATABASE db TABLE OVERRIDE t (COLUMNS (id UInt64, shard UInt8 MATERIALIZED modulo(id, 16)) PARTITION BY shard)",
        "CREATE TABLE db.t (id Int64) ENGINE=MergeTree",
        "CREATE TABLE db.t (`id` UInt64, `shard` UInt8 MATERIALIZED id % 16) ENGINE = MergeTree PARTITION BY shard"
    },
    {
        "CREATE DATABASE db TABLE OVERRIDE a (PARTITION BY modulo(id, 3)), TABLE OVERRIDE b (PARTITION BY modulo(id, 5))",
        "CREATE TABLE db.a (id Int64) ENGINE=MergeTree",
        "CREATE TABLE db.a (`id` Int64) ENGINE = MergeTree PARTITION BY id % 3"
    },
    {
        "CREATE DATABASE db TABLE OVERRIDE a (PARTITION BY modulo(id, 3)), TABLE OVERRIDE b (PARTITION BY modulo(id, 5))",
        "CREATE TABLE db.b (id Int64) ENGINE=MergeTree",
        "CREATE TABLE db.b (`id` Int64) ENGINE = MergeTree PARTITION BY id % 5"
    },
    {
        "CREATE DATABASE db ENGINE=MaterializeMySQL('addr:port', 'db', 'user', 'pw') TABLE OVERRIDE `tbl` (PARTITION BY toYYYYMM(created))",
        "CREATE TABLE db.tbl (id Int64, created DateTime) ENGINE=Foo",
        "CREATE TABLE db.tbl (`id` Int64, `created` DateTime) ENGINE = Foo PARTITION BY toYYYYMM(created)",
    }
}));
