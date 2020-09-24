#include <stdlib.h>

#include "cdds/cdds_builtin.h"
#include "dds/dds.h"

int main(int argc, char *argv[]) {

    dds_entity_t p;
    dds_entity_t t;
    dds_entity_t w;
    NameValue sample;

    if (argc < 3) {
        printf("USAGE:\n\tkl_pub <name> <value>");
        exit(1);
    }

    p = dds_create_participant (DDS_DOMAIN_DEFAULT, NULL, NULL);
    t = dds_create_topic (p, &NameValue_desc, "NameValue", NULL, NULL);
    w = dds_create_writer (p, t, NULL, NULL);

    sample.name = argv[1];
    sample.value = argv[2];

    printf ("=== [Writing]: \n");
    while (true) {
        printf ("(%s, %s)\n", sample.name, sample.value);
        fflush (stdout);
        dds_write (w, &sample);
        dds_sleepfor (DDS_MSECS (500));

    }
  return 0;
}