#include <stdlib.h>

#include "cdds/cdds_builtin.h"
#include "dds/dds.h"

#define MAX_SAMPLES 16

int main(int argc, char *argv[]) {

    dds_entity_t p;
    dds_entity_t t;
    dds_entity_t w;
    dds_qos_t *qos;
    KeyValue sample;

    if (argc < 4) {
        printf("USAGE:\n\tkl_pub <topic-name> <name> <value>");
        exit(1);
    }

    p = dds_create_participant (DDS_DOMAIN_DEFAULT, NULL, NULL);
    t = dds_create_topic (p, &KeyValue_desc, argv[1], NULL, NULL);
    qos = dds_create_qos();
    dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS (10));
    dds_qset_history(qos, DDS_HISTORY_KEEP_ALL, 0);
    dds_qset_resource_limits (qos, MAX_SAMPLES, DDS_LENGTH_UNLIMITED, DDS_LENGTH_UNLIMITED);
    w = dds_create_writer (p, t, qos, NULL);

    sample.key = argv[2];
    sample.value = argv[3];

    printf ("=== [Writing]: \n");
    int i = 0;
    while (true) {
        sample.sn = i;
        printf ("[%d] - (%s, %s)\n", i, sample.key, sample.value);
        fflush (stdout);
        dds_write (w, &sample);
        dds_sleepfor (DDS_MSECS (500));
        i += 1;
    }
  return 0;
}