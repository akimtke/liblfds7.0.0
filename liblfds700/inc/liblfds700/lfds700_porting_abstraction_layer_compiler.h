/****************************************************************************/
#if( defined __GNUC__ )
  // TRD : makes checking GCC versions much tidier
  #define LFDS700_PAL_GCC_VERSION ( __GNUC__ * 100 + __GNUC_MINOR__ * 10 + __GNUC_PATCHLEVEL__ )
#endif

/****************************************************************************/
#if( defined __GNUC__ && LFDS700_PAL_GCC_VERSION >= 412 && LFDS700_PAL_GCC_VERSION < 473 )

  /* TRD : GCC 4.1.2 up to 4.7.3

           __GNUC__                 indicates GCC
           LFDS700_PAL_GCC_VERSION  indicates which version
                                      - __sync_synchronize requires 4.1.2

           GCC 4.1.2 introduced the __sync_*() atomic intrinsics
  */

  #ifdef LFDS700_PAL_PORTING_ABSTRACTION_LAYER_COMPILER
    #error More than one porting abstraction layer matches the current platform in lfds700_porting_abstraction_layer_compiler.h
  #endif

  #define LFDS700_PAL_PORTING_ABSTRACTION_LAYER_COMPILER

  #define LFDS700_PAL_COMPILER_STRING          "GCC < 4.7.3"

  #define LFDS700_PAL_ALIGN(alignment)         __attribute__( (aligned(alignment)) )
  #define LFDS700_PAL_INLINE                   inline

  static LFDS700_PAL_INLINE void lfds700_pal_barrier_compiler( void )
  {
    __asm__ __volatile__ ( "" : : : "memory" );
  }

  #define LFDS700_PAL_BARRIER_COMPILER_LOAD    lfds700_pal_barrier_compiler()
  #define LFDS700_PAL_BARRIER_COMPILER_STORE   lfds700_pal_barrier_compiler()
  #define LFDS700_PAL_BARRIER_COMPILER_FULL    lfds700_pal_barrier_compiler()

  #define LFDS700_PAL_BARRIER_PROCESSOR_LOAD   __sync_synchronize()
  #define LFDS700_PAL_BARRIER_PROCESSOR_STORE  __sync_synchronize()
  #define LFDS700_PAL_BARRIER_PROCESSOR_FULL   __sync_synchronize()

  #define LFDS700_PAL_ATOMIC_CAS( pointer_to_destination, pointer_to_compare, new_destination, cas_strength, result )       \
  {                                                                                                                         \
    lfds700_pal_atom_t                                                                                                      \
      original_compare;                                                                                                     \
                                                                                                                            \
    /* LFDS700_PAL_ASSERT( (pointer_to_destination) != NULL ); */                                                           \
    /* LFDS700_PAL_ASSERT( (pointer_to_compare) != NULL ); */                                                               \
    /* TRD : new_destination can be any value in its range */                                                               \
    /* TRD : cas_strength can be any value in its range */                                                                  \
    /* TRD : result can be any value in its range */                                                                        \
                                                                                                                            \
    original_compare = (lfds700_pal_atom_t) *(pointer_to_compare);                                                          \
                                                                                                                            \
    LFDS700_PAL_BARRIER_COMPILER_FULL;                                                                                      \
    *(pointer_to_compare) = __sync_val_compare_and_swap( pointer_to_destination, *(pointer_to_compare), new_destination );  \
    LFDS700_PAL_BARRIER_COMPILER_FULL;                                                                                      \
                                                                                                                            \
    result = (unsigned char) ( original_compare == (lfds700_pal_atom_t) *(pointer_to_compare) );                            \
  }

  // TRD : ARM and x86 have DWCAS which we can get via GCC intrinsics
  #if( defined __arm__ || defined __i686__ || defined __i586__ || defined __i486__ )
    #define LFDS700_PAL_ATOMIC_DWCAS( pointer_to_destination, pointer_to_compare, pointer_to_new_destination, cas_strength, result )                                                                                                   \
    {                                                                                                                                                                                                                                  \
      int long long unsigned                                                                                                                                                                                                           \
        original_destination;                                                                                                                                                                                                          \
                                                                                                                                                                                                                                       \
      /* LFDS700_PAL_ASSERT( (pointer_to_destination) != NULL ); */                                                                                                                                                                    \
      /* LFDS700_PAL_ASSERT( (pointer_to_compare) != NULL ); */                                                                                                                                                                        \
      /* LFDS700_PAL_ASSERT( (pointer_to_new_destination) != NULL ); */                                                                                                                                                                \
      /* TRD : cas_strength can be any value in its range */                                                                                                                                                                           \
      /* TRD : result can be any value in its range */                                                                                                                                                                                 \
                                                                                                                                                                                                                                       \
      LFDS700_PAL_BARRIER_COMPILER_FULL;                                                                                                                                                                                               \
      original_destination = __sync_val_compare_and_swap( (int long long unsigned volatile *) (pointer_to_destination), *(int long long unsigned *) (pointer_to_compare), *(int long long unsigned *) (pointer_to_new_destination) );  \
      LFDS700_PAL_BARRIER_COMPILER_FULL;                                                                                                                                                                                               \
                                                                                                                                                                                                                                       \
      result = (char unsigned) ( original_destination == *(int long long unsigned *) (pointer_to_compare) );                                                                                                                           \
                                                                                                                                                                                                                                       \
      *(int long long unsigned *) (pointer_to_compare) = original_destination;                                                                                                                                                         \
    }
  #endif

  #define LFDS700_PAL_ATOMIC_EXCHANGE( pointer_to_destination, pointer_to_exchange )                                                                   \
  {                                                                                                                                                    \
    /* LFDS700_PAL_ASSERT( (pointer_to_destination) != NULL ); */                                                                                      \
    /* LFDS700_PAL_ASSERT( (pointer_to_exchange) != NULL ); */                                                                                         \
                                                                                                                                                       \
    LFDS700_PAL_BARRIER_COMPILER_FULL;                                                                                                                 \
    *( (lfds700_pal_atom_t *) pointer_to_exchange) = (lfds700_pal_atom_t) __sync_lock_test_and_set( pointer_to_destination, *(pointer_to_exchange) );  \
    LFDS700_PAL_BARRIER_COMPILER_FULL;                                                                                                                 \
  }

#endif

/****************************************************************************/
#if( defined __GNUC__ && LFDS700_PAL_GCC_VERSION >= 473 )

  /* TRD : GCC 4.7.3 and greater

           __GNUC__                 indicates GCC
           LFDS700_PAL_GCC_VERSION  indicates which version
                                      - __atomic_thread_fence requires 4.7.3

           GCC 4.7.3 introduced the better __atomic*() atomic intrinsics
  */

  #ifdef LFDS700_PAL_PORTING_ABSTRACTION_LAYER_COMPILER
    #error More than one porting abstraction layer matches the current platform in lfds700_porting_abstraction_layer_compiler.h
  #endif

  #define LFDS700_PAL_PORTING_ABSTRACTION_LAYER_COMPILER

  #define LFDS700_PAL_COMPILER_STRING          "GCC >= 4.7.3"

  #define LFDS700_PAL_ALIGN(alignment)         __attribute__( (aligned(alignment)) )
  #define LFDS700_PAL_INLINE                   inline

  // TRD : GCC >= 4.7.3 compiler barriers are built into the intrinsics
  #define LFDS700_PAL_NO_COMPILER_BARRIERS

  #define LFDS700_PAL_BARRIER_PROCESSOR_LOAD   __atomic_thread_fence( __ATOMIC_ACQUIRE )
  #define LFDS700_PAL_BARRIER_PROCESSOR_STORE  __atomic_thread_fence( __ATOMIC_RELEASE )
  #define LFDS700_PAL_BARRIER_PROCESSOR_FULL   __atomic_thread_fence( __ATOMIC_ACQ_REL )

  #define LFDS700_PAL_ATOMIC_CAS( pointer_to_destination, pointer_to_compare, new_destination, cas_strength, result )                                                                      \
  {                                                                                                                                                                                        \
    /* LFDS700_PAL_ASSERT( (pointer_to_destination) != NULL ); */                                                                                                                          \
    /* LFDS700_PAL_ASSERT( (pointer_to_compare) != NULL ); */                                                                                                                              \
    /* TRD : new_destination can be any value in its range */                                                                                                                              \
    /* TRD : cas_strength can be any value in its range */                                                                                                                                 \
    /* TRD : result can be any value in its range */                                                                                                                                       \
                                                                                                                                                                                           \
    result = (char unsigned) __atomic_compare_exchange_n( pointer_to_destination, (void *) (pointer_to_compare), (new_destination), (cas_strength), __ATOMIC_RELAXED, __ATOMIC_RELAXED );  \
  }

  // TRD : ARM and x86 have DWCAS which we can get via GCC intrinsics
  #if( defined __arm__ || defined __i686__ || defined __i586__ || defined __i486__ )
    #define LFDS700_PAL_ATOMIC_DWCAS( pointer_to_destination, pointer_to_compare, pointer_to_new_destination, cas_strength, result )                                                                                                                                                          \
    {                                                                                                                                                                                                                                                                                         \
      /* LFDS700_PAL_ASSERT( (pointer_to_destination) != NULL ); */                                                                                                                                                                                                                           \
      /* LFDS700_PAL_ASSERT( (pointer_to_compare) != NULL ); */                                                                                                                                                                                                                               \
      /* LFDS700_PAL_ASSERT( (pointer_to_new_destination) != NULL ); */                                                                                                                                                                                                                       \
      /* TRD : cas_strength can be any value in its range */                                                                                                                                                                                                                                  \
      /* TRD : result can be any value in its range */                                                                                                                                                                                                                                        \
                                                                                                                                                                                                                                                                                              \
      (result) = (char unsigned) __atomic_compare_exchange_n( (int long long unsigned volatile *) (pointer_to_destination), (int long long unsigned *) (pointer_to_compare), *(int long long unsigned *) (pointer_to_new_destination), (cas_strength), __ATOMIC_RELAXED, __ATOMIC_RELAXED );  \
    }
  #endif

  #if( defined __x86_64__ )
    /* TRD : __GNUC__    indicates GCC
                           - __asm__ requires GCC
                           - __volatile__ requires GCC
             __x86_64__  indicates x64
                           - cmpxchg16b requires x64

             On 64 bit platforms, unsigned long long int is 64 bit, so we must manually use cmpxchg16b,
             as __sync_val_compare_and_swap() will only emit cmpxchg8b
    */

    // TRD : lfds700_pal_atom_t volatile (*destination)[2], lfds700_pal_atom_t (*compare)[2], lfds700_pal_atom_t (*new_destination)[2], enum lfds700_misc_cas_strength cas_strength, char unsigned result

    #define LFDS700_PAL_ATOMIC_DWCAS( pointer_to_destination, pointer_to_compare, pointer_to_new_destination, cas_strength, result )  \
    {                                                                                                                                 \
      /* LFDS700_PAL_ASSERT( (pointer_to_destination) != NULL ); */                                                                   \
      /* LFDS700_PAL_ASSERT( (pointer_to_compare) != NULL ); */                                                                       \
      /* LFDS700_PAL_ASSERT( (pointer_to_new_destination) != NULL ); */                                                               \
      /* TRD : cas_strength can be any value in its range */                                                                          \
      /* TRD : result can be any value in its range */                                                                                \
                                                                                                                                      \
      (result) = 0;                                                                                                                   \
                                                                                                                                      \
      __asm__ __volatile__                                                                                                            \
      (                                                                                                                               \
        "lock;"           /* make cmpxchg16b atomic        */                                                                         \
        "cmpxchg16b %0;"  /* cmpxchg16b sets ZF on success */                                                                         \
        "setz       %3;"  /* if ZF set, set result to 1    */                                                                         \
                                                                                                                                      \
        /* output */                                                                                                                  \
        : "+m" (*pointer_to_destination), "+a" ((pointer_to_compare)[0]), "+d" ((pointer_to_compare)[1]), "=q" (result)               \
                                                                                                                                      \
        /* input */                                                                                                                   \
        : "b" ((pointer_to_new_destination)[0]), "c" ((pointer_to_new_destination)[1])                                                \
                                                                                                                                      \
        /* clobbered */                                                                                                               \
        : "cc", "memory"                                                                                                              \
      );                                                                                                                              \
    }
  #endif

  #define LFDS700_PAL_ATOMIC_EXCHANGE( pointer_to_destination, pointer_to_exchange )                                     \
  {                                                                                                                      \
    /* LFDS700_PAL_ASSERT( (pointer_to_destination) != NULL ); */                                                        \
    /* LFDS700_PAL_ASSERT( (pointer_to_exchange) != NULL ); */                                                           \
                                                                                                                         \
    *(pointer_to_exchange) = __atomic_exchange_n( (pointer_to_destination), *(pointer_to_exchange), __ATOMIC_RELAXED );  \
  }

#endif
