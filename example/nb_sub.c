#include <stdlib.h>
#include <string.h>

#include "cdds/cdds_builtin.h"
#include "dds/dds.h"

#define MAX_SAMPLES 16

int validate(dds_sequence_t *xs)
{
    for (int i = 0; i < xs->_length; ++i)
    {
        if (xs->_buffer[i] != (uint8_t)i % 10)
        {
            return i;
        }
    }
    return 0;
}

void on_data_available(dds_entity_t r, void *arg)
{
    NamedBlob *sample[1] = {0};
    dds_sample_info_t si;
    int rv = 0;
    while (dds_take(r, (void **)sample, &si, 1, 1) > 0)
    {
        if (si.valid_data)
        {
            rv = validate(&sample[0]->value);
            if (rv == 0)
                printf("[%d] - %s - %d bytes -- Valid\n", sample[0]->sn, sample[0]->name, sample[0]->value._length);
            else
                printf("[%d] - %s - %d bytes received but invalid byte found at index %d\n", sample[0]->sn, sample[0]->name, sample[0]->value._length, rv);
            fflush(stdout);
        }
    }
}

int main(int argc, char *argv[])
{

    dds_entity_t p;
    dds_entity_t t;
    dds_qos_t *qos;
    dds_entity_t r;
    dds_listener_t *l = dds_create_listener(0);
    NamedBlob sample;
    uint32_t value_len = 0;
    uint32_t did = DDS_DOMAIN_DEFAULT;

    p = dds_create_participant(did, NULL, NULL);

    if (argc < 3)
    {
        printf("USAGE:\n\tnb_pub <topic-name> <name> [<domain-id>]");
        exit(1);
    }
    if (argc > 3)
    {
        did = strtoul(argv[3], &argv[3] + strlen(argv[3]), 10);
        printf("Creating participant on domain %u\n", did);
    }
    p = dds_create_participant(did, NULL, NULL);
    t = dds_create_topic(p, &NamedBlob_desc, argv[1], NULL, NULL);
    dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS(30));
    dds_qset_history(qos, DDS_HISTORY_KEEP_ALL, 0);
    dds_qset_resource_limits(qos, MAX_SAMPLES, DDS_LENGTH_UNLIMITED, DDS_LENGTH_UNLIMITED);
    dds_lset_data_available(l, on_data_available);
    r = dds_create_reader(p, t, qos, l);

    printf("Press any key to stop reading... \n");
    char ch;
    read(STDIN_FILENO, &ch, 1);
    return 0;
}
