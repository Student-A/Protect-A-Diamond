#include "Wave.h"

void Wave::init(int must_types_count)
{
    _must_types.resize(must_types_count);
    _must_counts.resize(must_types_count);
}
