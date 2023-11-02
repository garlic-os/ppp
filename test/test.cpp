#include <string>
#include "../src/pesterchum.h"
#include "./catch_amalgamated.hpp"


void do_test(std::string input, std::string expected_output) {
	INFO("Input: " << input);
	char *actual_output = strdup(input.c_str());
	convert_message(&actual_output);
	CHECK(expected_output == actual_output);
	free(actual_output);
}


TEST_CASE("Color tags", "[color]") {
	SECTION("Basic / hex code") {
		do_test(
			"&lt;c=#ff00ff&gt;hi&lt;/c&gt;",
			"<FONT COLOR=\"#ff00ff\">hi</FONT>"
		);
	}

	SECTION("Longer text content") {
		do_test(
			"&lt;c=#ff00ff&gt;computer&lt;/c&gt;",
			"<FONT COLOR=\"#ff00ff\">computer</FONT>"
		);
	}

	SECTION("Text before tag") {
		do_test(
			"guh&lt;c=#ff00ff&gt;hi&lt;/c&gt;",
			"guh<FONT COLOR=\"#ff00ff\">hi</FONT>"
		);
	}

	SECTION("Text after tag") {
		do_test(
			"&lt;c=#ff00ff&gt;hi&lt;/c&gt;guh",
			"<FONT COLOR=\"#ff00ff\">hi</FONT>guh"
		);
	}

	SECTION("Multiple tags") {
		do_test(
			"&lt;c=#ff00ff&gt;hi&lt;/c&gt;&lt;c=#00ff00&gt;bye&lt;/c&gt;",
			"<FONT COLOR=\"#ff00ff\">hi</FONT><FONT COLOR=\"#00ff00\">bye</FONT>"
		);
	}

	SECTION("RGB") {
		do_test(
			"&lt;c=0,255,0&gt;hi&lt;/c&gt;",
			"<FONT COLOR=\"rgb(0,255,0)\">hi</FONT>"
		);
	}

	SECTION("Color name") {
		do_test(
			"&lt;c=red&gt;hi&lt;/c&gt;",
			"<FONT COLOR=\"red\">hi</FONT>"
		);
	}
};
