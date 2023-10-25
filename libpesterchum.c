#define PURPLE_PLUGINS

#define GLIB_VERSION_MIN_REQUIRED (GLIB_VERSION_2_76)
#include <glib.h>

#ifdef _WIN32
	#include "./libpurple-mini/win32/win32dep.h"
#endif

#include "./libpurple-mini/version.h"
#include "./libpurple-mini/plugin.h"
#include "./libpurple-mini/util.h"
#include "./libpurple-mini/internal.h"
#include "./libpurple-mini/debug.h"


#define P_NAME     "Purple Pesterchum"
#define P_SUMMARY  "Pesterchum plugin for libpurple/Pidgin"
#define P_DESC     "Renders Pesterchum effects in Pidgin."
#define P_ID       "garlic-pesterchum_pidgin_plugin"
#define P_AUTHOR   "garlicOS® <sisdfk@gmail.com>"
#define P_VERSION  "0.0.1"
#define P_WEBSITE  "https://github.com/garlic_os/purple-pesterchum"

// TO DO: delete
#define PREFS_I_RED    0
#define PREFS_I_GREEN  0
#define PREFS_I_BLUE   0
#define PREFS_T_RED    0
#define PREFS_T_GREEN  0
#define PREFS_T_BLUE   0


inline static guint8 round_gfloat_to_guint8(gfloat f) {
	return ((guchar)(f + 0.5f));
}


static void colorize_message(char **message) {
	g_return_if_fail(message   != NULL);
	g_return_if_fail(*message  != NULL);
	g_return_if_fail(**message != '\0');
	g_return_if_fail(g_utf8_validate(*message, -1, NULL));

	// A decent starting size
	GString* new_msg = g_string_sized_new(strlen(*message));

	// Get colors from preferences
	guint8 initial_rgb[3] = {
		(guint8)purple_prefs_get_int(PREFS_I_RED),
		(guint8)purple_prefs_get_int(PREFS_I_GREEN),
		(guint8)purple_prefs_get_int(PREFS_I_BLUE)
	};
	guint8 target_rgb[3] = {
		(guint8)purple_prefs_get_int(PREFS_T_RED),
		(guint8)purple_prefs_get_int(PREFS_T_GREEN),
		(guint8)purple_prefs_get_int(PREFS_T_BLUE)
	};

	// Initialize current gradient value
	gfloat grad[3] = {
		(gfloat)initial_rgb[0],
		(gfloat)initial_rgb[1],
		(gfloat)initial_rgb[2]
	};

	// Determine the delta gradient value
	gfloat d_grad[3];
	glong len = g_utf8_strlen( *message, -1);
	d_grad[0] = (gfloat)(target_rgb[0] - initial_rgb[0]) / (gfloat)len;
	d_grad[1] = (gfloat)(target_rgb[1] - initial_rgb[1]) / (gfloat)len;
	d_grad[2] = (gfloat)(target_rgb[2] - initial_rgb[2]) / (gfloat)len;

	// Open initial font tag and format first character
	gunichar ch = g_utf8_get_char(*message);
	gchar* c = g_utf8_next_char(*message);
	g_string_append_printf(new_msg, "<font color=\"#%02x%02x%02x\">",
	                       round_gfloat_to_guint8(grad[0]),
	                       round_gfloat_to_guint8(grad[1]),
	                       round_gfloat_to_guint8(grad[2]));
	g_string_append_unichar(new_msg, ch);

	// Format each character one by one:
	//   (if it is printable and not a space -- this excludes LTR, control, and
	//    format characters) AND
	//   (if it is not the same color as the last character)
	for ( ; c && *c; c = g_utf8_next_char(c)) {
		gunichar ch = g_utf8_get_char(c);

		// Store last color
		guint8 last_rgb[3] = {
			round_gfloat_to_guint8(grad[0]),
			round_gfloat_to_guint8(grad[1]),
			round_gfloat_to_guint8(grad[2])
		};

		// Increment the gradient
		grad[0] += d_grad[0];
		grad[1] += d_grad[1];
		grad[2] += d_grad[2];

		// Format next character appropriately
		if (g_unichar_isgraph(ch) && !rgb_equals(last_rgb, grad)) {
			g_string_append_printf(new_msg, "</font><font color=\"#%02x%02x%02x\">",
			                                round_gfloat_to_guint8(grad[0]),
			                                round_gfloat_to_guint8(grad[1]),
			                                round_gfloat_to_guint8(grad[2]));
		}
		g_string_append_unichar(new_msg, ch);
	}

	// Close final font tag
	g_string_append(new_msg, "</font>");

	// Return result
	g_free(*message);
	*message = g_string_free(new_msg, FALSE);
}


static gboolean plugin_load(/* PurplePlugin *plugin */) {
	return TRUE;
}


static gboolean plugin_unload(/* PurplePlugin *plugin */) {
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
	.magic             = PURPLE_P_MAGIC,
	.major_version     = PURPLE_MAJOR_VERSION,
	.minor_version     = PURPLE_MINOR_VERSION,
	.type              = PURPLE_P_STANDARD,
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


// Initialize default preferences
static void init_plugin(/* PurplePlugin *plugin */) {
	#ifdef ENABLE_NLS
		bindtextdomain(GETTEXT_PACKAGE, PP_LOCALEDIR);
		bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	#endif
	info.name = (char *) _(P_NAME);
	info.summary = (char *) _(P_SUMMARY);
	info.description = (char *) _(P_DESCRIPTION);
}


PURPLE_INIT_PLUGIN(purple_pesterchum, init_plugin, info)
