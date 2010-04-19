#include "hektor-pep.h"

int pep_extract_value(const char *value_name, const char *pep_page) {
  // The total length of a pep column, including the name, eeparating
  // whitespace, and the value. For example:
  //
  // tx_bytes              2147904
  // rx_bytes             130982283
  // ^----------------------------^ is 30 characters
  enum { COLUMN_LENGTH = 30 };

  const char *value_name_match = strstr(pep_page, value_name);
  if (!value_name_match) return 0;

  const size_t value_name_begin = value_name_match - pep_page;
  const size_t value_name_end = value_name_begin + strlen(value_name);

  const size_t value_name_length = value_name_end - value_name_begin;
  const size_t remaining_length = COLUMN_LENGTH - value_name_length;

  char buffer[remaining_length];
  memcpy(buffer, &pep_page[value_name_end], remaining_length);

  return atoi(buffer);
}
