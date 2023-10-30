#include "../lib/munit/munit.h"
#include "../src/pesterchum.h"

#include <stdio.h>


MunitResult do_test(const MunitParameter params[], void *fixture) {
	(void) fixture;
	char *input = (char*) munit_parameters_get(params, "input");
	const char *expected_output = munit_parameters_get(params, "expected_output");
	colorize_message(&input);
	printf("actual output: %s\n", input);
	munit_assert_string_equal(input, expected_output);
	return MUNIT_OK;
}


int main() {
	// do_test(
	// 	"<c=0,255,0>hi</c>",
	// 	"<FONT COLOR=\"#00ff00\">hi</FONT>"
	// );

	static char* input_params[] = {
		"<c=0,255,0>hi</c>",
		NULL
	};

	static char* expected_output_params[] = {
		"<FONT COLOR=\"#00ff00\">hi</FONT>",
		NULL
	};

	static MunitParameterEnum test_params[] = {
		{ "input", input_params },
		{ "expected_output", expected_output_params },
		{ NULL, NULL },
	};

	static MunitTest tests[] = {
		{
			.name       = (char*) "/rgb",
			.test       = do_test,
			.setup      = NULL,
			.tear_down  = NULL,
			.options    = MUNIT_TEST_OPTION_NONE,
			.parameters = test_params
		},
		// End-of-array marker
		{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
	};

	static const MunitSuite suite = {
		.prefix     = (char*) "/libpesterchum",
		.tests      = tests,
		.suites     = NULL,
		.iterations = 1,
		.options    = MUNIT_SUITE_OPTION_NONE
	};
	
	return munit_suite_main(&suite, NULL, 0, NULL);
}
