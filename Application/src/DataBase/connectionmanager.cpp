#include "DataBase/connectionmanager.h"
#include "DataBase/dbtypes.h"
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

struct ConnectionManager::ConnectionManagerPrivate
{
    struct SqliteDeleter
    {
        void operator()(QSqlDatabase* db) {
            db->close();
            QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
            delete db;
        }
    };

    std::unique_ptr<QSqlDatabase, SqliteDeleter> db;
    QString dbPath;
    bool isValid {true};
    DBState state {DBState::OK};

    bool setup();
    bool setupWorkspace();
    bool setupTables();
};

ConnectionManager::~ConnectionManager()
{}

ConnectionManager &ConnectionManager::instance()
{
    static ConnectionManager instance;
    return instance;
}

bool ConnectionManager::isValide()
{
    return _m->isValid;
}

ConnectionManager::ConnectionManager()
    : _m{std::make_unique<ConnectionManagerPrivate>()}
{
    _m->isValid = _m->setup();
}

bool ConnectionManager::ConnectionManagerPrivate::setup()
{
    const QString driver {"QSQLITE"};

    if (!QSqlDatabase::isDriverAvailable(driver)) {
        state = DBState::ERROR_NO_DRIVER;
        qWarning() << "Driver " << driver << " is not available.";
        return false;
    }

    if (!setupWorkspace())
    {
        state = DBState::ERROR_WORKSPACE;
        qCritical() << "Unable to setup workspace";
        return false;
    }

    db.reset(new QSqlDatabase(QSqlDatabase::addDatabase(driver)));
    db->setDatabaseName(dbPath);

    if (!db->open())
    {
        state = DBState::ERROR_OPENING;
        qCritical() << "Unable to open database: " << db->lastError().text();
        return false;
    }

    return setupTables();
}

bool ConnectionManager::ConnectionManagerPrivate::setupWorkspace()
{
    static const QString databaseName {"Maps"};
    const QString location(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    dbPath = location + QString("/") + databaseName;

    QDir dbDirectory {location};
    if (!dbDirectory.exists())
    {
        const bool creationResult {dbDirectory.mkpath(location)};
        if (creationResult) {
            qInfo() << "DB directory created";
        } else {
            qCritical() << "Unable to create DB directory";
        }
    }

    qDebug() << "Data path is: " + dbPath;

    return dbDirectory.exists();
}

bool ConnectionManager::ConnectionManagerPrivate::setupTables()
{
    bool result {true};

    std::vector<QString> tablesNames {
        QString {
            "MapFiles"
        },
        QString {
            "Maps"
        }
    };

    std::vector<QSqlQuery> creationQueries {
        QSqlQuery {
            "CREATE TABLE IF NOT  EXISTS MapFiles"
            "("
            "map_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "map_name TEXT NOT NULL DEFAULT '',"
            "author_tag TEXT NOT NULL DEFAULT '',"
            "map_picture TEXT NOT NULL DEFAULT '',"
            "map_cihanged TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
            ")"
        },
        QSqlQuery {
            "CREATE TABLE IF NOT  EXISTS Maps"
            "("
            "map_id INT PRIMARY KEY NOT NULL,"
            "map TEXT NOT NULL"
            ")"
        }
    };

    for (size_t i = 0; i < tablesNames.size(); ++i)
    {
        if (!db->tables().contains(tablesNames[i]))
        {
            if (!creationQueries[i].exec())
            {
                result = false;
                state = DBState::ERROR_TABLES;
                qWarning() << creationQueries[i].lastError().text();
            }
        }
    }

    return result;
}
