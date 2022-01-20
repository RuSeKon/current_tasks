#if 1
  #define ASSERT(expr) \
    if (! (expr)) \
      abort();
#else
  #define ASSERT(expr)
#endif


#define ASSERT(expr) \
  if (! (expr)) \
    { \
      fprintf (stderr, "assertion \"%s\" failed at %s:%d\n", #expr, __FILE__, __LINE__); \
      exit (1); \
    }

#define ASSERT(expr) \
  do { \
    if (! (expr)) \
      { \
        fprintf (stderr, "assertion \"%s\" failed at %s:%d\n", #expr, __FILE__, __LINE__); \
        exit (1); \
      } \
  } while (0)

//FINALY ASSERT IMPLEMENTATION

#if 1

  /* Отладочная версия */
  #define ASSERT(expr) \
    do { \
      if (expr) \
        /* nothing */ ; \
      else \
        { \
          fprintf (stderr, "assertion \"%s\" failed at %s:%d\n", #expr, __FILE__, __LINE__); \
          exit (1); \
        } \
    } while (0)

#else

  /* Боевая версия */
  #define ASSERT(expr) \
    (void) (0 && (expr))

#endif
