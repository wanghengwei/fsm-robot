#include "../Connection.h"

class X51Connection : public Connection {
public:
    X51Connection(IEventLink* link, const ConnectionParameters& params) : Connection{link, params} {}

};
