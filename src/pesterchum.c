// Pesterchum markup defined (loosely) here:
// https://github.com/illuminatedwax/pesterchum/blob/master/README.mkdn

#define PURPLE_PLUGINS

#include <string.h>
#include <stdint.h>

#define GLIB_VERSION_MIN_REQUIRED (GLIB_VERSION_2_76)
#include <glib.h>

#ifdef _WIN32
	#include "../libpurple-mini/win32/win32dep.h"
#endif

#include "../lib/libpurple-mini/version.h"
#include "../lib/libpurple-mini/plugin.h"
#include "../lib/libpurple-mini/util.h"
#include "../lib/libpurple-mini/internal.h"
#include "../lib/libpurple-mini/debug.h"

#include "pesterchum.h"


#define P_NAME     "Purple Pesterchum"
#define P_SUMMARY  "Pesterchum plugin for libpurple/Pidgin"
#define P_DESC     "Renders Pesterchum effects in Pidgin."
#define P_ID       "garlic-pesterchum_pidgin_plugin"
#define P_AUTHOR   "garlicOS® <sisdfk@gmail.com>"
#define P_VERSION  "0.0.1"
#define P_WEBSITE  "https://github.com/garlic_os/purple-pesterchum"


// https://stackoverflow.com/a/4832
static int64_t index_of(const char *haystack, const char *needle, size_t start_index) {
	g_return_val_if_fail(haystack != NULL, -1);
	g_return_val_if_fail(needle   != NULL, -1);
	g_return_val_if_fail(start_index < strlen(haystack), -1);

	const char *source = haystack + start_index;
	const char *found = strstr(source, needle);
	if (found == NULL) {
		return -1;
	}
	return found - source;
}

// Converts message format from Pesterchum to Pidgin
// Pidgin font format: <FONT COLOR="#ffffff">hi</FONT>
// Pesterchum font format:
//   <c=0,255,0>hi</c>
//   <c=#7f7f7f>hi</c>
//   <c=red>hi</c>
//   ending </c> not required
void colorize_message(char **message) {
	g_return_if_fail(message   != NULL);
	g_return_if_fail(*message  != NULL);
	g_return_if_fail(**message != '\0');
	g_return_if_fail(g_utf8_validate(*message, -1, NULL));

	GString *new_msg = g_string_sized_new(strlen(*message));

	// Scan the message for Pesterchum color codes and add respective
	// Pidgin color codes to the new message
	int64_t index_close = 0;
	int64_t index_open = 0;
	int64_t index_start = 0;
	int64_t index_end = 0;
	while (index_open != -1) {
		index_open = index_of(*message, "<c=", index_close);
		if (index_open != -1) {
			index_close = index_of(*message, ">", index_open);
			if (index_close != -1) {
				index_start = index_close + 1;
				index_end = index_of(*message, "</c>", index_start);
				if (index_end != -1) {
					g_string_append_len(new_msg, *message + index_start, index_end - index_start);
					g_string_append(new_msg, "</FONT>");
					g_string_append_len(new_msg, *message + index_open + 3, index_close - index_open - 3);
					g_string_append(new_msg, "<FONT COLOR=\"#");
					g_string_append_len(new_msg, *message + index_open + 3, index_close - index_open - 3);
					g_string_append(new_msg, "\">");
					index_close += 4;
				} else {
					g_string_append_len(new_msg, *message + index_start, strlen(*message) - index_start);
					g_string_append(new_msg, "</FONT>");
					g_string_append_len(new_msg, *message + index_open + 3, index_close - index_open - 3);
					g_string_append(new_msg, "<FONT COLOR=\"#");
					g_string_append_len(new_msg, *message + index_open + 3, index_close - index_open - 3);
					g_string_append(new_msg, "\">");
					index_close = strlen(*message);
				}
			} else {
				g_string_append_len(new_msg, *message + index_open, strlen(*message) - index_open);
				index_close = strlen(*message);
			}
		} else {
			g_string_append_len(new_msg, *message + index_close, strlen(*message) - index_close);
		}
	}

	// Return result
	// g_free(*message);
	*message = g_string_free(new_msg, FALSE);
}


static gboolean plugin_load(PurplePlugin *plugin) {
	(void) plugin;
	return TRUE;
}


static gboolean plugin_unload(PurplePlugin *plugin) {
	(void) plugin;
	return TRUE;
}


// static PurplePluginUiInfo prefs_info = {
// 	NULL,
// 	0,
// 	NULL,
// 	NULL,
// 	NULL,
// 	NULL,
// 	NULL
// };

static PurplePluginInfo info = {
	.magic             = PURPLE_PLUGIN_MAGIC,
	.major_version     = PURPLE_MAJOR_VERSION,
	.minor_version     = PURPLE_MINOR_VERSION,
	.type              = PURPLE_PLUGIN_STANDARD,
	.ui_requirement    = NULL,
	.flags             = 0,
	.dependencies      = NULL,
	.priority          = PURPLE_PRIORITY_DEFAULT,
	.id                = P_ID,
	.name              = NULL,
	.version           = P_VERSION,
	.summary           = NULL,
	.description       = NULL,
	.author            = P_AUTHOR,
	.homepage          = P_WEBSITE,
	.load              = plugin_load,
	.unload            = plugin_unload,
	.destroy           = NULL,
	.ui_info           = NULL,
	.extra_info        = NULL,
	.prefs_info        = NULL, // &prefs_info,
	.actions           = NULL,
	._purple_reserved1 = NULL,
	._purple_reserved2 = NULL,
	._purple_reserved3 = NULL,
	._purple_reserved4 = NULL
};


static void init_plugin(PurplePlugin *plugin) {
	(void) plugin;
	#ifdef ENABLE_NLS
		bindtextdomain(GETTEXT_PACKAGE, PP_LOCALEDIR);
		bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	#endif
	info.name = (char *) _(P_NAME);
	info.summary = (char *) _(P_SUMMARY);
	info.description = (char *) _(P_DESC);
}


PURPLE_INIT_PLUGIN(purple_pesterchum, init_plugin, info)
