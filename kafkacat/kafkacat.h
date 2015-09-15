/*
 * kafkacat - Apache Kafka consumer and producer
 *
 * Copyright (c) 2015, Magnus Edenhill
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <librdkafka/rdkafka.h>

#include "config.h"


typedef enum {
        KC_FMT_STR,
        KC_FMT_OFFSET,
        KC_FMT_KEY,
        KC_FMT_KEY_LEN,
        KC_FMT_PAYLOAD,
        KC_FMT_PAYLOAD_LEN,
        KC_FMT_TOPIC,
        KC_FMT_PARTITION,
} fmt_type_t;

#define KC_FMT_MAX_SIZE  128

struct conf {
        int     run;
        int     verbosity;
        int     exitcode;
        char    mode;
        int     flags;
#define CONF_F_FMT_JSON   0x1 /* JSON formatting */
#define CONF_F_KEY_DELIM  0x2 /* Producer: use key delimiter */
#define CONF_F_OFFSET     0x4 /* Print offsets */
#define CONF_F_TEE        0x8 /* Tee output when producing */
#define CONF_F_NULL       0x10 /* Send empty messages as NULL */
        int     delim;
        int     key_delim;

        struct {
                fmt_type_t type;
                const char *str;
                int         str_len;
        } fmt[KC_FMT_MAX_SIZE];
        int     fmt_cnt;
        int     msg_size;
        char   *brokers;
        char   *topic;
        int32_t partition;
        int64_t offset;
        int     exit_eof;
        int64_t msg_cnt;
        char   *null_str;
        int     null_str_len;

        rd_kafka_conf_t       *rk_conf;
        rd_kafka_topic_conf_t *rkt_conf;

        rd_kafka_t            *rk;
        rd_kafka_topic_t      *rkt;

        char   *debug;
        int     conf_dump;
};

extern struct conf conf;


void __attribute__((noreturn)) fatal0 (const char *func, int line,
                                       const char *fmt, ...);

#define FATAL(fmt...)  fatal0(__FUNCTION__, __LINE__, fmt)

/* Info printout */
#define INFO(VERBLVL,FMT...) do {                    \
                if (conf.verbosity >= (VERBLVL))     \
                        fprintf(stderr, "%% " FMT);  \
        } while (0)



/*
 * format.c
 */
void fmt_msg_output (FILE *fp, const rd_kafka_message_t *rkmessage);

void fmt_parse (const char *fmt);

void fmt_init (void);
void fmt_term (void);



#if ENABLE_JSON
/*
 * json.c
 */
void fmt_msg_output_json (FILE *fp, const rd_kafka_message_t *rkmessage);
void metadata_print_json (const struct rd_kafka_metadata *metadata);

void fmt_init_json (void);
void fmt_term_json (void);

#endif
