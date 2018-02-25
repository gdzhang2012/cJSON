/*
  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include "unity/examples/unity_config.h"
#include "unity/src/unity.h"
#include "common.h"

static void parse_with_context_should_parse_json(void)
{
    cJSON_Context context = NULL;
    cJSON *number = NULL;

    context = cJSON_CreateContext(NULL, NULL);
    TEST_ASSERT_NOT_NULL(context);

    number = cJSON_ParseWithContext("42", sizeof("42"), context);
    TEST_ASSERT_TRUE(cJSON_IsNumber(number));

    TEST_ASSERT_EQUAL(2, cJSON_GetParseEnd(context));

    cJSON_DeleteWithContext(number, context);
    free(context);
}

static void parse_with_context_should_parse_json_without_zero_terminator(void)
{
    cJSON_Context context = NULL;
    cJSON *number = NULL;
    char fourty_two[2] = {'4', '2'};

    context = cJSON_CreateContext(NULL, NULL);
    TEST_ASSERT_NOT_NULL(context);

    number = cJSON_ParseWithContext(fourty_two, sizeof(fourty_two), context);
    TEST_ASSERT_TRUE(cJSON_IsNumber(number));

    TEST_ASSERT_EQUAL(1, cJSON_GetParseEnd(context));

    cJSON_DeleteWithContext(number, context);
    free(context);
}

static void parse_with_context_should_handle_null(void)
{
    TEST_ASSERT_NULL_MESSAGE(cJSON_ParseWithContext(NULL, 0, NULL), "Failed to handle NULL input.");
}

static void parse_with_context_should_fallback_to_default_context(void)
{
    cJSON *number = cJSON_ParseWithContext("42", sizeof("42"), NULL);
    TEST_ASSERT_TRUE(cJSON_IsNumber(number));
    delete_item(number, &global_default_context);
}

static void *custom_allocate(size_t size, void *userdata)
{
    *((size_t*)userdata) = size;
    return malloc(size);
}
static void custom_deallocate(void *pointer, void *userdata)
{
    TEST_ASSERT_NOT_EQUAL(0, *((size_t*)userdata));
    free(pointer);
}

static const cJSON_Allocators custom_allocators = {
    custom_allocate,
    custom_deallocate,
    NULL
};

static void parse_with_context_should_take_custom_allocators(void)
{
    cJSON *number = NULL;
    cJSON_Context context = NULL;
    size_t userdata = 0;

    context = cJSON_CreateContext(NULL, NULL);
    TEST_ASSERT_NOT_NULL(context);
    TEST_ASSERT_NOT_NULL(cJSON_SetAllocators(context, custom_allocators));
    TEST_ASSERT_NOT_NULL(cJSON_SetUserdata(context, &userdata));

    number = cJSON_ParseWithContext("42", sizeof("42"), context);
    TEST_ASSERT_TRUE(cJSON_IsNumber(number));
    TEST_ASSERT_EQUAL_DOUBLE(42, number->valuedouble);

    TEST_ASSERT_EQUAL(sizeof(cJSON), userdata);

    TEST_ASSERT_EQUAL(2, cJSON_GetParseEnd(context));

    cJSON_DeleteWithContext(number, context);
    free(context);
}

/*
static void parse_with_opts_should_handle_empty_strings(void)
{
    const char empty_string[] = "";
    const char *error_pointer = NULL;

    TEST_ASSERT_NULL(cJSON_ParseWithOpts(empty_string, NULL, false));
    TEST_ASSERT_EQUAL_PTR(empty_string, cJSON_GetErrorPtr());

    TEST_ASSERT_NULL(cJSON_ParseWithOpts(empty_string, &error_pointer, false));
    TEST_ASSERT_EQUAL_PTR(empty_string, error_pointer);
    TEST_ASSERT_EQUAL_PTR(empty_string, cJSON_GetErrorPtr());
}

static void parse_with_opts_should_handle_incomplete_json(void)
{
    const char json[] = "{ \"name\": ";
    const char *parse_end = NULL;

    TEST_ASSERT_NULL(cJSON_ParseWithOpts(json, &parse_end, false));
    TEST_ASSERT_EQUAL_PTR(json + strlen(json), parse_end);
    TEST_ASSERT_EQUAL_PTR(json + strlen(json), cJSON_GetErrorPtr());
}

static void parse_with_opts_should_require_null_if_requested(void)
{
    cJSON *item = cJSON_ParseWithOpts("{}", NULL, true);
    TEST_ASSERT_NOT_NULL(item);
    cJSON_Delete(item);
    item = cJSON_ParseWithOpts("{} \n", NULL, true);
    TEST_ASSERT_NOT_NULL(item);
    cJSON_Delete(item);
    TEST_ASSERT_NULL(cJSON_ParseWithOpts("{}x", NULL, true));
}

static void parse_with_opts_should_return_parse_end(void)
{
    const char json[] = "[] empty array XD";
    const char *parse_end = NULL;

    cJSON *item = cJSON_ParseWithOpts(json, &parse_end, false);
    TEST_ASSERT_NOT_NULL(item);
    TEST_ASSERT_EQUAL_PTR(json + 2, parse_end);
    cJSON_Delete(item);
}

static void parse_with_opts_should_parse_utf8_bom(void)
{
    cJSON *with_bom = NULL;
    cJSON *without_bom = NULL;

    with_bom = cJSON_ParseWithOpts("\xEF\xBB\xBF{}", NULL, true);
    TEST_ASSERT_NOT_NULL(with_bom);
    without_bom = cJSON_ParseWithOpts("{}", NULL, true);
    TEST_ASSERT_NOT_NULL(with_bom);

    TEST_ASSERT_TRUE(cJSON_Compare(with_bom, without_bom, true));

    cJSON_Delete(with_bom);
    cJSON_Delete(without_bom);
}
*/

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(parse_with_context_should_parse_json);
    RUN_TEST(parse_with_context_should_parse_json_without_zero_terminator);
    RUN_TEST(parse_with_context_should_handle_null);
    RUN_TEST(parse_with_context_should_fallback_to_default_context);
    RUN_TEST(parse_with_context_should_take_custom_allocators);
    /* RUN_TEST(parse_with_opts_should_handle_empty_strings); */
    /* RUN_TEST(parse_with_opts_should_handle_incomplete_json); */
    /* RUN_TEST(parse_with_opts_should_require_null_if_requested); */
    /* RUN_TEST(parse_with_opts_should_return_parse_end); */
    /* RUN_TEST(parse_with_opts_should_parse_utf8_bom); */

    return UNITY_END();
}
