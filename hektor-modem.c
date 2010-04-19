#include "hektor-modem.h"

// Append a path to the modem's base url.
static inline void modem_build_url(const char *url_path, char *url_buffer) {
  strncat(url_buffer, "http://192.168.0.1", MAX_PATH_LENGTH);
  strncat(url_buffer, url_path, MAX_PATH_LENGTH - strlen(url_buffer));
}

// Request a url with a callback function.
static CURLcode modem_fetch_url(const char *url, void *receive_fn, void *fn_data) {
  char full_url[MAX_PATH_LENGTH] = {0};
  modem_build_url(url, full_url);

  CURL *curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_URL, full_url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_fn);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fn_data);

  CURLcode result = curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  return result;
}

bool modem_find_url(const char *page_title, const char *menu_page,
                    char *url_buffer) {
  // The little bit of text between a page title and its url
  static const char *TITLE_URL_SEP = "\", \"";
  // The little bit of text at the end of a url
  static const char *URL_ENDING = "\"))";

  // Try to match the page title.
  const char *title_match = strstr(menu_page, page_title);
  if (!title_match) return false;

  const size_t title_begin = title_match - menu_page;
  const size_t url_begin = title_begin
                         + strlen(page_title)
                         + strlen(TITLE_URL_SEP);

  const char *url_end_match = strstr(&menu_page[url_begin], URL_ENDING);
  if (!url_end_match) return false;

  const size_t url_end = url_end_match - menu_page;
  const size_t url_length = url_end - url_begin;

  memcpy(url_buffer, &menu_page[url_begin], min(url_length, MAX_PATH_LENGTH));

  return true;
}

// Used to keep state between chunks.
typedef struct {
  char *buffer;
  size_t amount_written;
} buffer_append_state_t;

// Append successive chunks of a page to a buffer.
size_t buffer_append_fn(const char *chunk, const size_t item_size,
                        const size_t items, buffer_append_state_t *state)
{
  const size_t chunk_size = item_size * items;
  const size_t buffer_remaining = MAX_PAGE_SIZE - state->amount_written;

  if (buffer_remaining <= 0) return 0;

  memcpy(&state->buffer[state->amount_written], chunk, min(chunk_size,
                                                           buffer_remaining));
  state->amount_written += chunk_size;

  return chunk_size;
}

size_t modem_fetch_page(const char *url, char *buffer) {
  buffer_append_state_t state = {buffer};
  modem_fetch_url(url, buffer_append_fn, &state);

  return state.amount_written;
}
