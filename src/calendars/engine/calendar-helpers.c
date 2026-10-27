// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 1993-2026 Shannon Smith

#include "calendar-helpers.h"
#include "integer-math.h"

#include <glib/gi18n-lib.h>

gint64
calendar_count_multiples_inclusive(gint64 first,
                                        gint64 last,
                                        gint64 divisor)
{
    gint64 first_quotient;
    gint64 first_remainder;
    gint64 last_quotient;
    gint64 first_multiple_index;
    gint64 distance;

    g_return_val_if_fail(divisor > 0, 0);
    if (first > last)
        return 0;

    g_return_val_if_fail(
        infiltratr_i64_floor_divmod(first, divisor,
                                    &first_quotient, &first_remainder), 0);
    g_return_val_if_fail(
        infiltratr_i64_floor_divmod(last, divisor,
                                    &last_quotient, NULL), 0);

    first_multiple_index = first_remainder == 0 ?
        first_quotient :
        calendar_i64_add_saturating(first_quotient, 1);
    distance = infiltratr_i64_subtract_saturating(
        last_quotient, first_multiple_index);
    return distance < 0 ? 0 :
        calendar_i64_add_saturating(distance, 1);
}


