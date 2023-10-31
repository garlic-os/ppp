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


#define P_NAME     (char*) "Purple Pesterchum"
#define P_SUMMARY  (char*) "Pesterchum plugin for libpurple/Pidgin"
#define P_DESC     (char*) "Renders Pesterchum effects in Pidgin."
#define P_ID       (char*) "garlic-pesterchum_pidgin_plugin"
#define P_AUTHOR   (char*) "garlicOS® <sisdfk@gmail.com>"
#define P_VERSION  (char*) "0.0.1"
#define P_WEBSITE  (char*) "https://github.com/garlic_os/purple-pesterchum"


// https://stackoverflow.com/a/4832
static gssize index_of(const char *haystack, const char *needle, gssize start_index) {
	g_return_val_if_fail(haystack != NULL, -1);
	g_return_val_if_fail(needle   != NULL, -1);
	g_return_val_if_fail(start_index < (gssize) strlen(haystack), -1);

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
	g_return_if_fail(**message != '\0');  //  Ensure string is null-terminated
	g_return_if_fail(g_utf8_validate(*message, -1, NULL));

	GString *new_msg = g_string_sized_new(strlen(*message));

	// Scan the message for Pesterchum color codes and add respective
	// Pidgin color codes to the new message
	gssize message_size = strlen(*message);
	gssize index_close_end = 0;
	while (index_close_end < message_size) {
		gssize index_open_start = index_of(*message, "<c=", index_close_end);
		if (index_open_start == -1) break;  // No more color tags in the message
		gssize index_open_end = index_of(*message, ">", index_open_start + 3);
		if (index_open_start == -1) break;  // Malformed tag; just give up
		const char * const color_code = *message + index_open_start + 3;
		size_t code_size = index_open_end - index_open_start;
		g_string_append(new_msg, "<FONT COLOR=\"");
		g_string_append_len(new_msg, color_code, code_size);
		g_string_append(new_msg, "\">");
	
		gssize index_close_start = index_of(*message, "</c>", index_open_end + 1);
		if (index_close_start == -1) index_close_start = message_size;
		const char * const text_content = *message + 3 + index_open_end + 1;
		size_t content_size = index_close_start - index_open_end + 4;
		g_string_append_len(new_msg, text_content, content_size);
		g_string_append(new_msg, "</FONT>");
		index_close_end = index_close_start + 4;
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
