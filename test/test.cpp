#include <string>
#include "../src/pesterchum.h"
#include "./catch_amalgamated.hpp"

using S = std::string;
#define C(cstr) (const_cast<char*>(cstr))


TEST_CASE("Color tags are converted", "[color]") {
	char *message;
	message = C("<c=#ff00ff>hi</c>");
	colorize_message(&message);
	CHECK(S("<FONT COLOR=\"#ff00ff\">hi</FONT>") == message);

	message = C("guh<c=#ff00ff>hi</c>");
	colorize_message(&message);
	CHECK(S("guh<FONT COLOR=\"#ff00ff\">hi</FONT>") == message);

	message = C("<c=#ff00ff>computer</c>");
	colorize_message(&message);
	CHECK(S("<FONT COLOR=\"#ff00ff\">computer</FONT>") == message);
};
