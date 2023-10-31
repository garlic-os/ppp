#include <string>
#include "../src/pesterchum.h"
#include "./catch_amalgamated.hpp"


void do_test(std::string input, std::string expected_output) {
	char *message = static_cast<char*>(malloc(input.size() + 1));
	strcpy(message, input.c_str());
	colorize_message(&message);
	CHECK(expected_output == message);
	free(message);
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
			"<FONT COLOR=\"#ff00ff\">computer</FONT"
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
