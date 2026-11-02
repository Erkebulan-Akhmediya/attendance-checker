#include "user_exceptions.h"
#include <stdexcept>

User::Exceptions::NoUserChanged::NoUserChanged()
    : std::runtime_error("no user changed")
{
}
