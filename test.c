#include <stdio.h>
#include <string.h>

#include "regexp3.h"

void printCatch(){
  char lineCatch[128];
  int i = 0, max = CatchIndex();

  while( ++i <= max )
    printf( "#%d# >%s<\n", i, getCatch( lineCatch, i ) );
}

int rtest(){
  int result = 0;
  int errs   = 0;
  int total  = 0;

#define TRUE_TEST( str, exp )                   \
  result = regexp3( str, exp );                 \
  total++;                                      \
  if( !result ){                                \
    puts( "Error on " str ", " exp );           \
    errs++;                                     \
  }

  TRUE_TEST( "linea simple", "" );
  TRUE_TEST( "linea simple", "^" );
  TRUE_TEST( "linea simple", "|" );
  TRUE_TEST( "linea simple", "()" );
  TRUE_TEST( "linea simple", "|(|)" );
  TRUE_TEST( "linea simple", "a s" );
  TRUE_TEST( "linea simple", "^linea" );
  TRUE_TEST( "linea simple", "simple$" );
  TRUE_TEST( "linea simple", "^linea simple$" );
  TRUE_TEST( "Handel", "Handel|Haendel|Dinosaurio" );
  TRUE_TEST( "Handel", "H[eioau]ndel" );
  TRUE_TEST( "Handel", "Ha-zndel" );
  TRUE_TEST( "Handel", "H[1-9a-z]ndel" );
  TRUE_TEST( "$|)}^", "([$|)}^])+" );
  TRUE_TEST( "Handel", "H[^eiou]ndel" );
  TRUE_TEST( "Handel", "H.ndel" );
  TRUE_TEST( "Hndel", "Ha?ndel" );
  TRUE_TEST( "Haaandel", "Ha+ndel" );
  TRUE_TEST( "Handel", "Hae*ndel" );
  TRUE_TEST( "Haandel", "Ha{1,100}ndel" );
  TRUE_TEST( "Haandel", "Ha{2}ndel" );
  TRUE_TEST( "Handel", "(Handel)" );
  TRUE_TEST( "Handel", "Hae*ndel" );
  TRUE_TEST( "▞▞▞▞aaaa1aaa", "((\u259e?){3,100}(\\D)*(\\d?)\\w+)*" );
  TRUE_TEST( "1999-12-05", "((\\d){2,4}(\\-|/)(\\d)*(\\W{1})(\\d+))" );
  TRUE_TEST( "<a href=\"https://es.wikipedia.org/wiki/Expresi%C3%B3n_regular\">", "((https?://)([^\"])*)" );
  TRUE_TEST( "contacto (nasciiboy@gmail.com) $$", "(\\w+@\\w+\\.\\w+)" );
  TRUE_TEST( "1. rango entre 1985-2014.", "\\D?(\\d{4})\\D?$" );
  TRUE_TEST( "linea86\nlinea87", "no|41|(di|no|saurio|((s|a|l)+i*ne(a[^\\D]{2}))\n?)+" );
  TRUE_TEST( "<>|{}()^$", "<\\<\\>\\|\\{\\}\\(\\)\\^\\$>" );

#define FALSE_TEST( str, exp )                  \
  result = regexp3( str, exp );                 \
  total++;                                      \
  if( result ){                                 \
    puts( "Error on " str ", " exp );           \
    errs++;                                     \
  }

  FALSE_TEST( "", "" );
  FALSE_TEST( "", "expresion" );
  FALSE_TEST( "linea simple", "e s" );
  FALSE_TEST( "lineo simple", "^linea" );
  FALSE_TEST( "linea simple.", "simple$" );
  FALSE_TEST( "linea simple.", "^linea simple$" );
  FALSE_TEST( "Mandel", "Handel|Haendel|Dinosaurio" );
  FALSE_TEST( "H1ndel", "H[eioau]ndel" );
  FALSE_TEST( "HAndel", "Ha-zndel" );
  FALSE_TEST( "HAndel", "H[1-9a-z]ndel" );
  FALSE_TEST( "Hendel", "H[^eiou]ndel" );
  FALSE_TEST( "Heandel", "H.ndel" );
  FALSE_TEST( "Heandel", "Ha?ndel" );
  FALSE_TEST( "Heendel", "Ha+ndel" );
  FALSE_TEST( "Hendel", "Hae*ndel" );
  FALSE_TEST( "Heendel", "Ha{1,100}ndel" );
  FALSE_TEST( "Handel", "Ha{2}ndel" );
  FALSE_TEST( "Hendel", "(Handel)" );
  FALSE_TEST( "Hendel", "Hae*ndel" );
  FALSE_TEST( "a", "((\u259e){3,100}(\\D)*(\\d?)\\w+)" );
  FALSE_TEST( "1999-12-", "((\\d){2,4}(\\-|/)(\\d)*(\\W{1})(\\d+))" );
  FALSE_TEST( "<a href=\"httpse//es.wikipedia.org/wiki/Expresi%C3%B3n_regular\">", "((https?://)([^\"])*)" );
  FALSE_TEST( "contacto (nasciiboy.gmail.com) $$", "(\\w+@\\w+\\.\\w+)" );
  FALSE_TEST( "1. rango entre 198-014.", "\\D?(\\d{4})\\D?$" );
  FALSE_TEST( "linea8\nlinea8", "no|41|(di|no|saurio|((s|a|l)+i*ne(a[^\\D]{2}))\n?)+" );

#define NTEST( str, exp, n )                            \
  result = regexp3( str, exp );                         \
  total++;                                              \
  if( result != n ){                                    \
    printf( "Error on " str ", " exp " %d\n", n );      \
    printf( "results %d\n", result );                   \
    errs++;                                             \
  }

  NTEST( "linea simple", "^", 1  );
  NTEST( "linea simple", "", 12 );
  NTEST( "linea simple", "|", 12 );
  NTEST( "linea simple", "()", 12 );
  NTEST( "linea simple", "|(|)", 12 );
  NTEST( "linea simple", "a s", 1 );
  NTEST( "linea simple", "^linea", 1 );
  NTEST( "linea simple", "simple$", 1 );
  NTEST( "linea simple", "^linea simple$", 1 );
  NTEST( "Handel", "Handel|Haendel|Dinosaurio", 1 );
  NTEST( "Dinosaurio Handel Haendel", "Handel|Haendel|Dinosaurio", 3 );
  NTEST( "1a2a3a4a5a6a", "a", 6 );
  NTEST( "1a2a3a4a5a6a", "1-6a", 6 );
  NTEST( "1a2a3a4a5a6a", "1-3a", 3 );
  NTEST( "ae ai au aw aa", "a", 6 );
  NTEST( "ae ai au aw aa", "a$", 1 );
  NTEST( "ae ai au aw aa", " aa$", 1 );
  NTEST( "ae ai au aw aa", "(a[^a])", 4 );
  NTEST( "ae ai au aw aa", "(a[^a])+", 4 );
  NTEST( "ae ai au aw aa", "(a[^a])*", 14 );
  NTEST( "ae ai au aw aa", "(a[^a])?", 14 );
  NTEST( "ae ai au aw aa", "(a[ei])?", 14 );
  NTEST( "ae ai au aw aa", "(a[ei])", 2 );
  NTEST( "ae ai au aw aa", "(a[ei] ?){2}", 1 );
  NTEST( "ae ai au aw aa", "(ae|(ai|au))", 3 );
  NTEST( "ae ai au aw aa", "<i a|u a>", 2 );
  NTEST( "ae ai au aw aa", "(i a|u a)*", 14 );
  NTEST( "ae ai au aw aa", "(i a(u a)+)*", 14 );
  NTEST( "ae ai au aw aa", "(i a(u a)+)+", 1 );
  NTEST( "linea multi anidamiento", "(linea (multi anidamiento))", 1 );
  NTEST( "linea multi opcion", "(linea|multi|opcion)", 3 );
  NTEST( "linea multi opcion", "linea (multi|opcion)", 1 );
  NTEST( "multi multiple multiples", "<multi(\\w+)?> ?", 3 );

  char line[1024];
#define CATCH_AND_REPLACE_TEST( str, exp, n, rstr, cstr )       \
  result = regexp3( str, exp );                                 \
  total++;                                                      \
  if( strcmp( replaceCatch( line, rstr, n ), cstr ) != 0 ){     \
    printf( "Error on " rstr ", " cstr "\n" );                  \
    printf( "result    >>%s<<\n"                                \
            "expected  >>%s<<\n", line, cstr );                 \
    errs++;                                                     \
  }

  CATCH_AND_REPLACE_TEST( "linea simple", "<linea>", 1, "dog", "dog simple"   );
  CATCH_AND_REPLACE_TEST( "linea simple", "<linea>", 0, "dog", "dog simple"   );
  CATCH_AND_REPLACE_TEST( "linea simple", "<linea|simple>", 0, "dog", "dog dog"   );
  CATCH_AND_REPLACE_TEST( "linea simple", "<linea|simple>", 1, "dog", "dog simple"   );
  CATCH_AND_REPLACE_TEST( "linea simple", "<linea|simple>", 2, "dog", "linea dog"   );
  CATCH_AND_REPLACE_TEST( "linea lineass lineashh", "<linea\\w*>", 1, "dog", "dog lineass lineashh" );
  CATCH_AND_REPLACE_TEST( "linea lineass lineashh", "<linea\\w*>", 2, "dog", "linea dog lineashh" );
  CATCH_AND_REPLACE_TEST( "linea lineass lineashh", "<linea\\w*>", 3, "dog", "linea lineass dog" );
  CATCH_AND_REPLACE_TEST( "linea lineass lineashh", "<linea\\w*>", 4, "dog", "linea lineass lineashh" );
  CATCH_AND_REPLACE_TEST( "linea lineass lineashh", "<linea\\w*>", 0, "dog", "dog dog dog" );
  CATCH_AND_REPLACE_TEST( "linea lineass lineashh", "<linea(\\w*)>", 0, "dog", "dog dog dog" );
  CATCH_AND_REPLACE_TEST( "linea lineass lineashh", "<linea(\\w*|raptor)>", 0, "dog", "dog dog dog" );
  CATCH_AND_REPLACE_TEST( "1988 1699 2085", "^<19<[\\d]{2}>>( (1699|2085))", 1, "raptor", "raptor 1699 2085" );
  CATCH_AND_REPLACE_TEST( "1988 1699 2085", "^<19<[\\d]{2}>>( (1699|2085))", 2, "raptor", "19raptor 1699 2085" );
  CATCH_AND_REPLACE_TEST( "1988 1699 2085", "^<19<[\\d]{2}>>( 1699 2085))$", 2, "raptor", "19raptor 1699 2085" );


  printf( "TEST %d *** ERRS %d\n\n", total, errs );
  return !errs;
}

int main(){
  if( rtest() )
    puts( "i am the raptor" );

  return 0;
}
