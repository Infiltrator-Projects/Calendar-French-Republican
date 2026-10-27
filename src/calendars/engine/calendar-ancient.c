// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 1993-2026 Shannon Smith

/*
 * Ancient-calendar engines.
 *
 * Mayan conversion uses the Goodman-Martínez-Thompson correlation constant
 * (JDN 584283), the conventional correlation adopted by the project's
 * reference literature (including Reingold/Dershowitz, Calendrical
 * Calculations). Alternative historical correlations exist, so Mayan absolute
 * dates are explicitly model-dependent rather than presented as uniquely
 * established chronology. The Long Count is treated as arithmetic over kin.
 * The native
 * representation stores tun / uinal / kin because those are the fields the UI
 * browses; conversion remains reversible even for dates before the epoch by
 * using floor division and a positive remainder.
 *
 * Roman dates are a formatting system over the proleptic Julian calendar, not
 * a second absolute chronology.  Roman day names count inclusively backwards
 * to Kalends, Nones or Ides, which is why the apparent distance includes both
 * the current day and the target marker.
 */

#include "calendar-ancient.h"

#include "julian-day.h"

#include <glib/gi18n-lib.h>

#define floor_divide calendar_floor_divide
#define positive_modulo calendar_positive_modulo

#define MAYAN_EPOCH_JDN ((gint64)584283)

enum
{
    MAYAN_KIN_PER_UINAL = 20,
    MAYAN_KIN_PER_TUN = 360,
    MAYAN_KIN_PER_HAAB = 365
};


gchar *
calendar_roman_number(gint64 value)
{
    static const struct
    {
        gint value;
        const gchar *digits;
    } numerals[] = {
        { 1000, "M" },
        { 900, "CM" },
        { 500, "D" },
        { 400, "CD" },
        { 100, "C" },
        { 90, "XC" },
        { 50, "L" },
        { 40, "XL" },
        { 10, "X" },
        { 9, "IX" },
        { 5, "V" },
        { 4, "IV" },
        { 1, "I" }
    };
    g_autoptr(GString) result = g_string_new(NULL);
    gsize index;

    /*
     * Conventional unbarred Roman notation is bounded. Avoid multi-megabyte
     * strings and millions of subtraction iterations for the library's much
     * wider supported civil-year domain.
     */
    if (value <= 0 || value > 3999)
        return g_strdup_printf("%" G_GINT64_FORMAT, value);

    for (index = 0; index < G_N_ELEMENTS(numerals); index++)
    {
        while (value >= numerals[index].value)
        {
            g_string_append(result, numerals[index].digits);
            value -= numerals[index].value;
        }
    }

    {
        GString *owned_result = result;

        result = NULL;
        return g_string_free(owned_result, FALSE);
    }
}


