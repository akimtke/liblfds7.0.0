/****************************************************************************/
#if( defined __GNUC__ )

  /* TRD : GCC, 32-bit ARM

           ARM is LL/SC and uses a reservation granule of 8 to 2048 bytes
           so the isolation value used here is worst-case - be sure to set
           this correctly, otherwise structures are painfully large
  */

  #ifdef LFDS700_PAL_PORTING_ABSTRACTION_LAYER_PROCESSOR
    #error More than one porting abstraction layer matches the current platform in lfds700_porting_abstraction_layer_processor.h
  #endif

  #define LFDS700_PAL_PORTING_ABSTRACTION_LAYER_PROCESSOR

  typedef int long unsigned lfds700_pal_atom_t;
  typedef int long unsigned lfds700_pal_uint_t;

  #define LFDS700_PAL_PROCESSOR_STRING            "ARM (32-bit)"

  #define LFDS700_PAL_ALIGN_SINGLE_POINTER        4
  #define LFDS700_PAL_ALIGN_DOUBLE_POINTER        8

  #define LFDS700_PAL_CACHE_LINE_LENGTH_IN_BYTES  32
  #define LFDS700_PAL_ATOMIC_ISOLATION_IN_BYTES   2048

#endif

/****************************************************************************/
#if( !defined LFDS700_PAL_PORTING_ABSTRACTION_LAYER_PROCESSOR )

  #error No matching porting abstraction layer in lfds700_porting_abstraction_layer_processor.h

#endif
