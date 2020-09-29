#include <stdlib.h>

#include "cdds/cdds_builtin.h"
#include "dds/dds.h"

#define MAX_SAMPLES 16

int main(int argc, char *argv[]) {

    dds_entity_t p;
    dds_entity_t t;
    dds_qos_t *qos;
    dds_entity_t r;
    KeyValue *sample[1] = {0};
    dds_sample_info_t si;

    if (argc < 2) {
        printf("USAGE:\n\tk_sub <topic-name>");
        exit(1);
    }

    p = dds_create_participant (DDS_DOMAIN_DEFAULT, NULL, NULL);
    t = dds_create_topic (p, &KeyValue_desc, argv[1], NULL, NULL);
    qos = dds_create_qos();
    dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS (10));
    dds_qset_history(qos, DDS_HISTORY_KEEP_ALL, 0);
    dds_qset_resource_limits (qos, MAX_SAMPLES, DDS_LENGTH_UNLIMITED, DDS_LENGTH_UNLIMITED);
    r = dds_create_reader (p, t, qos, NULL);

    printf ("=== [Reading] : \n");
    while (true) {
        int n = dds_take(r, (void**)sample, &si, 1, 1);
        if (n > 0 && si.valid_data) {
            printf ("[%d] - (%s, %s)\n", sample[0]->sn, sample[0]->key, sample[0]->value);
            fflush (stdout);
        }
        dds_sleepfor (DDS_MSECS (500));
    }
  return 0;
}