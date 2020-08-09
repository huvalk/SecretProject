#pragma once
#include <memory>

namespace DB
{
class ConnectionManager
{
public:
    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;
    ~ConnectionManager();

    static ConnectionManager& instance();
    bool isValide();

private:
    ConnectionManager();

    class ConnectionManagerPrivate;
    std::unique_ptr<ConnectionManagerPrivate> _m;
};
}
