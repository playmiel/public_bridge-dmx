"""
Look-up table Gamma correction generator for Python 3.
"""
from math import pow


def gamma_lut(input_array_size, ouput_array_size=None,
              correction_factor=2.2, reverse_gamma=False):
    """
    Generate a Gamma correction LookUp Table from the given parameters.
    :param input_array_size: The input array size.
    :param ouput_array_size: The output array size (default to the input array size).
    :param correction_factor: The correction factor (between 0 and 3, included, default 2.2).
    :param reverse_gamma: Set to `True` for reverse Gamma correction (default `False`).
    :return: A generator expression.
    """
    assert input_array_size > 0, "Input array size must be positive"
    assert ouput_array_size is None or ouput_array_size > 0, "Output array must be positive"
    assert 0 < correction_factor <= 3, "Correction factor must be between 0 and 3 (included)" 

    # Fix output array size
    ouput_array_size = ouput_array_size or input_array_size

    # fix correction factor
    if reverse_gamma:
        correction_factor = 1.0 / correction_factor

    # Gamma correction formula
    for index in range(input_array_size):
        yield round(pow(index / float(input_array_size - 1), correction_factor) * (ouput_array_size - 1))


def split_chunks(array, chunk_size):
    """ Split the given array into chunks of at most the given size. """
    return [array[x:x + chunk_size] for x in range(0, len(array), chunk_size)]


# Output array size - MUST be a power of two
INPUT_ARRAY_SIZE = 256
OUTPUT_ARRAY_SIZE = 256

# Gamma correction coefficient - standard: 2.2, Maxim standard: 2.5
GAMMA_CORRECTION_COEFF = 2.2

# Number of values per line
NUMBER_COUNT_PER_LINE = 16

# Output header
print("""/* ----- BEGIN OF AUTO-GENERATED CODE - DO NOT EDIT ----- */
#ifndef GAMMA_H_
#define GAMMA_H_

/* Dependency for PROGMEM */
#include <avr/pgmspace.h>

/** Gamma correction table in flash memory. */
static const uint8_t PROGMEM _igamma01[] = {""")

# Print LUT data
gamma_table = gamma_lut(INPUT_ARRAY_SIZE, OUTPUT_ARRAY_SIZE, GAMMA_CORRECTION_COEFF)
gamma_table = [format(x, "3d") for x in gamma_table]
gamma_table_lines = split_chunks(gamma_table, NUMBER_COUNT_PER_LINE)
gamma_table_lines = ['  ' + ', '.join(line) for line in gamma_table_lines]
print(',\n'.join(gamma_table_lines))

# Output footer
print("""};

/**
 * Apply gamma correction to the given sample.
 *
 * @param x The input 8 bits sample value.
 * @return The output 8 bits sample value with gamma correction.
 */
static inline uint8_t igamma01(uint8_t x) {
  return pgm_read_byte(&_çgamma01[x]);
}

#endif /* GAMMA_H_ */
/* ----- END OF AUTO-GENERATED CODE ----- */
""")
