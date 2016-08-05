#include <stdio.h>
#include <string.h>

#include "regexp3.h"

void printCatch(){
  int i = 0, max = totCatch();

  while( ++i <= max )
    printf( "#%d# >%.*s<\n", i, lenCatch( i ), gpsCatch( i ) );
}

int raptor_test(){
  int result = 0;
  int errs   = 0;
  int total  = 0;

#define TRUE_TEST( text, re )                   \
  result = regexp3( text, re );                 \
  total++;                                      \
  if( result == 0 ){                            \
    puts( "Error on TRUE TEST " text ", " re ); \
    errs++;                                     \
  }

  TRUE_TEST( "Raptor Test", "Raptor" );
  TRUE_TEST( "Raptor Test", "Test" );
  TRUE_TEST( "Raptor Test", "r T" );
  TRUE_TEST( "Raptor Test", "#^Raptor" );
  TRUE_TEST( "Raptor Test", "#$Test" );
  TRUE_TEST( "Raptor Test", "#^$Raptor Test" );
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
  TRUE_TEST( "# Raptor Test", "#^\\#" );
  TRUE_TEST( " Raptor Test", "#^\\ " );
  TRUE_TEST( "Raptor Test", "\\R\\a\\p\\t\\o\\r\\ \\T\\es\\t" );

  TRUE_TEST( "Raptor Test",     "#^$<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>" );
  TRUE_TEST( "CaptorTest",       "#$<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>" );
  TRUE_TEST( "Cap CaptorTest",   "#^<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>" );
  TRUE_TEST( "Rap Captor Fest",    "<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>" );

  TRUE_TEST( "1999-12-05", "<(\\d){2,4}(\\-|/)(\\d)*(\\W{1})(\\d+)>" );
  TRUE_TEST( "<a href=\"https://es.wikipedia.org/wiki/Expresi%C3%B3n_regular\">", "(https?://)<[^\"]*>" );
  TRUE_TEST( "contacto (nasciiboy@gmail.com) $$", "<\\w+@\\w+\\.\\w+>" );
  TRUE_TEST( "1. rango entre 1985-2014.", "#$\\D?<\\d{4}>\\D?" );

  TRUE_TEST( "Raptor Test", "#^#!Raptos" );
  TRUE_TEST( "Raptor Test", "#^Raptor #!Fest" );
  TRUE_TEST( "Raptor Test", "#^#![^R]..... Test" );
  TRUE_TEST( "Raptor Test", "#^(R)..... Test" );
  TRUE_TEST( "Raptor Test", "#^$#!(C)..... Test" );
  TRUE_TEST( "Raptor Test", "#^$#!<C>..... Test" );
  TRUE_TEST( "Raptor Test", "#^$#!<a-z|\\d>..... Test" );
  TRUE_TEST( "Raptor Test", "#^$#!<a-z|S-Z|\\s>..... Test" );
  TRUE_TEST( "Raptor Test", "#^$<(R)>..... Test" );
  TRUE_TEST( "Raptor Test", "#^$#!<(C)>..... Test" );
  TRUE_TEST( " R aptor Test", "#^#!<( C )>" );
  TRUE_TEST( " R aptor Test", "#^#!<( (N|G|E).)>" );
  TRUE_TEST( "Raptor Test", "#^<#!(N|G|E)>" );
  TRUE_TEST( "Raptor Test", "#^<#!(N|G|E)>" );
  TRUE_TEST( "Raptor Test", "#^<#!((N )|(G|_)|E)>" );
  TRUE_TEST( "Raptor Test", "#^<#!R|#!(R|R)|R>" );

  TRUE_TEST( "<< text : text :: text", "#^\\s*<[\\<^_\\>]{2}>\\s+<#!( (\\<:|::|:\\>|\\>:\\<) )+> <\\<:|::|:\\>|\\>:\\<> <.*>" );
  // printCatch();
  TRUE_TEST( "<^ text : text <: text", "#^\\s*<[\\<^_\\>]{2}>\\s+<#!( (\\<:|::|:\\>|\\>:\\<) )+> <\\<:|::|:\\>|\\>:\\<> <.*>" );
  // printCatch();
  TRUE_TEST( "^^ text : text :> text", "#^\\s*<[\\<^_\\>]{2}>\\s+<#!( (\\<:|::|:\\>|\\>:\\<) )+> <\\<:|::|:\\>|\\>:\\<> <.*>" );
  // printCatch();
  TRUE_TEST( "_< text : text >:< text", "#^\\s*<[\\<^_\\>]{2}>\\s+<#!( (\\<:|::|:\\>|\\>:\\<) )+> <\\<:|::|:\\>|\\>:\\<> <.*>" );
  // printCatch();

#define FALSE_TEST( text, re )                          \
  result = regexp3( text, re );                         \
  total++;                                              \
  if( result ){                                         \
    puts( "Error on FALSE TEST " text ", " re );        \
    errs++;                                             \
  }

  FALSE_TEST( "", "" );
  FALSE_TEST( "Raptor Test", "" );
  FALSE_TEST( "", "Raptor Test" );
  FALSE_TEST( "Raptor Test", "#^" );
  FALSE_TEST( "Raptor Test", "#$" );
  FALSE_TEST( "Raptor Test", "|" );
  FALSE_TEST( "Raptor Test", "()" );
  FALSE_TEST( "Raptor Test", "|(|)" );
  FALSE_TEST( "Raptor Test", "Captor" );
  FALSE_TEST( "Raptor Test", "Fest" );
  FALSE_TEST( "Raptor Test", "e T" );
  FALSE_TEST( "Raptor Test", "#^Captor" );
  FALSE_TEST( "Raptor Test", "#$Fest" );
  FALSE_TEST( "Raptor Test", "#^$Raptor Test " );
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
  FALSE_TEST( " Raptor Test", "#^\\#" );
  FALSE_TEST( " Raptor Test", "#^\\a" );
  FALSE_TEST( "Raptor Fest", "\\R\\a\\p\\t\\o\\r\\ \\T\\es\\t" );

  FALSE_TEST( "Raptor Tes4",    "#$^<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>" );
  FALSE_TEST( "CaptorTes4",      "#$<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>" );
  FALSE_TEST( "Cap CaptorTes4",  "#^<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>" );
  FALSE_TEST( "Rap Captor Fes4",   "<((C|R)ap C|C|R)(a+p{1}tor)\\s?((\\Se)(st))>" );

  FALSE_TEST( "1999+12-05", "<(\\d){2,4}(\\-|/)(\\d)*(\\W{1})(\\d+)>" );
  FALSE_TEST( "<a href=\"https@//es.wikipedia.org/wiki/Expresi%C3%B3n_regular\">", "(https?://)<[^\"]*>" );
  FALSE_TEST( "contacto (nasciiboy^gmail.com) $$", "<\\w+@\\w+\\.\\w+>" );
  FALSE_TEST( "1. rango entre 1985-201A.", "#$\\D?(\\d{4})\\D?" );

  FALSE_TEST( "Raptor Test", "#^#!Raptor" );
  FALSE_TEST( "Raptor Test", "#^#![R]aptor" );
  FALSE_TEST( "Raptor Test", "#^Raptor #!Test" );

#define NTEST( text, re, n )                                    \
  result = regexp3( text, re );                                 \
  total++;                                                      \
  if( result != n ){                                            \
    printf( "Error on NTEST " text ", " re " %d\n", n );        \
    printf( "results %d\n", result );                           \
    errs++;                                                     \
  }

  NTEST( "Raptor Test", "#^", 0  );
  NTEST( "Raptor Test", "", 0 );
  NTEST( "Raptor Test", "|", 0 );
  NTEST( "Raptor Test", "()", 0 );
  NTEST( "Raptor Test", "|(|)", 0 );
  NTEST( "Raptor Test", "r T", 1 );
  NTEST( "Raptor Test", "#^Raptor", 1 );
  NTEST( "Raptor Test", "#$Test", 1 );
  NTEST( "Raptor Test", "#^Raptor Test", 1 );
  NTEST( "Raptor Test", "Raptor|Test |Dinosaur", 1 );
  NTEST( "Raptor Test Fest", "Raptor|Test|Fest", 3 );
  NTEST( "Raptor Test", "."  , 11 );
  NTEST( "Raptor Test", "\\w", 10 );
  NTEST( "Raptor Test", "\\s", 1  );
  NTEST( "Raptor Test", "[t]", 2  );
  NTEST( "Raptor Test", "[^t]", 9  );
  NTEST( "Raptor Test", "a|e|i|o|u", 3  );
  NTEST( "Raptor Test", "[aeiou]", 3  );
  NTEST( "Raptor Test", "##![r]", 10 );
  NTEST( "Raptor Test", "[^r]", 10 );
  NTEST( "Raptor Test", "##![^t]", 2 );
  NTEST( "123456789", ".", 9 );
  NTEST( "△23△567▲△", ".", 9 );
  NTEST( "123456789", "#^.", 1 );
  NTEST( "123456789", "#?.", 1 );
  NTEST( "123456789", "#^7", 0 );
  NTEST( "123456789", "#?7", 1 );
  NTEST( "123456789", "#^?1", 1 );
  NTEST( "△23△567▲△", "\\W", 4 );
  NTEST( "▲△▲△▲△▲△▲", ".", 9 );
  NTEST( "▲△▲△▲△▲△▲", "\\W", 9 );
  NTEST( "1a2a3a4a5a6a", "a", 6 );
  NTEST( "1a2a3a4a5a6a", "1-6a", 6 );
  NTEST( "1a2a3a4a5a6a", "1-3a", 3 );
  NTEST( "ae ai au aw aa", "a", 6 );
  NTEST( "ae ai au aw aa", "#$a", 1 );
  NTEST( "ae ai au aw aa", "#$ aa", 1 );
  NTEST( "ae ai au aw aa", "(a[^a])", 4 );
  NTEST( "ae ai au aw aa", "(a[^a])+", 4 );
  NTEST( "ae ai au aw aa", "(a[^a])*", 10 );
  NTEST( "ae ai au aw aa", "(a[^a])?", 10 );
  NTEST( "ae ai au aw aa", "(a[ei])?", 12 );
  NTEST( "ae ai au aw aa", "(a[ei])", 2 );
  NTEST( "ae ai au aw aa", "(a[ei] ?){2}", 1 );
  NTEST( "ae ai au aw aa", "(ae|(ai|au))", 3 );
  NTEST( "ae ai au aw aa", "<i a|u a>", 2 );
  NTEST( "ae ai au aw aa", "(i a|u a)*", 9 );
  NTEST( "ae ai au aw aa", "(i a(u a)+)*", 9 );
  NTEST( "ae ai au aw aa", "(i a(u a)+)+", 1 );
  NTEST( "Raptor Test Fest", "(Raptor (Test (Fest)))", 1 );
  NTEST( "Raptor Test Fest", "(((Raptor) Test) Fest)", 1 );
  NTEST( "Raptor Raptors Raptoring", "(Raptor(\\w+)?) ?", 3 );
  NTEST( "@@b(bold)@e<emphasis>", "<@{1,2}[^@\x01-\x20\\&\\{\\(\\<\\[]+[\\{\\(\\<\\[]>", 2 );
  NTEST( "@@b(bold)@e<emphasis>", "#?<@{1,2}[^@\x01-\x20\\&\\{\\(\\<\\[]+[\\{\\(\\<\\[]>", 1 );

  char line[1024];
#define CATCH_TEST( text, re, n, rtext )                \
  result = regexp3( text, re );                         \
  total++;                                              \
  if( strcmp( cpyCatch( line, n ), rtext ) != 0 ){      \
    printf( "Error on " text ", " re "\n" );            \
    printf( "result    >>%s<<\n"                        \
            "expected  >>%s<<\n", line, rtext );        \
    errs++;                                             \
  }

  CATCH_TEST( "Raptor Test", "#^#!<(r )>+", 1, "Rapto" );
  CATCH_TEST( "Raptor Test", "#^#!<r |r >+", 1, "Rapto" );
  CATCH_TEST( "Raptor Test", "#^<#!((r ))+>", 1, "Rapto" );
  CATCH_TEST( "Raptor Test", "#^<#!(r )+>", 1, "Rapto" );
  CATCH_TEST( "Raptor Test", "#^#!<r >+", 1, "Rapto" );
  CATCH_TEST( "Raptor Test", "#^<#!(r |r )>+", 1, "Rapto" );

#define REPLACE_TEST( text, re, n, rtext, ctext )               \
  result = regexp3( text, re );                                 \
  total++;                                                      \
  if( strcmp( rplCatch( line, rtext, n ), ctext ) != 0 ){       \
    printf( "Error on " text ", " re "\n" );                    \
    printf( "result    >>%s<<\n"                                \
            "expected  >>%s<<\n", line, ctext );                \
    errs++;                                                     \
  }

  REPLACE_TEST( "Raptor Test", "<Raptor>", 1, "Captor", "Captor Test"   );
  REPLACE_TEST( "Raptor Test", "<Raptor>", 0, "Captor", "Raptor Test"   );
  REPLACE_TEST( "Raptor Test", "<Raptor|Test>", 0, "Captor", "Raptor Test"   );
  REPLACE_TEST( "Raptor Test", "<Raptor|Test>", 1, "Captor", "Captor Captor"   );
  REPLACE_TEST( "Raptor Test", "<Raptor|Test>", 2, "Captor", "Raptor Test"   );
  REPLACE_TEST( "Raptor Test", "<Raptor|<Test>>", 2, "Fest", "Raptor Fest"   );
  REPLACE_TEST( "Raptor Raptors Raptoring", "<Raptor\\w*>", 1, "Test", "Test Test Test" );
  REPLACE_TEST( "Raptor Raptors Raptoring", "<Raptor>\\w*", 1, "Test", "Test Tests Testing" );
  REPLACE_TEST( "Raptor Raptors Raptoring", "<<<R>a>ptor>\\w*", 3, "C", "Captor Captors Captoring" );
  REPLACE_TEST( "Raptor Raptors Raptoring", "<<<R>a>ptor>\\w*", 2, "4", "4ptor 4ptors 4ptoring" );

#define MOD_TEST( text, re, rtext, ctext )              \
  result = regexp3( text, re );                         \
  total++;                                              \
  if( strcmp( modCatch( line, rtext ), ctext ) != 0 ){  \
    printf( "Error on " text ", " rtext "\n" );         \
    printf( "result    >>%s<<\n"                        \
            "expected  >>%s<<\n", line, ctext );        \
    errs++;                                             \
  }

  MOD_TEST( "Raptor Test", "<aptor|est>", "C\\1 F\\2", "Captor Fest" );
  MOD_TEST( "Raptor Test", "<aptor|est>", "C\\5 F\\2", "C Fest" );
  MOD_TEST( "Raptor Test", "<aptor|est>", "C\\a F\\2", "C Fest" );
  MOD_TEST( "Raptor Test", "<aptor|est>", "C\\0 F\\2", "C Fest" );
  MOD_TEST( "Raptor Test", "<aptor|est>", "C\\43 F\\43", "C F" );
  MOD_TEST( "Raptor Test", "<aptor|est>", "C\\\\43 \\\\F\\43\\\\", "C\\43 \\F\\" );
  MOD_TEST( "Raptor Test", "<aptor|est>", "C\\\\43 \\\\1\\\\2", "C\\43 \\1\\2" );
  MOD_TEST( "Raptor Test", "<aptor|est>", "\\\\Raptor \\\\Test", "\\Raptor \\Test" );
  MOD_TEST( "Raptor Test Fest", "<Raptor> <Test>", "\\1_\\2", "Raptor_Test" );
  MOD_TEST( "1995-12/65", "<\\d{4}>(\\-|/)<\\d{2}>(\\-|/)<\\d{2}>", "\\1::\\2::\\3", "1995::12::65" );
  MOD_TEST( "1995-12/65", "<\\d{4}>(\\-|/)<\\d{2}>(\\-|/)<\\d{2}>", "\\1::\\2::\\Raptor", "1995::12::aptor" );
  MOD_TEST( "make a new Line", "#^<[^n]+>", "\\1raptor", "make a raptor" );

  printf( "TEST %d *** ERRS %d\n\n", total, errs );
  return !errs;
}

int main(){
  if( raptor_test() )
    puts( "i am the raptor" );

  return 0;
}
