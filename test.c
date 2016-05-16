#include <stdio.h>
#include <string.h>

#include "regexp3.h"

void printCatch(){
  char lineCatch[128];
  int i = 0, max = totCatch();

  while( ++i <= max )
    printf( "#%d# >%s<\n", i, cpyCatch( lineCatch, i ) );
}

int rtest(){
  int result = 0;
  int errs   = 0;
  int total  = 0;

#define TRUE_TEST( str, exp )                   \
  result = regexp3( str, exp );                 \
  total++;                                      \
  if( result == 0 ){                            \
    puts( "Error on TRUE TEST " str ", " exp ); \
    errs++;                                     \
  }

  TRUE_TEST( "Raptor Test", "Raptor" );
  TRUE_TEST( "Raptor Test", "Test" );
  TRUE_TEST( "Raptor Test", "r T" );
  TRUE_TEST( "Raptor Test", "^Raptor" );
  TRUE_TEST( "Raptor Test", "Test$" );
  TRUE_TEST( "Raptor Test", "^Raptor Test$" );
  TRUE_TEST( "Raptor Test", "Dinosaur|T Rex|Raptor|Triceratops" );
  TRUE_TEST( "Raptor Test", "Ra-zptor" );
  TRUE_TEST( "Raptor Test", "R[a-z]ptor" );
  TRUE_TEST( "Raptor Test", "R[uoiea]ptor" );
  TRUE_TEST( "Raptor Test", "R[^uoie]ptor" );
  TRUE_TEST( "Raptor Test", "R[^1-9b-z]ptor" );
  TRUE_TEST( "Raptor Test", "R.ptor" );
  TRUE_TEST( "Raptor Test", "Ra?ptor" );
  TRUE_TEST( "Rptor Test", "Ra?ptor" );
  TRUE_TEST( "Raptor Test", "Ra+ptor" );
  TRUE_TEST( "Raaaptor Test", "Ra+ptor" );
  TRUE_TEST( "Raptor Test", "Ra*ptor" );
  TRUE_TEST( "Rptor Test", "Ra*ptor" );
  TRUE_TEST( "Raaaptor Test", "Ra*ptor" );
  TRUE_TEST( "Raptor Test", "Ra{1}ptor" );
  TRUE_TEST( "Raaaptor Test", "Ra{0,100}ptor" );
  TRUE_TEST( "Rptor Test", "Ra{0}ptor" );
  TRUE_TEST( "Raptor Test", "R\141ptor" );
  TRUE_TEST( "Raaaptor Test", "R\141{3}ptor" );
  TRUE_TEST( "Raptor Test", "R\x61ptor" );
  TRUE_TEST( "Raaaptor Test", "R\x61{3}ptor" );
  TRUE_TEST( "Ráptor Test", "Ráptor" );
  TRUE_TEST( "Rááptor Test", "R.áptor" );
  TRUE_TEST( "Rááptor Test", "Rá{2}ptor" );
  TRUE_TEST( "Rááptor Test", "R(á){2}ptor" );
  TRUE_TEST( "R△ptor Test", "R\u25B3ptor" );
  TRUE_TEST( "R△ptor Test", "R\\Wptor" );
  TRUE_TEST( "R△ptor Test", "R\\Sptor" );
  TRUE_TEST( "R△ptor Test", "R\\Dptor" );
  TRUE_TEST( "R△△△ptor Test", "R\u25B3+ptor" );
  TRUE_TEST( "R△△△ptor Test", "R\u25B3{3}ptor" );
  TRUE_TEST( "R△△△ptor Test", "R△{3}ptor" );
  TRUE_TEST( "R△△△ptor Test", "R[^a]{3}ptor" );
  TRUE_TEST( "R△△ptor Test", "R.\u25B3ptor" );
  TRUE_TEST( "R△△ptor Test", "R(\u25B3){2}ptor" );
  TRUE_TEST( "R▲△ptor Test", "R[△▲]{2}ptor" );
  TRUE_TEST( "R▲△ptor Test", "R[^ae]{2}ptor" );
  TRUE_TEST( "R▲△ptor Test", "R.{2}ptor" );
  TRUE_TEST( "R▲△ptor Test", "R[.]{2}ptor" );
  TRUE_TEST( "Raptor Test", "R\\wptor" );
  TRUE_TEST( "R.ptor Test", "R\\Wptor" );
  TRUE_TEST( "Rapt0r Test", "Rapt\\dr" );
  TRUE_TEST( "Raptor Test", "Rapt\\Dr" );
  TRUE_TEST( "Raptor Test", "Raptor\\s" );
  TRUE_TEST( "Rapt0r Test", "Rapt\\Sr" );
  TRUE_TEST( "Raptor Test", "(Raptor)" );
  TRUE_TEST( "Raptor Test", "(Test)" );
  TRUE_TEST( "Raptor Test", "(Test|Raptor)" );
  TRUE_TEST( "Raptor Test", "<Raptor>" );
  TRUE_TEST( "Raptor Test", "<Test>" );
  TRUE_TEST( "Raptor Test", "<Test|Raptor>" );
  TRUE_TEST( "Raptor Test", "(Ra(p(t(or))))" );
  TRUE_TEST( "Raptor Test", "(((R)a)p)tor" );
  TRUE_TEST( "Raptor Test", "Rap((t(o)r))" );
  TRUE_TEST( "Raptor Test", "(((R)a)p)(t((o)r))" );
  TRUE_TEST( "Raptor Test", "((((R)a)p)(t((o)r)))" );
  TRUE_TEST( "Raptor Test", "<Ra<p<t<or>>>>" );
  TRUE_TEST( "Raptor Test", "<<<R>a>p>tor" );
  TRUE_TEST( "Raptor Test", "Rap<<t<o>r>>" );
  TRUE_TEST( "Raptor Test", "<<<R>a>p><t<<o>r>>" );
  TRUE_TEST( "Raptor Test", "<<<<R>a>p><t<<o>r>>>" );

  TRUE_TEST( "$|)}^", "[$|)}^]{5}" );
  TRUE_TEST( "|()<>[]?+*{}-\\", "\\|\\(\\)\\<\\>\\[\\]\\?\\+\\*\\{\\}\\-\\\\" );
  TRUE_TEST( "Raptor Test", "\\R\\a\\p\\t\\o\\r\\ \\T\\es\\t" );

  TRUE_TEST( "Raptor Test",     "^<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>$" );
  TRUE_TEST( "CaptorTest",       "<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>$" );
  TRUE_TEST( "Cap CaptorTest",  "^<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>" );
  TRUE_TEST( "Rap Captor Fest",  "<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>" );

  TRUE_TEST( "1999-12-05", "<(\\d){2,4}(\\-|/)(\\d)*(\\W{1})(\\d+)>" );
  TRUE_TEST( "<a href=\"https://es.wikipedia.org/wiki/Expresi%C3%B3n_regular\">", "(https?://)<[^\"]*>" );
  TRUE_TEST( "contacto (nasciiboy@gmail.com) $$", "<\\w+@\\w+\\.\\w+>" );
  TRUE_TEST( "1. rango entre 1985-2014.", "\\D?<\\d{4}>\\D?$" );


#define FALSE_TEST( str, exp )                          \
  result = regexp3( str, exp );                         \
  total++;                                              \
  if( result ){                                         \
    puts( "Error on FALSE TEST " str ", " exp );        \
    errs++;                                             \
  }

  FALSE_TEST( "", "" );
  FALSE_TEST( "Raptor Test", "" );
  FALSE_TEST( "Raptor Test", "^" );
  FALSE_TEST( "Raptor Test", "$" );
  FALSE_TEST( "Raptor Test", "|" );
  FALSE_TEST( "Raptor Test", "()" );
  FALSE_TEST( "Raptor Test", "|(|)" );
  FALSE_TEST( "", "Raptor Test" );
  FALSE_TEST( "Raptor Test", "Captor" );
  FALSE_TEST( "Raptor Test", "Fest" );
  FALSE_TEST( "Raptor Test", "e T" );
  FALSE_TEST( "Raptor Test", "^Captor" );
  FALSE_TEST( "Raptor Test", "Fest$" );
  FALSE_TEST( "Raptor Test", "^Raptor Test $" );
  FALSE_TEST( "Raptor Test", "Dinosaur|T Rex|Captor|Triceratops" );
  FALSE_TEST( "Raptor Test", "Rb-zptor" );
  FALSE_TEST( "Raptor Test", "R[b-z]ptor" );
  FALSE_TEST( "Raptor Test", "R[uoiex]ptor" );
  FALSE_TEST( "Raptor Test", "R[^uoiea]ptor" );
  FALSE_TEST( "Raptor Test", "R[^1-9a-z]ptor" );
  FALSE_TEST( "Raptor Test", "Ra.ptor" );
  FALSE_TEST( "Raptor Test", "Rx?ptor" );
  FALSE_TEST( "Raptor Test", "Rx+ptor" );
  FALSE_TEST( "Raaaptor Test", "Rx+ptor" );
  FALSE_TEST( "Raptor Test", "Rx*ptor" );
  FALSE_TEST( "Raaaptor Test", "Rx*ptor" );
  FALSE_TEST( "Raptor Test", "Rx{1}ptor" );
  FALSE_TEST( "Raaaptor Test", "Rx{0,100}ptor" );
  FALSE_TEST( "R4ptor Test", "R\141ptor" );
  FALSE_TEST( "R123ptor Test", "R\141{3}ptor" );
  FALSE_TEST( "R4ptor Test", "R\x61ptor" );
  FALSE_TEST( "R123ptor Test", "R\x61{3}ptor" );
  FALSE_TEST( "R4ptor Test", "Ráptor" );
  FALSE_TEST( "R44ptor Test", "R(á){2}ptor" );
  FALSE_TEST( "R4ptor Test", "R\u25B3ptor" );
  FALSE_TEST( "R△ptor Test", "R\\wptor" );
  FALSE_TEST( "R△ptor Test", "R\\sptor" );
  FALSE_TEST( "R△ptor Test", "R\\dptor" );
  FALSE_TEST( "R44ptor Test", "R(\u25B3){2}ptor" );
  FALSE_TEST( "Ráaptor Test", "R.áptor" );
  FALSE_TEST( "Ráaptor Test", "Rá{2}ptor" );
  FALSE_TEST( "R△△aptor Test", "R\u25B3{3}ptor" );
  FALSE_TEST( "R△△▲ptor Test", "R△{3}ptor" );
  FALSE_TEST( "R△a△ptor Test", "R[^a]{3}ptor" );
  FALSE_TEST( "R△▲ptor Test", "R.\u25B3ptor" );
  FALSE_TEST( "R△▲ptor Test", "R\u25B3{2}ptor" );
  FALSE_TEST( "R▲△ptor Test", "R[^△▲]{2}ptor" );
  FALSE_TEST( "R▲△ptor Test", "R[ae]{2}ptor" );
  FALSE_TEST( "R▲△ptor Test", "R.{1}ptor" );
  FALSE_TEST( "R▲△ptor Test", "R[.]{1}ptor" );
  FALSE_TEST( "R^ptor Test", "R\\wptor" );
  FALSE_TEST( "Raptor Test", "R\\Wptor" );
  FALSE_TEST( "Raptor Test", "Rapt\\dr" );
  FALSE_TEST( "Rapt0r Test", "Rapt\\Dr" );
  FALSE_TEST( "Raptor_Test", "Raptor\\s" );
  FALSE_TEST( "Rapt r Test", "Rapt\\Sr" );
  FALSE_TEST( "Raptor Test", "(Captor)" );
  FALSE_TEST( "Raptor Test", "(Fest)" );
  FALSE_TEST( "Raptor Test", "(Fest|Captor)" );
  FALSE_TEST( "Raptor Test", "<Captor>" );
  FALSE_TEST( "Raptor Test", "<Fest>" );
  FALSE_TEST( "Raptor Test", "<Fest|Captor>" );
  FALSE_TEST( "Raptor Test", "(Ra(p(t(0r))))" );
  FALSE_TEST( "Raptor Test", "(((R)a)p)t0r" );
  FALSE_TEST( "Raptor Test", "Rap((t(0)r))" );
  FALSE_TEST( "Raptor Test", "(((R)a)p)(t((0)r))" );
  FALSE_TEST( "Raptor Test", "((((R)a)p)(t((0)r)))" );
  FALSE_TEST( "Raptor Test", "<Ra<p<t<0r>>>>" );
  FALSE_TEST( "Raptor Test", "<<<R>a>p>t0r" );
  FALSE_TEST( "Raptor Test", "Rap<<t<0>r>>" );
  FALSE_TEST( "Raptor Test", "<<<R>a>p><t<<0>r>>" );
  FALSE_TEST( "Raptor Test", "<<<<R>a>p><t<<0>r>>>" );

  FALSE_TEST( "4|)}^", "[$|)}^]{5}" );
  FALSE_TEST( "|()<>[]?+*{}4\\", "\\|\\(\\)\\<\\>\\[\\]\\?\\+\\*\\{\\}\\-\\\\" );
  FALSE_TEST( "Raptor Fest", "\\R\\a\\p\\t\\o\\r\\ \\T\\es\\t" );

  FALSE_TEST( "Raptor Tes4",     "^<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>$" );
  FALSE_TEST( "CaptorTes4",       "<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>$" );
  FALSE_TEST( "Cap CaptorTes4",  "^<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>" );
  FALSE_TEST( "Rap Captor Fes4",  "<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>" );

  FALSE_TEST( "1999+12-05", "<(\\d){2,4}(\\-|/)(\\d)*(\\W{1})(\\d+)>" );
  FALSE_TEST( "<a href=\"https@//es.wikipedia.org/wiki/Expresi%C3%B3n_regular\">", "(https?://)<[^\"]*>" );
  FALSE_TEST( "contacto (nasciiboy^gmail.com) $$", "<\\w+@\\w+\\.\\w+>" );
  FALSE_TEST( "1. rango entre 1985-201A.", "\\D?(\\d{4})\\D?$" );

#define NTEST( str, exp, n )                                    \
  result = regexp3( str, exp );                                 \
  total++;                                                      \
  if( result != n ){                                            \
    printf( "Error on NTEST " str ", " exp " %d\n", n );        \
    printf( "results %d\n", result );                           \
    errs++;                                                     \
  }

  NTEST( "Raptor Test", "^", 0  );
  NTEST( "Raptor Test", "", 0 );
  NTEST( "Raptor Test", "|", 0 );
  NTEST( "Raptor Test", "()", 0 );
  NTEST( "Raptor Test", "|(|)", 0 );
  NTEST( "Raptor Test", "r T", 1 );
  NTEST( "Raptor Test", "^Raptor", 1 );
  NTEST( "Raptor Test", "Test$", 1 );
  NTEST( "Raptor Test", "^Raptor Test", 1 );
  NTEST( "Raptor Test", "Raptor|Test |Dinosaur", 1 );
  NTEST( "Raptor Test Fest", "Raptor|Test|Fest", 3 );
  NTEST( "123456789", ".", 9 );
  NTEST( "△23△567▲△", ".", 9 );
  NTEST( "△23△567▲△", "\\W", 4 );
  NTEST( "▲△▲△▲△▲△▲", ".", 9 );
  NTEST( "▲△▲△▲△▲△▲", "\\W", 9 );
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
  NTEST( "Raptor Test Fest", "(Raptor (Test (Fest)))", 1 );
  NTEST( "Raptor Test Fest", "(((Raptor) Test) Fest)", 1 );
  NTEST( "Raptor Raptors Raptoring", "(Raptor(\\w+)?) ?", 3 );

  char line[1024];
#define CATCH_AND_REPLACE_TEST( str, exp, n, rstr, cstr )       \
  result = regexp3( str, exp );                                 \
  total++;                                                      \
  if( strcmp( replaceCatch( line, rstr, n ), cstr ) != 0 ){     \
    printf( "Error on " str ", " exp "\n" );                    \
    printf( "result    >>%s<<\n"                                \
            "expected  >>%s<<\n", line, cstr );                 \
    errs++;                                                     \
  }

  CATCH_AND_REPLACE_TEST( "Raptor Test", "<Raptor>", 1, "Captor", "Captor Test"   );
  CATCH_AND_REPLACE_TEST( "Raptor Test", "<Raptor>", 0, "Captor", "Raptor Test"   );
  CATCH_AND_REPLACE_TEST( "Raptor Test", "<Raptor|Test>", 0, "Captor", "Raptor Test"   );
  CATCH_AND_REPLACE_TEST( "Raptor Test", "<Raptor|Test>", 1, "Captor", "Captor Captor"   );
  CATCH_AND_REPLACE_TEST( "Raptor Test", "<Raptor|Test>", 2, "Captor", "Raptor Test"   );
  CATCH_AND_REPLACE_TEST( "Raptor Test", "<Raptor|<Test>>", 2, "Fest", "Raptor Fest"   );
  CATCH_AND_REPLACE_TEST( "Raptor Raptors Raptoring", "<Raptor\\w*>", 1, "Test", "Test Test Test" );
  CATCH_AND_REPLACE_TEST( "Raptor Raptors Raptoring", "<Raptor>\\w*", 1, "Test", "Test Tests Testing" );
  CATCH_AND_REPLACE_TEST( "Raptor Raptors Raptoring", "<<<R>a>ptor>\\w*", 3, "C", "Captor Captors Captoring" );
  CATCH_AND_REPLACE_TEST( "Raptor Raptors Raptoring", "<<<R>a>ptor>\\w*", 2, "4", "4ptor 4ptors 4ptoring" );

#define NEWLINE_TEST( str, exp, rstr, cstr )                    \
  result = regexp3( str, exp );                                 \
  total++;                                                      \
  if( strcmp( newLineCatch( line, rstr ), cstr ) != 0 ){        \
    printf( "Error on " str ", " rstr "\n" );                   \
    printf( "result    >>%s<<\n"                                \
            "expected  >>%s<<\n", line, cstr );                 \
    errs++;                                                     \
  }

  NEWLINE_TEST( "Raptor Test", "<aptor|est>", "C\\1 F\\2", "Captor Fest" );
  NEWLINE_TEST( "Raptor Test", "<aptor|est>", "C\\5 F\\2", "C Fest" );
  NEWLINE_TEST( "Raptor Test", "<aptor|est>", "C\\a F\\2", "C Fest" );
  NEWLINE_TEST( "Raptor Test", "<aptor|est>", "C\\0 F\\2", "C Fest" );
  NEWLINE_TEST( "Raptor Test", "<aptor|est>", "C\\43 F\\43", "C F" );
  NEWLINE_TEST( "Raptor Test", "<aptor|est>", "C\\\\43 \\\\F\\43\\\\", "C\\43 \\F\\" );
  NEWLINE_TEST( "Raptor Test", "<aptor|est>", "C\\\\43 \\\\1\\\\2", "C\\43 \\1\\2" );
  NEWLINE_TEST( "Raptor Test", "<aptor|est>", "\\\\Raptor \\\\Test", "\\Raptor \\Test" );
  NEWLINE_TEST( "Raptor Test Fest", "<Raptor> <Test>", "\\1_\\2", "Raptor_Test" );
  NEWLINE_TEST( "1995-12/65", "<\\d{4}>(\\-|/)<\\d{2}>(\\-|/)<\\d{2}>", "\\1::\\2::\\3", "1995::12::65" );
  NEWLINE_TEST( "1995-12/65", "<\\d{4}>(\\-|/)<\\d{2}>(\\-|/)<\\d{2}>", "\\1::\\2::\\Raptor", "1995::12::aptor" );
  NEWLINE_TEST( "make a new Line", "^<[^n]+>", "\\1raptor", "make a raptor" );


  printf( "TEST %d *** ERRS %d\n\n", total, errs );
  return !errs;
}

int main(){
  if( rtest() )
    puts( "i am the raptor" );

  return 0;
}
