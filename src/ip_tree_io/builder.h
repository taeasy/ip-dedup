#ifndef _HAVE_IP_TREE_IO_BUILD_DATA_H_
#define _HAVE_IP_TREE_IO_BUILD_DATA_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "../ip_tree.h"
#include "../ip.h"

struct ip_tree_build_data;
struct ip_tree_builder_parse_data;


/**
 * Buffer for parsing IP addresses.
 *
 * Some input strings may be both valid IPv4 and IPv6 addresses, e.g. "0/1",
 * so allocate separate space for both address types.
 *
 * Additionally, store the type of the parsed address.
 * */
struct ip_tree_builder_parse_data {
    int addr_type;
    struct ip4_addr_t addr_v4;
    struct ip6_addr_t addr_v6;
};


typedef int (*ip_tree_builder_parse_ip_func) (
    char* const restrict,
    const size_t,
    struct ip_tree_builder_parse_data* const restrict
);


/**
 * Depending on the IP tree builder mode,
 * IPv4, IPv6 or trees of both types may be created.
 *
 * This are stored as pointers to the actual data structure here
 * and can later be "stolen" with ip_tree_builder_steal_v4/v6().
 *
 * The parsing backend depends on the tree mode, so store that one, too.
 * */
struct ip_tree_build_data {
    struct ip_tree* v4;
    struct ip_tree* v6;

    ip_tree_builder_parse_ip_func  f_parse;
};


/**
 * Creates a new tree builder for IPv4 addresses only.
 * */
struct ip_tree_build_data* ip_tree_builder_new_v4 (void);


/**
 * Creates a new tree builder for IPv6 addresses only.
 * */
struct ip_tree_build_data* ip_tree_builder_new_v6 (void);


/**
 * Creates a new tree builder for IPv4 and IPv6 addresses.
 * */
struct ip_tree_build_data* ip_tree_builder_new_mixed (void);


/**
 * Frees a tree builder including attached IP trees (if not stolen before).
 * */
void ip_tree_builder_destroy ( struct ip_tree_build_data** const obj_ptr );


/**
 * Removes the IPv4 tree from the IP builder and passes it to the caller.
 *
 * Note: Parsing new addresses after stealing trees will result in SEGFAULTs.
 * */
struct ip_tree* ip_tree_builder_steal_v4 (
    struct ip_tree_build_data* const obj
);


/**
 * Removes the IPv6 tree from the IP builder and passes it to the caller.
 *
 * Note: Parsing new addresses after stealing trees will result in SEGFAULTs.
 * */
struct ip_tree* ip_tree_builder_steal_v6 (
    struct ip_tree_build_data* const obj
);


/**
 * Reads and parses addresses/networks from the given input stream, one per line.
 *
 * @param obj               ip tree builder data
 * @param input_stream      input stream
 * @param keep_going        whether to tolerate invalid input and just
 *                          continue with the next token on tolerable errors
 *
 * @return PARSE_IP_RET status codes, zero means success
 * */
int ip_tree_builder_parse (
    struct ip_tree_build_data* const restrict obj,
    FILE* const restrict input_stream,
    const bool keep_going
);

#endif  /* _HAVE_IP_TREE_IO_BUILD_DATA_H_ */