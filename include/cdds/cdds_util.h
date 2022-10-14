#ifndef ATOLAB_CDDS_UTIL_H_
#define ATOLAB_CDDS_UTIL_H_


#include "dds/dds.h"
#include "dds/ddsi/ddsi_serdata.h"
#include "dds/ddsi/q_radmin.h"

//#define CY_DEBUG_ON 1
#ifdef CY_DEBUG_ON
    #define CY_DEBUG(msg) printf(msg)
    #define CY_DEBUG_WA(fmt, args...) printf(fmt, args)
#else
    #define CY_DEBUG(msg)
    #define CY_DEBUG_WA(fmt, args...)
#endif

struct cdds_ddsi_payload {
  struct ddsi_serdata sd;
  size_t size;
  enum ddsi_serdata_kind kind;
  unsigned char* payload;
};

struct cdds_ddsi_payload* cdds_ddsi_payload_create(struct ddsi_sertype *st, enum ddsi_serdata_kind kind, unsigned char* pload, size_t size);
void cdds_ddsi_payload_free(struct cdds_ddsi_payload* p);
unsigned char* cdds_ddsi_payload_get(struct cdds_ddsi_payload* p);
size_t cdds_ddsi_payload_len(struct cdds_ddsi_payload* p);
enum ddsi_serdata_kind cdds_ddsi_payload_kind(struct cdds_ddsi_payload* p);


struct ddsi_sertype* cdds_create_blob_sertype(dds_entity_t dp, char* type_name, bool is_keyless);

dds_entity_t cdds_create_blob_topic(dds_entity_t dp, char *topic_name, char* type_name, bool is_keyless);

int cdds_take_blob(dds_entity_t rd, struct cdds_ddsi_payload** sample, dds_sample_info_t * si);

void cdds_serdata_ref(struct ddsi_serdata *sd);
void cdds_serdata_unref(struct ddsi_serdata *sd);
void cdds_sertype_ref(struct ddsi_sertype *st);
void cdds_sertype_unref(struct ddsi_sertype *st);

#endif /* ATOLAB_CDDS_UTIL_H_ */
