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
			"<c=#ff00ff>hi</c>",
			"<FONT COLOR=\"#ff00ff\">hi</FONT>"
		);
	}

	SECTION("Longer text content") {
		do_test(
			"<c=#ff00ff>computer</c>",
			"<FONT COLOR=\"#ff00ff\">computer</FONT>"
		);
	}

	SECTION("Text before tag") {
		do_test(
			"guh<c=#ff00ff>hi</c>",
			"guh<FONT COLOR=\"#ff00ff\">hi</FONT>"
		);
	}

	SECTION("Text after tag") {
		do_test(
			"<c=#ff00ff>hi</c>guh",
			"<FONT COLOR=\"#ff00ff\">hi</FONT>guh"
		);
	}

	SECTION("Multiple tags") {
		do_test(
			"<c=#ff00ff>hi</c><c=#00ff00>bye</c>",
			"<FONT COLOR=\"#ff00ff\">hi</FONT><FONT COLOR=\"#00ff00\">bye</FONT>"
		);
	}

	SECTION("RGB") {
		do_test(
			"<c=0,255,0>hi</c>",
			"<FONT COLOR=\"rgb(0,255,0)\">hi</FONT>"
		);
	}

	SECTION("Color name") {
		do_test(
			"<c=red>hi</c>",
			"<FONT COLOR=\"red\">hi</FONT>"
		);
	}
};
