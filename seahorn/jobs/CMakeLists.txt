##################################################################
#                           Tango                                #
##################################################################

# tcache
add_subdirectory(tango/tcache/tango_tcache_bounds_init_delete)
add_subdirectory(tango/tcache/tango_tcache_bounds_query)
add_subdirectory(tango/tcache/tango_tcache_bounds_insert)

# dcache
add_subdirectory(tango/dcache/tango_dcache_bounds_init_delete)
add_subdirectory(tango/dcache/tango_dcache_bounds_compact)

# mcache
add_subdirectory(tango/mcache/tango_mcache_bounds_init_delete)
add_subdirectory(tango/mcache/tango_mcache_bounds_publish)
add_subdirectory(tango/mcache/tango_mcache_bounds_query)

# fseq
add_subdirectory(tango/fseq/tango_fseq_bounds_init_delete)
add_subdirectory(tango/fseq/tango_fseq_bounds_query)

# fctl
add_subdirectory(tango/fctl/tango_fctl_bounds_init_delete)
add_subdirectory(tango/fctl/tango_fctl_bounds_cfg_rx_add_done)

##################################################################
#                           Waltz                                #
##################################################################

# quic
add_subdirectory(waltz/quic/waltz_quic_decode_initial)
add_subdirectory(waltz/quic/waltz_quic_decrypt)

# tls
add_subdirectory(waltz/tls/waltz_quic_tls_client_hs)
add_subdirectory(waltz/tls/waltz_quic_tls_server_hs)


##################################################################
#                           Util                                 #
##################################################################
# hash
add_subdirectory(util/util_fd_hash)

# fibre
add_subdirectory(util/util_fd_fibre)
add_subdirectory(util/util_fd_fibre_schedule)

# cstr
add_subdirectory(util/util_fd_cstr_to_ulong_seq)

# bits
add_subdirectory(util/util_fd_bits_svw)

# pod
add_subdirectory(util/util_fd_pod_init_delete)
add_subdirectory(util/util_fd_pod_iter)
add_subdirectory(util/util_fd_pod_accessors)
add_subdirectory(util/util_fd_pod_resize)
add_subdirectory(util/util_fd_pod_query)

##################################################################
#                           Ballet                               #
##################################################################
# txn
add_subdirectory(ballet/txn/ballet_txn_parse)
add_subdirectory(ballet/txn/ballet_txn_parse_minsz)

# chacha20
add_subdirectory(ballet/chacha20/ballet_chacha20_block)
add_subdirectory(ballet/chacha20/ballet_chacha20rng_init_delete)