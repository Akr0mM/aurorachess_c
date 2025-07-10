#include "aurora.h"
#include "position.h"

#include <stdint.h>

void aurora_init(Aurora *aurora, char *fen) {
  // INIT ENGINE
  aurora_load_fen(aurora, fen);
}
