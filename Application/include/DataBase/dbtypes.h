#pragma once

enum class DBResult {
    OK,
    FAIL
};

enum class DBTables {
    MapFiles
};

enum class DBState {
    OK,
    ERROR_NO_DRIVER,
    ERROR_WORKSPACE,
    ERROR_TABLES,
    ERROR_OPENING
};
