#include <stdlib.h>
#include <string.h>

#include "cdds/cdds_builtin.h"
#include "dds/dds.h"

#define MAX_SAMPLES 16

int main(int argc, char *argv[]) {

    dds_entity_t p;
    dds_entity_t t;
    dds_qos_t *qos = dds_create_qos();
    dds_entity_t w;
    NamedBlob sample;
    uint32_t value_len = 0;
    uint32_t did = DDS_DOMAIN_DEFAULT;

    p = dds_create_participant (did, NULL, NULL);

    if (argc < 4) {
        printf("USAGE:\n\tnb_pub <topic-name> <name> <blob-len> [<domain-id>]");
        exit(1);
    }
    value_len = strtoul(argv[3], &argv[3] + strlen(argv[3]), 10);
    printf("Value length: %u\n", value_len);

    if (argc > 4) {
        did = strtoul(argv[4], &argv[4] + strlen(argv[4]), 10);
        printf("Creating participant on domain %u\n", did);
    }


    sample.value._length = value_len;
    sample.value._buffer = value_len ? dds_alloc (value_len) : NULL;
    sample.value._release = true;
    sample.value._maximum = value_len;

    for (int i =0; i < value_len; ++i) {
        sample.value._buffer[i] = (uint8_t)i % 10;
    }

    p = dds_create_participant (did, NULL, NULL);
    t = dds_create_topic (p, &NamedBlob_desc, argv[1], NULL, NULL);
    dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS (30));
    dds_qset_history(qos, DDS_HISTORY_KEEP_ALL, 0);
    dds_qset_resource_limits (qos, MAX_SAMPLES, DDS_LENGTH_UNLIMITED, DDS_LENGTH_UNLIMITED);
    w = dds_create_writer (p, t, qos, NULL);

    sample.name = argv[2];
    int i = 0;
    printf ("=== [Writing]: \n");
    while (true) {
        sample.sn = i;
        printf ("[%d] -(%s, %s)\n", i, sample.name, "...");
        fflush (stdout);
        dds_write (w, &sample);
        dds_sleepfor (DDS_MSECS (1000));
        i += 1;
    }
    return 0;
}
