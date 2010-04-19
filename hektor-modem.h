#ifndef HEKTOR_MODEM_H
#define HEKTOR_MODEM_H

#include <curl/curl.h>
#include <stdbool.h>
#include <string.h>

#include "hektor-path.h"
#include "hektor-common.h"

enum { MAX_PAGE_SIZE = 1024 * 64 };

// Dynamically find the url of a page by its page title.
bool modem_find_url(const char *page_title, const char *menu_page,
                    char *url_buffer);

// Get the pep page's url.
static inline bool modem_find_pep_url(const char *menu_page, char *url_buffer) {
  return modem_find_url("ALL BCB Stats", menu_page, url_buffer);
}

// Download a page into a buffer.
size_t modem_fetch_page(const char *url, char *buffer);

// Download the menu page.
static inline size_t modem_fetch_menu_page(char *buffer) {
  return modem_fetch_page("/fs/scripts/cat_menu.js", buffer);
}

#endif
