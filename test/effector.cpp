#include <rapidcheck.h>

#include <flom/effector.hpp>

int main() {
  rc::check("Effector test", [](const flom::Effector &) { return true; });
}
