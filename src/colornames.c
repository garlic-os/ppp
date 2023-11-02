#include <stdint.h>
#include <string.h>
#include <glib.h>
#include "./colornames.h"

static GHashTable *colornames = NULL;

#define ADD_COLOR(name, code) \
	g_hash_table_insert(colornames, (char*) (name), (char*) (code));

char *colornames_get(const char *name, size_t len) {
	if (colornames == NULL) colornames_init();
	char *key = strndup(name, len);
	char *color_code = (char*) g_hash_table_lookup(colornames, key);
	free(key);
	return color_code;
}

void colornames_destroy() {
	if (colornames == NULL) return;
	g_hash_table_destroy(colornames);
	colornames = NULL;
}


// Pesterchum's color names
// Pasted from:
// https://github.com/Dpeta/pesterchum-alt-servers/blob/main/pnc/unicolor.py
void colornames_init() {
	if (colornames != NULL) return;
	colornames = g_hash_table_new(g_str_hash, g_str_equal);
	ADD_COLOR("aliceblue", "240,248,255");
	ADD_COLOR("antiquewhite", "250,235,215");
	ADD_COLOR("aqua", "0,255,255");
	ADD_COLOR("aquamarine", "127,255,212");
	ADD_COLOR("azure", "240,255,255");
	ADD_COLOR("beige", "245,245,220");
	ADD_COLOR("bisque", "255,228,196");
	ADD_COLOR("black", "0,0,0");
	ADD_COLOR("blanchedalmond", "255,235,205");
	ADD_COLOR("blue", "0,0,255");
	ADD_COLOR("blueviolet", "138,43,226");
	ADD_COLOR("brown", "165,42,42");
	ADD_COLOR("burlywood", "222,184,135");
	ADD_COLOR("cadetblue", "95,158,160");
	ADD_COLOR("chartreuse", "127,255,0");
	ADD_COLOR("chocolate", "210,105,30");
	ADD_COLOR("coral", "255,127,80");
	ADD_COLOR("cornflowerblue", "100,149,237");
	ADD_COLOR("cornsilk", "255,248,220");
	ADD_COLOR("crimson", "220,20,60");
	ADD_COLOR("cyan", "0,255,255");
	ADD_COLOR("darkblue", "0,0,139");
	ADD_COLOR("darkcyan", "0,139,139");
	ADD_COLOR("darkgoldenrod", "184,134,11");
	ADD_COLOR("darkgray", "169,169,169");
	ADD_COLOR("darkgreen", "0,100,0");
	ADD_COLOR("darkgrey", "169,169,169");
	ADD_COLOR("darkkhaki", "189,183,107");
	ADD_COLOR("darkmagenta", "139,0,139");
	ADD_COLOR("darkolivegreen", "85,107,47");
	ADD_COLOR("darkorange", "255,140,0");
	ADD_COLOR("darkorchid", "153,50,204");
	ADD_COLOR("darkred", "139,0,0");
	ADD_COLOR("darksalmon", "233,150,122");
	ADD_COLOR("darkseagreen", "143,188,143");
	ADD_COLOR("darkslateblue", "72,61,139");
	ADD_COLOR("darkslategray", "47,79,79");
	ADD_COLOR("darkslategrey", "47,79,79");
	ADD_COLOR("darkturquoise", "0,206,209");
	ADD_COLOR("darkviolet", "148,0,211");
	ADD_COLOR("deeppink", "255,20,147");
	ADD_COLOR("deepskyblue", "0,191,255");
	ADD_COLOR("dimgray", "105,105,105");
	ADD_COLOR("dimgrey", "105,105,105");
	ADD_COLOR("dodgerblue", "30,144,255");
	ADD_COLOR("firebrick", "178,34,34");
	ADD_COLOR("floralwhite", "255,250,240");
	ADD_COLOR("forestgreen", "34,139,34");
	ADD_COLOR("fuchsia", "255,0,255");
	ADD_COLOR("gainsboro", "220,220,220");
	ADD_COLOR("ghostwhite", "248,248,255");
	ADD_COLOR("gold", "255,215,0");
	ADD_COLOR("goldenrod", "218,165,32");
	ADD_COLOR("gray", "128,128,128");
	ADD_COLOR("grey", "128,128,128");
	ADD_COLOR("green", "0,128,0");
	ADD_COLOR("greenyellow", "173,255,47");
	ADD_COLOR("honeydew", "240,255,240");
	ADD_COLOR("hotpink", "255,105,180");
	ADD_COLOR("indianred", "205,92,92");
	ADD_COLOR("indigo", "75,0,130");
	ADD_COLOR("ivory", "255,255,240");
	ADD_COLOR("khaki", "240,230,140");
	ADD_COLOR("lavender", "230,230,250");
	ADD_COLOR("lavenderblush", "255,240,245");
	ADD_COLOR("lawngreen", "124,252,0");
	ADD_COLOR("lemonchiffon", "255,250,205");
	ADD_COLOR("lightblue", "173,216,230");
	ADD_COLOR("lightcoral", "240,128,128");
	ADD_COLOR("lightcyan", "224,255,255");
	ADD_COLOR("lightgoldenrodyellow", "250,250,210");
	ADD_COLOR("lightgray", "211,211,211");
	ADD_COLOR("lightgreen", "144,238,144");
	ADD_COLOR("lightgrey", "211,211,211");
	ADD_COLOR("lightpink", "255,182,193");
	ADD_COLOR("lightsalmon", "255,160,122");
	ADD_COLOR("lightseagreen", "32,178,170");
	ADD_COLOR("lightskyblue", "135,206,250");
	ADD_COLOR("lightslategray", "119,136,153");
	ADD_COLOR("lightslategrey", "119,136,153");
	ADD_COLOR("lightsteelblue", "176,196,222");
	ADD_COLOR("lightyellow", "255,255,224");
	ADD_COLOR("lime", "0,255,0");
	ADD_COLOR("limegreen", "50,205,50");
	ADD_COLOR("linen", "250,240,230");
	ADD_COLOR("magenta", "255,0,255");
	ADD_COLOR("maroon", "128,0,0");
	ADD_COLOR("mediumaquamarine", "102,205,170");
	ADD_COLOR("mediumblue", "0,0,205");
	ADD_COLOR("mediumorchid", "186,85,211");
	ADD_COLOR("mediumpurple", "147,112,219");
	ADD_COLOR("mediumseagreen", "60,179,113");
	ADD_COLOR("mediumslateblue", "123,104,238");
	ADD_COLOR("mediumspringgreen", "0,250,154");
	ADD_COLOR("mediumturquoise", "72,209,204");
	ADD_COLOR("mediumvioletred", "199,21,133");
	ADD_COLOR("midnightblue", "25,25,112");
	ADD_COLOR("mintcream", "245,255,250");
	ADD_COLOR("mistyrose", "255,228,225");
	ADD_COLOR("moccasin", "255,228,181");
	ADD_COLOR("navajowhite", "255,222,173");
	ADD_COLOR("navy", "0,0,128");
	ADD_COLOR("oldlace", "253,245,230");
	ADD_COLOR("olive", "128,128,0");
	ADD_COLOR("olivedrab", "107,142,35");
	ADD_COLOR("orange", "255,165,0");
	ADD_COLOR("orangered", "255,69,0");
	ADD_COLOR("orchid", "218,112,214");
	ADD_COLOR("palegoldenrod", "238,232,170");
	ADD_COLOR("palegreen", "152,251,152");
	ADD_COLOR("paleturquoise", "175,238,238");
	ADD_COLOR("palevioletred", "219,112,147");
	ADD_COLOR("papayawhip", "255,239,213");
	ADD_COLOR("peachpuff", "255,218,185");
	ADD_COLOR("peru", "205,133,63");
	ADD_COLOR("pink", "255,192,203");
	ADD_COLOR("plum", "221,160,221");
	ADD_COLOR("powderblue", "176,224,230");
	ADD_COLOR("purple", "128,0,128");
	ADD_COLOR("red", "255,0,0");
	ADD_COLOR("rosybrown", "188,143,143");
	ADD_COLOR("royalblue", "65,105,225");
	ADD_COLOR("saddlebrown", "139,69,19");
	ADD_COLOR("salmon", "250,128,114");
	ADD_COLOR("sandybrown", "244,164,96");
	ADD_COLOR("seagreen", "46,139,87");
	ADD_COLOR("seashell", "255,245,238");
	ADD_COLOR("sienna", "160,82,45");
	ADD_COLOR("silver", "192,192,192");
	ADD_COLOR("skyblue", "135,206,235");
	ADD_COLOR("slateblue", "106,90,205");
	ADD_COLOR("slategray", "112,128,144");
	ADD_COLOR("slategrey", "112,128,144");
	ADD_COLOR("snow", "255,250,250");
	ADD_COLOR("springgreen", "0,255,127");
	ADD_COLOR("steelblue", "70,130,180");
	ADD_COLOR("tan", "210,180,140");
	ADD_COLOR("teal", "0,128,128");
	ADD_COLOR("thistle", "216,191,216");
	ADD_COLOR("tomato", "255,99,71");
	ADD_COLOR("turquoise", "64,224,208");
	ADD_COLOR("violet", "238,130,238");
	ADD_COLOR("wheat", "245,222,179");
	ADD_COLOR("white", "255,255,255");
	ADD_COLOR("whitesmoke", "245,245,245");
	ADD_COLOR("yellow", "255,255,0");
	ADD_COLOR("yellowgreen", "154,205,50");
}
