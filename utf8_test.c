#include <stdio.h>
#include <string.h>

#include "regexp3.h"

void printCatch(){
  int i = 0, max = totCatch();

  while( ++i <= max )
    printf( "#%d# >%.*s<\n", i, lenCatch( i ), gpsCatch( i ) );
}

int raptorTest(){
  int result = 0;
  int errs   = 0;
  int total  = 0;

#define NTEST( text, re, n )                            \
  result = regexp3( text, re );                         \
  total++;                                              \
  if( result != n ){                                    \
    printf( "%s:%d Error on N-TEST >%s< >%s<\n",        \
            __FILE__, __LINE__, text, re );             \
    printf( "results %d - %d expected\n", result, n );  \
    errs++;                                             \
  }


  NTEST( "a", "▲", 0 );
  NTEST( "a", "▲?", 1 );
  NTEST( "a", "△?", 1 );
  NTEST( "a", "▲+", 0 );
  NTEST( "a", "▲*", 1 );
  NTEST( "a", "△*", 1 );
  NTEST( "▲", "▲", 1 );
  NTEST( "▲", "▲?", 1 );
  NTEST( "▲", "△?", 1 );
  NTEST( "▲", "▲+", 1 );
  NTEST( "▲", "▲*", 1 );
  NTEST( "▲", "△*", 1 );
  NTEST( "▲▲▲", "▲+", 1 );
  NTEST( "▲▲▲", "▲*", 1 );
  NTEST( "▲▲▲", "▲+", 1 );
  NTEST( "▲▲▲", "▲?", 3 );
  NTEST( "▲▲▲△", "▲+", 1 );
  NTEST( "▲▲▲△", "▲*", 2 );
  NTEST( "▲▲▲△", "▲?", 4 );
  NTEST( "▲▲▲△", "▲+△", 1 );
  NTEST( "▲▲▲△", "▲*△", 1 );
  NTEST( "▲▲▲△", "▲?△", 1 );
  NTEST( "▲▲▲△", "▲+△?", 1 );
  NTEST( "▲▲▲△", "▲*△?", 1 );
  NTEST( "▲▲▲△", "▲?△?", 3 );
  NTEST( "▲▲▲△", "▲+△+", 1 );
  NTEST( "▲▲▲△", "▲*△+", 1 );
  NTEST( "▲▲▲△", "▲?△+", 1 );
  NTEST( "▲▲▲△", "▲+△*", 1 );
  NTEST( "▲▲▲△", "▲*△*", 1 );
  NTEST( "▲▲▲△", "▲?△*", 3 );
  NTEST( "▲▲▲△▲▲▲", "▲+", 2 );
  NTEST( "▲▲▲△▲▲▲", "▲*", 3 );
  NTEST( "▲▲▲△▲▲▲", "▲*", 3 );
  NTEST( "▲▲▲△▲▲▲", "▲*", 3 );
  NTEST( "▲", "(▲)?", 1 );
  NTEST( "▲", "(△)?", 1 );
  NTEST( "▲", "(▲)+", 1 );
  NTEST( "▲", "(▲)*", 1 );
  NTEST( "▲", "(△)*", 1 );
  NTEST( "▲▲▲", "(▲)+", 1 );
  NTEST( "▲▲▲", "(▲)*", 1 );
  NTEST( "▲▲▲", "#~(▲)+", 3 );
  NTEST( "▲▲▲", "#~(▲)*", 3 );
  NTEST( "▲▲▲△", "#~(▲+)", 3 );
  NTEST( "▲▲▲△", "#~(▲*)", 4 );
  NTEST( "▲▲▲△", "#~(▲?)", 4 );
  NTEST( "▲▲▲△", "#~(▲+△)", 3 );
  NTEST( "▲▲▲△", "#~(▲*△)", 4 );
  NTEST( "▲▲▲△", "#~(▲?△)", 2 );
  NTEST( "▲▲▲△", "#~(▲+△?)", 3 );
  NTEST( "▲▲▲△", "#~(▲*△?)", 4 );
  NTEST( "▲▲▲△", "#~(▲?△?)", 4 );
  NTEST( "▲▲▲△", "#~(▲+△+)", 3 );
  NTEST( "▲▲▲△", "#~(▲*△+)", 4 );
  NTEST( "▲▲▲△", "#~(▲?△+)", 2 );
  NTEST( "▲▲▲△", "#~(▲+△*)", 3 );
  NTEST( "▲▲▲△", "#~(▲*△*)", 4 );
  NTEST( "▲▲▲△", "#~(▲?△*)", 4 );
  NTEST( "▲▲▲△▲▲▲", "#~(▲+)", 6 );
  NTEST( "▲▲▲△▲▲▲", "#~(▲*)", 7 );
  NTEST( "▲", "[▲]?", 1 );
  NTEST( "▲", "[△]?", 1 );
  NTEST( "▲", "[▲]+", 1 );
  NTEST( "▲", "[▲]*", 1 );
  NTEST( "▲", "[△]*", 1 );
  NTEST( "▲▲▲", "[▲]?", 3 );
  NTEST( "▲▲▲", "[▲]+", 1 );
  NTEST( "▲▲▲", "[▲]*", 1 );
  NTEST( "▲▲▲", "#~[▲]?", 3 );
  NTEST( "▲▲▲", "#~[▲]+", 3 );
  NTEST( "▲▲▲", "#~[▲]*", 3 );
  NTEST( "▲▲▲△", "#~[▲△]", 4 );
  NTEST( "▲▲▲△", "#~[▲△]?", 4 );
  NTEST( "▲▲▲△", "#~[▲△]+", 4 );
  NTEST( "▲▲▲△", "#~[▲△]*", 4 );
  NTEST( "▲▲▲△▲▲▲", "#~[▲△]", 7 );
  NTEST( "▲", ":&", 1 );
  NTEST( "▲", ":&?", 1 );
  NTEST( "▲", ":&+", 1 );
  NTEST( "▲", ":&*", 1 );
  NTEST( "▲▲▲", ":&?", 3 );
  NTEST( "▲▲▲", ":&+", 1 );
  NTEST( "▲▲▲", ":&*", 1 );
  NTEST( "▲▲▲", "#~:&?", 3 );
  NTEST( "▲▲▲", "#~:&+", 3 );
  NTEST( "▲▲▲", "#~:&*", 3 );
  NTEST( "▲▲▲△", "#~:&", 4 );
  NTEST( "▲▲▲△", "#~:&?", 4 );
  NTEST( "▲▲▲△", "#~:&+", 4 );
  NTEST( "▲▲▲△", "#~:&*", 4 );
  NTEST( "▲▲▲△▲▲▲", "#~:&", 7 );
  NTEST( "▲", ":w", 0 );
  NTEST( "▲", ":w?", 1 );
  NTEST( "▲", ":w+", 0 );
  NTEST( "▲", ":w*", 1 );
  NTEST( "▲▲▲", ":w?", 3 );
  NTEST( "▲▲▲", ":w+", 0 );
  NTEST( "▲▲▲", ":w*", 3 );
  NTEST( "▲▲▲", "#~:w?", 3 );
  NTEST( "▲▲▲", "#~:w+", 0 );
  NTEST( "▲▲▲", "#~:w*", 3 );
  NTEST( "▲▲▲△", "#~:w", 0 );
  NTEST( "▲▲▲△", "#~:w?", 4 );
  NTEST( "▲▲▲△", "#~:w+", 0 );
  NTEST( "▲▲▲△", "#~:w*", 4 );
  NTEST( "▲▲▲△▲▲▲", "#~:w", 0 );
  NTEST( "▲", ":W", 1 );
  NTEST( "▲", ":W?", 1 );
  NTEST( "▲", ":W+", 1 );
  NTEST( "▲", ":W*", 1 );
  NTEST( "▲▲▲", ":W?", 3 );
  NTEST( "▲▲▲", ":W+", 1 );
  NTEST( "▲▲▲", ":W*", 1 );
  NTEST( "▲▲▲", "#~:W?", 3 );
  NTEST( "▲▲▲", "#~:W+", 3 );
  NTEST( "▲▲▲", "#~:W*", 3 );
  NTEST( "▲▲▲△", "#~:W", 4 );
  NTEST( "▲▲▲△", "#~:W?", 4 );
  NTEST( "▲▲▲△", "#~:W+", 4 );
  NTEST( "▲▲▲△", "#~:W*", 4 );
  NTEST( "▲▲▲△▲▲▲", "#~:W", 7 );

  NTEST( "△▲3△567△9", ".", 9 );
  NTEST( "△▲3△567△9", "(.)", 9 );
  NTEST( "△▲3△567△9", "[.]", 0 );
  NTEST( "△▲3△567△9", "(.+)", 1 );
  NTEST( "△▲3△567△9", ":&", 4 );
  NTEST( "△▲3△567△9", ":w", 5 );
  NTEST( "△▲3△567△9", ":W", 4 );
  NTEST( "△▲3△567△9", ":d", 5 );
  NTEST( "△▲3△567△9", ":a", 0 );
  NTEST( "△▲3△567△9", "[3-9]", 5 );
  NTEST( "△▲3△567△9", "[^3-9]", 4 );
  NTEST( "△▲3△567△9", "[^a-z]", 9 );
  NTEST( "△▲3△567△9", "[^:d]", 4 );
  NTEST( "△▲3△567△9", "[^:D]", 5 );
  NTEST( "△▲3△567△9", "[^:w]", 4 );
  NTEST( "△▲3△567△9", "[^:W]", 5 );
  NTEST( "△▲3△567△9", "[^:&]", 5 );
  NTEST( "△▲3△567△9", "[^:a]", 9 );
  NTEST( "△▲3△567△9", "[^:A]", 0 );



  NTEST( "Rááptor Test", "R.áptor", 1 );
  NTEST( "Rááptor Test", "Rá{2}ptor", 1 );
  NTEST( "Rááptor Test", "R(á){2}ptor", 1 );
  NTEST( "R△△△ptor Test", "R△{3}ptor", 1 );
  NTEST( "R△△△ptor Test", "R[^a]{3}ptor", 1 );
  NTEST( "R▲△ptor Test", "R[△▲]{2}ptor", 1 );
  NTEST( "R▲△ptor Test", "R[^ae]{2}ptor", 1 );
  NTEST( "R▲△ptor Test", "R.{2}ptor", 1 );
  NTEST( "R▲△ptor Test", "R[:W]{2}ptor", 1 );
  NTEST( "R▲△ptor Test", "R[^:w]{2}ptor", 1 );


  char str[1024];
#define CATCH_TEST( text, re, n, rtext )                \
  result = regexp3( text, re );                         \
  total++;                                              \
  if( strcmp( cpyCatch( str, n ), rtext ) != 0 ){       \
    printf( "%s:%d Error on " text ", " re "\n",        \
            __FILE__, __LINE__ );                       \
    printf( "result    >>%s<<\n"                        \
            "expected  >>%s<<\n", str, rtext );         \
    errs++;                                             \
  }


#define REPLACE_TEST( text, re, n, rtext, ctext )               \
  result = regexp3( text, re );                                 \
  total++;                                                      \
  if( strcmp( rplCatch( str, rtext, n ), ctext ) != 0 ){        \
    printf( "%s:%d Error on " text ", " re "\n",                \
            __FILE__, __LINE__ );                               \
    printf( "result    >>%s<<\n"                                \
            "expected  >>%s<<\n", str, ctext );                 \
    errs++;                                                     \
  }

  CATCH_TEST( "▲", "<▲>", 1, "▲" );
  CATCH_TEST( "▲▲", "<▲▲>", 1, "▲▲" );
  CATCH_TEST( "▲ ▲ ▲", "<▲>", 3, "▲" );
  CATCH_TEST( "▲bcd", "<▲|b|c|d>", 1, "▲" );
  CATCH_TEST( "▲bcd", "<▲|b|c|d>", 2, "b" );
  CATCH_TEST( "▲bcd", "<▲|b|c|d>", 3, "c" );
  CATCH_TEST( "▲bcd", "<▲|b|c|d>", 4, "d" );
  CATCH_TEST( "▲bcd", "<▲|b|c|d>", 5, "" );
  CATCH_TEST( "▲bc", "▲<x>", 1, "" );
  CATCH_TEST( "▲bc", "<▲>x", 1, "" );
  CATCH_TEST( "▲bc", "<▲|b>x", 1, "" );
  CATCH_TEST( "▲bc", "<<▲|b>x|▲bc>", 1, "▲bc" );
  CATCH_TEST( "▲bc", "<<▲|b>x|▲bc>", 2, "" );
  CATCH_TEST( "▲bc", "<x<▲|b>|▲bc>", 1, "▲bc" );
  CATCH_TEST( "▲bc", "<x<▲|b>|▲bc>", 2, "" );
  CATCH_TEST( "▲bc ▲bc ▲bc", "<▲|b|c>", 9, "c" );
  CATCH_TEST( "▲bc ▲bc ▲bc", "<(▲|b|c)(▲|b|c)(▲|b|c)>", 3, "▲bc" );
  CATCH_TEST( "▲bc ▲bc ▲bc", "<(▲|b|c)(▲|b|c)(▲|b|c)> ", 2, "▲bc" );
  CATCH_TEST( "▲bc ▲bc ▲bc", "#?<(▲|b|c)(▲|b|c)(▲|b|c)>", 1, "▲bc" );
  CATCH_TEST( "▲bc ▲bc ▲bc", "#?<(▲|b|c)(▲|b|c)((▲|b)|x)>", 1, "" );
  CATCH_TEST( "▲bc ▲bc ▲bx", "#?<(▲|b|c)(▲|b|c)((▲|b)|x)>", 1, "▲bx" );
  CATCH_TEST( "▲bc iecc oeb", "<<(▲|e)|(i|o)>e<b|c>|▲bc>", 1, "▲bc" );
  CATCH_TEST( "▲bc iecc oeb", "<<(▲|e)|(i|o)>e<b|c>|▲bc>", 2, "iec" );
  CATCH_TEST( "▲bc iecc oeb", "<<(▲|e)|(i|o)>e<b|c>|▲bc>", 3, "i" );
  CATCH_TEST( "▲bc iecc oeb", "<<(▲|e)|(i|o)>e<b|c>|▲bc>", 4, "c" );
  CATCH_TEST( "▲bc iecc oeb", "<<(▲|e)|(i|o)>e<b|c>|▲bc>", 5, "oeb" );
  CATCH_TEST( "▲bc iecc oeb", "<<(▲|e)|(i|o)>e<b|c>|▲bc>", 6, "o" );
  CATCH_TEST( "▲bc iecc oeb", "<<(▲|e)|(i|o)>e<b|c>|▲bc>", 7, "b" );
  CATCH_TEST( "▲bc iecc oeb", "<<(▲|e)|(i|o)>e<b|c>|▲bc>", 8, "" );

  CATCH_TEST( "▲", "<▲>?", 1, "▲" );
  CATCH_TEST( "▲", "<b>?", 1, "" );
  CATCH_TEST( "▲", "<▲>+", 1, "▲" );
  CATCH_TEST( "▲", "<▲>*", 1, "▲" );
  CATCH_TEST( "▲", "<b>*", 1, "" );
  CATCH_TEST( "▲▲▲", "<▲>+", 1, "▲▲▲" );
  CATCH_TEST( "▲▲▲", "<▲>*", 1, "▲▲▲" );
  CATCH_TEST( "▲▲▲", "#~<▲>+", 1, "▲▲▲" );
  CATCH_TEST( "▲▲▲", "#~<▲>*", 1, "▲▲▲" );
  CATCH_TEST( "▲▲▲b", "#~<▲+>", 1, "▲▲▲" );
  CATCH_TEST( "▲▲▲b", "#~<▲*>", 1, "▲▲▲" );
  CATCH_TEST( "▲▲▲b", "#~<▲?>", 4, "" );
  CATCH_TEST( "▲▲▲b", "#~<▲+b>", 1, "▲▲▲b" );
  CATCH_TEST( "▲▲▲b", "#~<▲*b>", 1, "▲▲▲b" );
  CATCH_TEST( "▲▲▲b", "#~<▲?b>", 1, "▲b" );
  CATCH_TEST( "▲▲▲b", "#~<▲+b?>", 1, "▲▲▲b" );
  CATCH_TEST( "▲▲▲b", "#~<▲*b?>", 1, "▲▲▲b" );
  CATCH_TEST( "▲▲▲b", "#~<▲?b?>", 3, "▲b" );
  CATCH_TEST( "▲▲▲b", "#~<▲+b+>", 1, "▲▲▲b" );
  CATCH_TEST( "▲▲▲b", "#~<▲*b+>", 1, "▲▲▲b" );
  CATCH_TEST( "▲▲▲b", "#~<▲?b+>", 1, "▲b" );
  CATCH_TEST( "▲▲▲b", "#~<▲+b*>", 1, "▲▲▲b" );
  CATCH_TEST( "▲▲▲b", "#~<▲*b*>", 1, "▲▲▲b" );
  CATCH_TEST( "▲▲▲b", "#~<▲?b*>", 3, "▲b" );
  CATCH_TEST( "▲▲▲b▲▲▲", "#~<▲+>", 4, "▲▲▲" );
  CATCH_TEST( "▲▲▲b▲▲▲", "#~<▲*>", 5, "▲▲▲" );

  CATCH_TEST( "R▲ptor Test",     "<((C|R)▲p C|C|R)(▲+p{1}tor) ?((<T|F>+e)(st))>", 1, "R▲ptor Test" );
  CATCH_TEST( "R▲ptor Test",     "<((C|R)▲p C|C|R)(▲+p{1}tor) ?((<T|F>+e)(st))>", 2, "T" );
  CATCH_TEST( "R▲▲▲▲ptor TFest", "<((C|R)▲p C|C|R)(▲+p{1}tor) ?((<T|F>+e)(st))>", 1, "R▲▲▲▲ptor TFest" );
  CATCH_TEST( "R▲▲▲▲ptor TFest", "<((C|R)▲p C|C|R)(▲+p{1}tor) ?((<T|F>+e)(st))>", 2, "TF" );
  CATCH_TEST( "C▲ptorTest",      "<((C|R)▲p C|C|R)(▲+p{1}tor) ?((<T|F>+e)(st))>", 1, "C▲ptorTest" );
  CATCH_TEST( "C▲p C▲ptorTest",  "<((C|R)▲p C|C|R)(▲+p{1}tor) ?((<T|F>+e)(st))>", 1, "C▲p C▲ptorTest" );
  CATCH_TEST( "C▲p C▲ptorTest",  "#~<((C|R)▲p C|C|R)(▲+p{1}tor) ?((<T|F>+e)(st))>", 3, "C▲ptorTest" );
  CATCH_TEST( "R▲p C▲ptor Fest", "<((C|R)▲p C|C|R)(▲+p{1}tor) ?((<T|F>+e)(st))>", 1, "R▲p C▲ptor Fest" );
  CATCH_TEST( "R▲p C▲ptor Fest", "#~<((C|R)▲p C|C|R)(▲+p{1}tor) ?((<T|F>+e)(st))>", 3, "C▲ptor Fest" );

#define REPLACE_TEST( text, re, n, rtext, ctext )               \
  result = regexp3( text, re );                                 \
  total++;                                                      \
  if( strcmp( rplCatch( str, rtext, n ), ctext ) != 0 ){        \
    printf( "%s:%d Error on " text ", " re "\n",                \
            __FILE__, __LINE__ );                               \
    printf( "result    >>%s<<\n"                                \
            "expected  >>%s<<\n", str, ctext );                 \
    errs++;                                                     \
  }

  REPLACE_TEST( "▲", "<▲>"   , 1, "", "" );
  REPLACE_TEST( "▲", "<▲>?"  , 1, "", "" );
  REPLACE_TEST( "▲", "<▲>+"  , 1, "", "" );
  REPLACE_TEST( "▲", "<▲>*"  , 1, "", "" );
  REPLACE_TEST( "▲", "<▲>{1}", 1, "", "" );
  REPLACE_TEST( "▲", "<▲?>"  , 1, "", "" );
  REPLACE_TEST( "▲", "<▲+>"  , 1, "", "" );
  REPLACE_TEST( "▲", "<▲*>"  , 1, "", "" );
  REPLACE_TEST( "▲", "<▲{1}>", 1, "", "" );

  REPLACE_TEST( "▲", "<▲>"   , 1, "e", "e" );
  REPLACE_TEST( "▲", "<▲>?"  , 1, "e", "e" );
  REPLACE_TEST( "▲", "<▲>+"  , 1, "e", "e" );
  REPLACE_TEST( "▲", "<▲>*"  , 1, "e", "e" );
  REPLACE_TEST( "▲", "<▲>{1}", 1, "e", "e" );
  REPLACE_TEST( "▲", "<▲?>"  , 1, "e", "e" );
  REPLACE_TEST( "▲", "<▲+>"  , 1, "e", "e" );
  REPLACE_TEST( "▲", "<▲*>"  , 1, "e", "e" );
  REPLACE_TEST( "▲", "<▲{1}>", 1, "e", "e" );

  REPLACE_TEST( "▲", "<x>"   , 1, "z", "▲" );
  REPLACE_TEST( "▲", "<x>?"  , 1, "z", "z▲" );
  REPLACE_TEST( "▲", "<x>+"  , 1, "z", "▲" );
  REPLACE_TEST( "▲", "<x>*"  , 1, "z", "z▲" );
  REPLACE_TEST( "▲", "<x>{1}", 1, "z", "▲" );
  REPLACE_TEST( "▲", "<x?>"  , 1, "z", "z▲" );
  REPLACE_TEST( "▲", "<x+>"  , 1, "z", "▲" );
  REPLACE_TEST( "▲", "<x*>"  , 1, "z", "z▲" );
  REPLACE_TEST( "▲", "<x{1}>", 1, "z", "▲" );

  REPLACE_TEST( "▲▲▲", "<▲>"   , 1, "", "" );
  REPLACE_TEST( "▲▲▲", "<▲>?"  , 1, "", "" );
  REPLACE_TEST( "▲▲▲", "<▲>+"  , 1, "", "" );
  REPLACE_TEST( "▲▲▲", "<▲>*"  , 1, "", "" );
  REPLACE_TEST( "▲▲▲", "<▲>{1}", 1, "", "" );
  REPLACE_TEST( "▲▲▲", "<▲?>"  , 1, "", "" );
  REPLACE_TEST( "▲▲▲", "<▲+>"  , 1, "", "" );
  REPLACE_TEST( "▲▲▲", "<▲*>"  , 1, "", "" );
  REPLACE_TEST( "▲▲▲", "<▲{1}>", 1, "", "" );

  REPLACE_TEST( "▲▲▲", "<▲>"   , 1, "e", "eee" );
  REPLACE_TEST( "▲▲▲", "<▲>?"  , 1, "e", "eee" );
  REPLACE_TEST( "▲▲▲", "<▲>+"  , 1, "e", "e" );
  REPLACE_TEST( "▲▲▲", "<▲>*"  , 1, "e", "e" );
  REPLACE_TEST( "▲▲▲", "<▲>{1}", 1, "e", "eee" );
  REPLACE_TEST( "▲▲▲", "<▲?>"  , 1, "e", "eee" );
  REPLACE_TEST( "▲▲▲", "<▲+>"  , 1, "e", "e" );
  REPLACE_TEST( "▲▲▲", "<▲*>"  , 1, "e", "e" );
  REPLACE_TEST( "▲▲▲", "<▲{1}>", 1, "e", "eee" );

  REPLACE_TEST( "▲▲▲", "<x>"   , 1, "z", "▲▲▲" );
  REPLACE_TEST( "▲▲▲", "<x>?"  , 1, "z", "z▲z▲z▲" );
  REPLACE_TEST( "▲▲▲", "<x>+"  , 1, "z", "▲▲▲" );
  REPLACE_TEST( "▲▲▲", "<x>*"  , 1, "z", "z▲z▲z▲" );
  REPLACE_TEST( "▲▲▲", "<x>{1}", 1, "z", "▲▲▲" );
  REPLACE_TEST( "▲▲▲", "<x?>"  , 1, "z", "z▲z▲z▲" );
  REPLACE_TEST( "▲▲▲", "<x+>"  , 1, "z", "▲▲▲" );
  REPLACE_TEST( "▲▲▲", "<x*>"  , 1, "z", "z▲z▲z▲" );
  REPLACE_TEST( "▲▲▲", "<x{1}>", 1, "z", "▲▲▲" );

  REPLACE_TEST( "▲▲▲b", "<▲>"   , 1, "e", "eeeb" );
  REPLACE_TEST( "▲▲▲b", "<▲>?"  , 1, "e", "eeeeb" );
  REPLACE_TEST( "▲▲▲b", "<▲>+"  , 1, "e", "eb" );
  REPLACE_TEST( "▲▲▲b", "<▲>*"  , 1, "e", "eeb" );
  REPLACE_TEST( "▲▲▲b", "<▲>{1}", 1, "e", "eeeb" );
  REPLACE_TEST( "▲▲▲b", "<▲?>"  , 1, "e", "eeeeb" );
  REPLACE_TEST( "▲▲▲b", "<▲+>"  , 1, "e", "eb" );
  REPLACE_TEST( "▲▲▲b", "<▲*>"  , 1, "e", "eeb" );
  REPLACE_TEST( "▲▲▲b", "<▲{1}>", 1, "e", "eeeb" );

  REPLACE_TEST( "▲▲▲b", "<x>"   , 1, "z", "▲▲▲b" );
  REPLACE_TEST( "▲▲▲b", "<x>?"  , 1, "z", "z▲z▲z▲zb" );
  REPLACE_TEST( "▲▲▲b", "<x>+"  , 1, "z", "▲▲▲b" );
  REPLACE_TEST( "▲▲▲b", "<x>*"  , 1, "z", "z▲z▲z▲zb" );
  REPLACE_TEST( "▲▲▲b", "<x>{1}", 1, "z", "▲▲▲b" );
  REPLACE_TEST( "▲▲▲b", "<x?>"  , 1, "z", "z▲z▲z▲zb" );
  REPLACE_TEST( "▲▲▲b", "<x+>"  , 1, "z", "▲▲▲b" );
  REPLACE_TEST( "▲▲▲b", "<x*>"  , 1, "z", "z▲z▲z▲zb" );
  REPLACE_TEST( "▲▲▲b", "<x{1}>", 1, "z", "▲▲▲b" );

  REPLACE_TEST( "▲▲▲b▲▲▲", "<▲>"   , 1, "e", "eeebeee" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<▲>?"  , 1, "e", "eeeebeee" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<▲>+"  , 1, "e", "ebe" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<▲>*"  , 1, "e", "eebe" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<▲>{1}", 1, "e", "eeebeee" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<▲?>"  , 1, "e", "eeeebeee" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<▲+>"  , 1, "e", "ebe" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<▲*>"  , 1, "e", "eebe" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<▲{1}>", 1, "e", "eeebeee" );

  REPLACE_TEST( "▲▲▲b▲▲▲", "<x>"   , 1, "z", "▲▲▲b▲▲▲" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<x>?"  , 1, "z", "z▲z▲z▲zbz▲z▲z▲" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<x>+"  , 1, "z", "▲▲▲b▲▲▲" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<x>*"  , 1, "z", "z▲z▲z▲zbz▲z▲z▲" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<x>{1}", 1, "z", "▲▲▲b▲▲▲" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<x?>"  , 1, "z", "z▲z▲z▲zbz▲z▲z▲" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<x+>"  , 1, "z", "▲▲▲b▲▲▲" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<x*>"  , 1, "z", "z▲z▲z▲zbz▲z▲z▲" );
  REPLACE_TEST( "▲▲▲b▲▲▲", "<x{1}>", 1, "z", "▲▲▲b▲▲▲" );


  REPLACE_TEST( "R▲ptor Test", "<R▲ptor>", 1, "C▲ptor", "C▲ptor Test"   );
  REPLACE_TEST( "R▲ptor Test", "<R▲ptor>", 0, "C▲ptor", "R▲ptor Test"   );
  REPLACE_TEST( "R▲ptor Test", "<R▲ptor|Test>", 0, "C▲ptor", "R▲ptor Test"   );
  REPLACE_TEST( "R▲ptor Test", "<R▲ptor|Test>", 1, "C▲ptor", "C▲ptor C▲ptor"   );
  REPLACE_TEST( "R▲ptor Test", "<R▲ptor|Test>", 2, "C▲ptor", "R▲ptor Test"   );
  REPLACE_TEST( "R▲ptor Test", "<R▲ptor|<Test>>", 2, "Fest", "R▲ptor Fest"   );
  REPLACE_TEST( "R▲ptor R▲ptors R▲ptoring", "<R▲ptor:w*>", 1, "Test", "Test Test Test" );
  REPLACE_TEST( "R▲ptor R▲ptors R▲ptoring", "<R▲ptor>:w*", 1, "Test", "Test Tests Testing" );
  REPLACE_TEST( "R▲ptor R▲ptors R▲ptoring", "<<<R>▲>ptor>:w*", 3, "C", "C▲ptor C▲ptors C▲ptoring" );
  REPLACE_TEST( "R▲ptor R▲ptors R▲ptoring", "<<<R>▲>ptor>:w*", 2, "4", "4ptor 4ptors 4ptoring" );


#define PUT_TEST( text, re, rtext, ctext )              \
  result = regexp3( text, re );                         \
  total++;                                              \
  if( strcmp( putCatch( str, rtext ), ctext ) != 0 ){   \
    printf( "%s:%d Error on " text ", " re "\n",        \
            __FILE__, __LINE__ );                       \
    printf( "result    >>%s<<\n"                        \
            "expected  >>%s<<\n", str, ctext );         \
    errs++;                                             \
  }

  PUT_TEST( "▲", "<▲>", "#1", "▲" );
  PUT_TEST( "▲", "<▲>", "#x", "x" );
  PUT_TEST( "▲", "<▲>", "#xx", "xx" );
  PUT_TEST( "▲", "<▲>", "###1##", "#▲#" );
  PUT_TEST( "▲", "<▲>", "[#0][#1][#2#3#1000000]", "[][▲][]" );
  PUT_TEST( "▲▲", "<▲▲>", "#1", "▲▲" );
  PUT_TEST( "▲ ▲ ▲", "<▲>", "#1#2#3", "▲▲▲" );
  PUT_TEST( "▲bcd", "<▲|b|c|d>", "#4 #3 #2 #1", "d c b ▲" );
  PUT_TEST( "1 2 3 4 5 6 7 8 9", "<1|2|3|4|5|6|7|8|9>", "#5 #6 #7 #8 #9 #1 #2 #3 #4", "5 6 7 8 9 1 2 3 4" );
  PUT_TEST( "R▲ptor Test", "<▲ptor|est>", "C#1 F#2", "C▲ptor Fest" );
  PUT_TEST( "R▲ptor Test", "<▲ptor|est>", "C#5 F#2", "C Fest" );
  PUT_TEST( "R▲ptor Test", "<▲ptor|est>", "C#▲ F#2", "C▲ Fest" );
  PUT_TEST( "R▲ptor Test", "<▲ptor|est>", "C#0 F#2", "C Fest" );
  PUT_TEST( "R▲ptor Test", "<▲ptor|est>", "C#43 F#43", "C F" );
  PUT_TEST( "R▲ptor Test", "<▲ptor|est>", "C##43 ##F#43##", "C#43 #F#" );
  PUT_TEST( "R▲ptor Test", "<▲ptor|est>", "C##43 ##1##2", "C#43 #1#2" );
  PUT_TEST( "R▲ptor Test", "<▲ptor|est>", "##R▲ptor ##Test", "#R▲ptor #Test" );
  PUT_TEST( "R▲ptor Test Fest", "<R▲ptor> <Test>", "#1_#2", "R▲ptor_Test" );


  printf( "TEST %d *** ERRS %d\n\n", total, errs );
  return !errs;
}

int main(){
  if( raptorTest() ) puts ("I am the Raptor" );

  return 0;
}
