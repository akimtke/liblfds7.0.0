/****************************************************************************/
#if( defined __GNUC__ )

  // TRD : GCC, freestanding or bare implementation

  #ifdef LFDS700_PAL_PORTING_ABSTRACTION_LAYER_OPERATING_SYSTEM
    #error More than one porting abstraction layer matches the current platform in lfds700_porting_abstraction_layer_operating_system.h
  #endif

  #define LFDS700_PAL_PORTING_ABSTRACTION_LAYER_OPERATING_SYSTEM

  #define LFDS700_PAL_OS_STRING             "Embedded (freestanding/bare)"
  #define LFDS700_PAL_ASSERT( expression )

#endif

/****************************************************************************/
#if( !defined LFDS700_PAL_PORTING_ABSTRACTION_LAYER_OPERATING_SYSTEM )

  #error No matching porting abstraction layer in lfds700_porting_abstraction_layer_operating_system.h

#endif
