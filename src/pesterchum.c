// Pesterchum markup defined (loosely) here:
// https://github.com/illuminatedwax/pesterchum/blob/master/README.mkdn

#define PURPLE_PLUGINS

// DEBUG
#include <stdio.h>

#include <ctype.h>  // isdigit
#include <stdint.h>  // size_t
#include <string.h>  // strlen, strstr

#define GLIB_VERSION_MIN_REQUIRED (GLIB_VERSION_2_32)
#include <glib.h>  // g_*

// libpurple headers
#include <libpurple/account.h>  // PurpleAccount
#include <libpurple/conversation.h>  // PurpleConversation, PurpleMessageFlags, purple_conversations_get_handle
#include <libpurple/plugin.h>  // PurplePlugin, PurplePluginInfo, PURPLE_INIT_PLUGIN, PURPLE_PLUGIN_STANDARD, PURPLE_PRIORITY_DEFAULT
#include <libpurple/signals.h>  // PURPLE_CALLBACK, purple_signal_connect
#include <libpurple/version.h>  // PURPLE_*_VERSION

#include "./pesterchum.h"


#define P_NAME     (char*) "Purple Pesterchum"
#define P_SUMMARY  (char*) "Pesterchum plugin for libpurple/Pidgin"
#define P_DESC     (char*) "Renders Pesterchum effects in Pidgin."
#define P_ID       (char*) "garlic-purple_pesterchum"
#define P_AUTHOR   (char*) "garlicOS® <sisdfk@gmail.com>"
#define P_VERSION  (char*) "0.0.1"
#define P_WEBSITE  (char*) "https://github.com/garlic_os/purple-pesterchum"


/**
 * Converts message format from Pesterchum to Pidgin
 * Pidgin font format: <FONT COLOR="#ffffff">hi</FONT>
 * Pesterchum font format:
 *   <c=0,255,0>hi</c>
 *   <c=#7f7f7f>hi</c>
 *   <c=red>hi</c>
 *   ending </c> not required
 *
 * @param message  (IN/OUT) The message to convert
 * @pre   message is a null-terminated UTF-8 string
 * @pre   message is allocated with malloc()
 * @post  message should be freed with free()
 */
void convert_message(char **message) {
	g_return_if_fail(message   != NULL);
	g_return_if_fail(*message  != NULL);
	g_return_if_fail(**message != '\0');  //  Ensure string is null-terminated
	g_return_if_fail(g_utf8_validate(*message, -1, NULL));

	GString *new_msg = g_string_sized_new(strlen(*message));

	printf("[DEBUG] original:  %s\n", *message);

	// Scan the message for Pesterchum color markup and add corresponding
	// Pidgin color markup to the new message
	size_t message_size = strlen(*message);
	char *cursor = *message;
	while (cursor < *message + message_size) {
		char *tag_start = strstr(cursor, "&lt;c=");
		if (tag_start == NULL) break;  // No more color tags in the message
		char *color_start = tag_start + 6;
		char *color_end = strstr(color_start, "&gt;");
		if (color_end == NULL) break;  // Malformed tag; just give up
		size_t color_size = color_end - color_start;
		bool is_rgb = isdigit(color_start[0]);
		char *preceding_text = cursor;
		size_t preceding_text_size = tag_start - cursor;
		g_string_append_len(new_msg, preceding_text, preceding_text_size);

		bool is_hex = color_start[0] == '#';
		g_string_append(new_msg, "<FONT COLOR=\"");
		if (is_hex) {
			g_string_append_len(new_msg, color_start, color_size);
		} else {
			g_string_append(new_msg, "rgb(");
			g_string_append_len(new_msg, color_start, color_size);
			g_string_append(new_msg, ")");
		}
		g_string_append(new_msg, "\">");
	
		char *content_start = color_end + 4;
		char *content_end = strstr(content_start, "&lt;/c&gt;");
		if (content_end == NULL) {
			// No closing tag; just use the rest of the message
			content_end = *message + message_size;
		}
		size_t content_size = content_end - content_start;
		g_string_append_len(new_msg, content_start, content_size);
		g_string_append(new_msg, "</FONT>");
		cursor = content_end + 10;
	}
	g_string_append(new_msg, cursor);  // Add any text after the last tag

	// Return result
	g_free(*message);
	*message = g_string_free(new_msg, FALSE);
	printf("[DEBUG] converted: %s\n", *message);
}


// Convert incoming text by intercepting the receiving-im signal
static gboolean receiving_im_msg(
	PurpleAccount *account,
	char **sender,
	char **message,
	PurpleConversation *conv,
	PurpleMessageFlags *flags
) {
	(void) account;
	(void) sender;
	(void) conv;
	(void) flags;
	convert_message(message);
	return FALSE;  // "message should not be canceled"
}


// Register the receiving-im signal
static gboolean plugin_load(PurplePlugin *plugin) {
	colornames_init();
	purple_signal_connect(
		purple_conversations_get_handle(),
		"receiving-im-msg",
		plugin,
		PURPLE_CALLBACK(receiving_im_msg),
		NULL
	);
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
	// .type              = PURPLE_PLUGIN_PROTOCOL,
	.ui_requirement    = NULL,
	.flags             = 0,
	.dependencies      = NULL,
	.priority          = PURPLE_PRIORITY_DEFAULT,
	.id                = P_ID,
	.name              = P_NAME,
	.version           = P_VERSION,
	.summary           = P_SUMMARY,
	.description       = P_DESC,
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
}


#ifdef __cplusplus
	extern "C" {
#endif
	PURPLE_INIT_PLUGIN(purple_pesterchum, init_plugin, info);
#ifdef __cplusplus
	}
#endif
