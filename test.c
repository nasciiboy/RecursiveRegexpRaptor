#include <stdio.h>

#include "regexp3.h"

void printCatch(){
  int i = 0, max = CatchIndex();

  while( ++i <= max )
    printf( "#%d# >%s<\n", i, getCatch( i ) );
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

  printf( "TEST %d *** ERRS %d\n\n", total, errs );
  return !errs;
}

int main(){
  if( rtest() )
    puts( "i am the walrus" );

  return 0;
}
