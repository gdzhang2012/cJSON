#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

/* Create a bunch of objects as demonstration. */
static int print_objects(cJSON *root)
{
    char *out = NULL;

    /* formatted print */
    out = cJSON_Print(root);

    if (out)
    {
        printf("%s\n", out);

        free(out);
    }

    return 0;
}

/* Create a bunch of objects as demonstration. */
static void create_objects(void)
{
    /* declare a few. */
    cJSON *root = NULL;
    /*
    cJSON *fmt = NULL;
    */

    /* Here we construct some JSON standards, from the JSON site. */

    /* Our "Video" datatype: */
    root = cJSON_CreateObject();
    /*
    cJSON_AddStringToObject(root, "name", "Jack (\"Bee\") Nimble");
    cJSON_AddItemToObject(root, "format", fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "type", "rect");
    cJSON_AddNumberToObject(fmt, "width", 1920);
    cJSON_AddNumberToObject(fmt, "height", 1080);
    cJSON_AddFalseToObject (fmt, "interlace");
    cJSON_AddNumberToObject(fmt, "frame rate", 24);
    */
    cJSON_AddStringToObject(root, "version", "4");

    /* Print to text */
    if (print_objects(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);
}

int main(void)
{
    /* Now some samplecode for building objects concisely: */
    create_objects();

    return 0;
}
