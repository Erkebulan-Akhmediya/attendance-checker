#include <stdexcept>

namespace User::Exceptions {

struct NoUserChanged : std::runtime_error {
    NoUserChanged();
};

}
