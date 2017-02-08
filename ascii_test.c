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

#define TRUE_TEST( text, re )                   \
  result = regexp3( text, re );                 \
  total++;                                      \
  if( result == 0 ){                            \
    printf( "%s:%d Error on TRUE TEST %s %s\n", \
            __FILE__, __LINE__, text, re );     \
    errs++;                                     \
  }

  TRUE_TEST( "ab", "a(b|c)|A(B|C)" );
  TRUE_TEST( "ac", "a(b|c)|A(B|C)" );
  TRUE_TEST( "AB", "a(b|c)|A(B|C)" );
  TRUE_TEST( "AC", "a(b|c)|A(B|C)" );
  TRUE_TEST( "ab", "a<b|c>|A<B|C>" );
  TRUE_TEST( "ac", "a<b|c>|A<B|C>" );
  TRUE_TEST( "AB", "a<b|c>|A<B|C>" );
  TRUE_TEST( "AC", "a<b|c>|A<B|C>" );
  TRUE_TEST( "ab"    , "(a(b|c)|A(B|C))|1234(ea|eb|ec)" );
  TRUE_TEST( "ac"    , "(a(b|c)|A(B|C))|1234(ea|eb|ec)" );
  TRUE_TEST( "AC"    , "(a(b|c)|A(B|C))|1234(ea|eb|ec)" );
  TRUE_TEST( "ab"    , "<a<b|c>|A<B|C>>|1234<ea|eb|ec>" );
  TRUE_TEST( "ac"    , "<a<b|c>|A<B|C>>|1234<ea|eb|ec>" );
  TRUE_TEST( "AC"    , "<a<b|c>|A<B|C>>|1234<ea|eb|ec>" );
  TRUE_TEST( "1234eb", "(a(b|c)|A(B|C))|1234(ea|eb|ec)" );
  TRUE_TEST( "1234ec", "(a(b|c)|A(B|C))|1234(ea|eb|ec)" );
  TRUE_TEST( "1234eb", "<a<b|c>|A<B|C>>|1234<ea|eb|ec>" );
  TRUE_TEST( "1234ec", "<a<b|c>|A<B|C>>|1234<ea|eb|ec>" );
  TRUE_TEST( "1234ec", "(a<b|c>|A<B|C>)|1234(ea|eb|ec)" );
  TRUE_TEST( "1234eb", "<a(b|c)|A(B|C)>|1234(ea|eb|ec)" );
  TRUE_TEST( "1234ec", "(a<b|c>|A(B|C))|1234<ea|eb|ec>" );
  TRUE_TEST( "ace", "a(b|c)(d|e)|A(B|C)(D|E)" );
  TRUE_TEST( "ABD", "a(b|c)(d|e)|A(B|C)(D|E)" );
  TRUE_TEST( "ace", "a<b|c><d|e>|A<B|C><D|E>" );
  TRUE_TEST( "ABD", "a<b|c><d|e>|A<B|C><D|E>" );

  TRUE_TEST( "ABC", "#^A" );
  TRUE_TEST( "ABC", "#^AB" );
  TRUE_TEST( "ABC", "#^ABC" );
  TRUE_TEST( "ABC", "#^(b|A)" );
  TRUE_TEST( "ABC", "#^A(B|C)(B|C)" );
  TRUE_TEST( "ABC", "#^(A(B|C))(B|C)" );
  TRUE_TEST( "ABC", "#$C" );
  TRUE_TEST( "ABC", "#$BC" );
  TRUE_TEST( "ABC", "#$ABC" );
  TRUE_TEST( "ABC", "#$(b|C)" );
  TRUE_TEST( "ABC", "#$A(B|C)(B|C)" );
  TRUE_TEST( "ABC", "#$(A(B|C))(B|C)" );
  TRUE_TEST( "ABC", "#^$ABC" );
  TRUE_TEST( "ABC", "#^$A(c|B)(b|C)" );
  TRUE_TEST( "ABC", "#^$A(B|C)(B|C)" );
  TRUE_TEST( "ABC", "#^$(A(B|C))(B|C)" );


#define FALSE_TEST( text, re )                          \
  result = regexp3( text, re );                         \
  total++;                                              \
  if( result ){                                         \
    printf( "%s:%d Error on FALSE TEST %s %s\n",        \
            __FILE__, __LINE__, text, re );             \
    errs++;                                             \
  }

  FALSE_TEST( "ab", "" );
  // FALSE_TEST( "ab", "|" );
  // FALSE_TEST( "ab", "()" );
  // FALSE_TEST( "ab", "|()" );
  // FALSE_TEST( "ab", "()|" );
  // FALSE_TEST( "ab", "(|)" );
  // FALSE_TEST( "ab", "()|()" );
  // FALSE_TEST( "ab", "(|)|(|)" );
  FALSE_TEST( "", "a" );
  FALSE_TEST( "", "a|b" );
  FALSE_TEST( "", "a(b)c" );
  FALSE_TEST( "", "raptor|(test)" );
  // FALSE_TEST( "", "|()" );
  // FALSE_TEST( "", "()|" );
  // FALSE_TEST( "", "(|)" );
  // FALSE_TEST( "", "()|()" );
  // FALSE_TEST( "", "(|)|(|)" );
  FALSE_TEST( "ax", "a(b|c)|A(B|C)" );
  FALSE_TEST( "ax", "a(b|c)|A(B|C)" );
  FALSE_TEST( "Ax", "a(b|c)|A(B|C)" );
  FALSE_TEST( "Ax", "a(b|c)|A(B|C)" );

  FALSE_TEST( "ABC", "#^E" );
  FALSE_TEST( "ABC", "#^EB" );
  FALSE_TEST( "ABC", "#^EBC" );
  FALSE_TEST( "ABC", "#^(b|E)" );
  FALSE_TEST( "ABC", "#^A(B|C)(B|E)" );
  FALSE_TEST( "ABC", "#^(A(B|C))(B|E)" );
  FALSE_TEST( "ABC", "#$E" );
  FALSE_TEST( "ABC", "#$BE" );
  FALSE_TEST( "ABC", "#$ABE" );
  FALSE_TEST( "ABC", "#$(b|E)" );
  FALSE_TEST( "ABC", "#$A(B|C)(B|E)" );
  FALSE_TEST( "ABC", "#$(A(B|C))(B|E)" );
  FALSE_TEST( "ABC", "#^$ABE" );
  FALSE_TEST( "ABC", "#^$A(c|B)(b|E)" );
  FALSE_TEST( "ABC", "#^$A(B|C)(B|E)" );
  FALSE_TEST( "ABC", "#^$(A(B|C))(B|E)" );

#define NTEST( text, re, n )                            \
  result = regexp3( text, re );                         \
  total++;                                              \
  if( result != n ){                                    \
    printf( "%s:%d Error on N-TEST >%s< >%s<\n",        \
            __FILE__, __LINE__, text, re );             \
    printf( "results %d - %d expected\n", result, n );  \
    errs++;                                             \
  }

  NTEST( "",  "",  0 );
  NTEST( "", "a",  0 );
  NTEST( "a", "",  0 );
  NTEST( "a", "o", 0 );
  NTEST( "a", "a", 1 );
  // NTEST( "a", "()", 0 );
  // NTEST( "a", "()|", 0 );
  // NTEST( "a", "|()", 0 );
  // NTEST( "a", "(|)", 0 );
  NTEST( "aaa", "a", 3 );
  NTEST( "a", "aaa", 0 );
  // NTEST( "a", "a()", 0 );
  NTEST( "a aaa aaa", "aaa", 2 );
  NTEST( "Raptor Test", "a", 1 );
  NTEST( "Raptor Test", "t", 2 );
  NTEST( "aeiou", "a|e|i|o|u", 5 );
  NTEST( "aeiou", "(a|e|i|o|u)", 5 );
  NTEST( "aeiou", "(a|e)|i|(o|u)", 5 );
  NTEST( "aeiou", "(a|(e))|(i|(o|u))", 5 );
  NTEST( "aa ae ai ao au", "a(a|e|i|o|u)", 5 );
  NTEST( "aa ae ai ao au", "a(0|1|2|3|4)", 0 );
  NTEST( "aa ae ai ao au", "a(a|e|i|o|u) ", 4 );
  NTEST( "aa ae Ai ao au", "A(a|e|i|o|u)", 1 );
  NTEST( "aa ae Ai ao au", "(A|a)(a|e|i|o|u)", 5 );
  NTEST( "aae aei Aio aoa auu", "(A|a)(a|e|i|o|u)(a|e|i|o|u)", 5 );

  NTEST( "a aaa aaa", "#^aaa", 0 );
  NTEST( "a aaa aaa", "#$aaa", 1 );
  NTEST( "a aaa aaa", "#?aaa", 1 );
  NTEST( "a aaa aaa", "#~aaa", 2 );
  NTEST( "a aaa aaa", "#^?aaa", 0 );
  NTEST( "a aaa aaa", "#?^aaa", 0 );
  NTEST( "a aaa aaa", "#^?$aaa", 0 );
  NTEST( "a aaa aaa", "#?$^aaa", 0 );
  NTEST( "a aaa aaa", "#^?$a aaa aaa", 1 );
  NTEST( "aa aaaa aaaa", "a", 10 );
  NTEST( "aa aaaa aaaa", "#~a", 10 );
  NTEST( "aa aaaa aaaa", "aa", 5 );
  NTEST( "aa aaaa aaaa", "#~aa", 7 );
  NTEST( "aa aaaa aaaa", "aaa", 2 );
  NTEST( "aa aaaa aaaa", "#~aaa", 4 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a", 20 );
  NTEST( "abababababababababababababababababababab", "a"  , 20 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "(a)", 20 );
  NTEST( "abababababababababababababababababababab", "(a)", 20 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "<a>", 20 );
  NTEST( "abababababababababababababababababababab", "<a>", 20 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a+",   1 );
  NTEST( "abababababababababababababababababababab", "a+" , 20 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "#?a+", 1 );
  NTEST( "abababababababababababababababababababab", "#?a+" , 1 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "#~a+", 20 );
  NTEST( "abababababababababababababababababababab", "#~a+" , 20 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a?", 20 );
  NTEST( "abababababababababababababababababababab", "a?" , 40 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a*", 1 );
  NTEST( "abababababababababababababababababababab", "a*" , 40 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{1}", 20 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{1,}", 1 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{1}", 20 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{5}", 4 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{5,}", 1 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{1,5}", 4 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{5,5}", 4 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{10}", 2 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{10,}", 1 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{1,100}", 1 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{001,00100}", 1 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{0001,}", 1 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{021,}", 0 );
  NTEST( "abababababababababababababababababababab", "a{1}" , 20 );
  NTEST( "abababababababababababababababababababab", "a{1,}" , 20 );
  NTEST( "abababababababababababababababababababab", "a{001}" , 20 );
  NTEST( "abababababababababababababababababababab", "a{001,}" , 20 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{1,1}", 20 );
  NTEST( "abababababababababababababababababababab", "a{1,1}" , 20 );
  NTEST( "abababababababababababababababababababab", "a{001,000001}" , 20 );
  NTEST( "aaaaaaaaaaaaaaaaaaaa", "a{20}", 1 );
  NTEST( "abababababababababababababababababababab", "(a|b){1,1}" , 40 );


  NTEST( "abc", "<b>", 1 );
  NTEST( "abc", "a<b>", 1 );
  NTEST( "abc", "<b>c", 1 );
  NTEST( "abc", "a<b>c", 1 );
  NTEST( "abc", "<a|b>", 2 );
  NTEST( "abc", "<a|b|c>", 3 );
  NTEST( "abc", "<(a|b)|c>", 3 );
  NTEST( "abc", "#~<a|b|c>", 3 );
  NTEST( "abc", "#~<ab|c>", 2 );
  NTEST( "aa aaaa aaaa", "<aa>", 5 );
  NTEST( "aa aaaa aaaa", "#~<aa>", 7 );
  NTEST( "abc", "a<x>", 0 );
  NTEST( "abc", "<a>x", 0 );
  NTEST( "abc", "<a|b>x", 0 );
  NTEST( "abc", "<<a|b>x|abc>", 1 );
  NTEST( "abc", "<x<a|b>|abc>", 1 );
  NTEST( "abc abc abc", "<a|b|c>", 9 );
  NTEST( "abc abc abc", "<(a|b|c)(a|b|c)(a|b|c)>", 3 );
  NTEST( "abc abc abc", "<(a|b|c)(a|b|c)(a|b|c)> ", 2 );
  NTEST( "abc abc abc", "#?<(a|b|c)(a|b|c)(a|b|c)>", 1 );
  NTEST( "abc abc abc", "#?<(a|b|c)(a|b|c)((a|b)|x)>", 0 );
  NTEST( "abc abc abx", "#?<(a|b|c)(a|b|c)((a|b)|x)>", 1 );
  NTEST( "abc iecc oeb", "<<(a|e)|(i|o)>e<b|c>|abc>", 3 );

  NTEST( "a", "a?", 1 );
  NTEST( "a", "b?", 1 );
  NTEST( "a", "a+", 1 );
  NTEST( "a", "a*", 1 );
  NTEST( "a", "b*", 1 );
  NTEST( "a", "aa?", 1 );
  NTEST( "a", "ab?", 1 );
  NTEST( "a", "aa+", 0 );
  NTEST( "a", "aa*", 1 );
  NTEST( "a", "ab*", 1 );
  NTEST( "a", "a{1,2}", 1 );
  NTEST( "aaa", "a+", 1 );
  NTEST( "aaa", "a*", 1 );
  NTEST( "aaa", "a+", 1 );
  NTEST( "aaa", "a?", 3 );
  NTEST( "aaab", "a+", 1 );
  NTEST( "aaab", "a*", 2 );
  NTEST( "aaab", "a?", 4 );
  NTEST( "aaab", "a+b", 1 );
  NTEST( "aaab", "a*b", 1 );
  NTEST( "aaab", "a?b", 1 );
  NTEST( "aaab", "a+b?", 1 );
  NTEST( "aaab", "a*b?", 1 );
  NTEST( "aaab", "a?b?", 3 );
  NTEST( "aaab", "a+b+", 1 );
  NTEST( "aaab", "a*b+", 1 );
  NTEST( "aaab", "a?b+", 1 );
  NTEST( "aaab", "a+b*", 1 );
  NTEST( "aaab", "a*b*", 1 );
  NTEST( "aaab", "a?b*", 3 );
  NTEST( "aaabaaa", "a+", 2 );
  NTEST( "aaabaaa", "a*", 3 );
  NTEST( "aaabaaa", "a*", 3 );
  NTEST( "aaabaaa", "a*", 3 );
  NTEST( "a", "(a)?", 1 );
  NTEST( "a", "(b)?", 1 );
  NTEST( "a", "(a)+", 1 );
  NTEST( "a", "(a)*", 1 );
  NTEST( "a", "(b)*", 1 );
  NTEST( "aaa", "(a)+", 1 );
  NTEST( "aaa", "(a)*", 1 );
  NTEST( "aaa", "#~(a)+", 3 );
  NTEST( "aaa", "#~(a)*", 3 );
  NTEST( "aaab", "#~(a+)", 3 );
  NTEST( "aaab", "#~(a*)", 4 );
  NTEST( "aaab", "#~(a?)", 4 );
  NTEST( "aaab", "#~(a+b)", 3 );
  NTEST( "aaab", "#~(a*b)", 4 );
  NTEST( "aaab", "#~(a?b)", 2 );
  NTEST( "aaab", "#~(a+b?)", 3 );
  NTEST( "aaab", "#~(a*b?)", 4 );
  NTEST( "aaab", "#~(a?b?)", 4 );
  NTEST( "aaab", "#~(a+b+)", 3 );
  NTEST( "aaab", "#~(a*b+)", 4 );
  NTEST( "aaab", "#~(a?b+)", 2 );
  NTEST( "aaab", "#~(a+b*)", 3 );
  NTEST( "aaab", "#~(a*b*)", 4 );
  NTEST( "aaab", "#~(a?b*)", 4 );
  NTEST( "aaabaaa", "#~(a+)", 6 );
  NTEST( "aaabaaa", "#~(a*)", 7 );

  NTEST( "Raptor Raptor Raptor", "Raptor", 3 );
  NTEST( "Raptor Raptor Raptor", ":waptor", 3 );
  NTEST( "Raptor Raptor Raptor", ":wa:w:wor", 3 );
  NTEST( "Raptor Raptor Raptor", ":w+", 3 );
  NTEST( "Raptor Raptor Raptor", ":w*", 5 );
  NTEST( "Raptor Raptor Raptor", ":w{6}", 3 );
  NTEST( "Raptor Raptor Raptor", ":w{6}:s?", 3 );

  NTEST( "Raptor Raaptor Raaaptor", "Ra+ptor", 3 );
  NTEST( "Raptor Raaptor Raaaptor", "Ra*ptor", 3 );
  NTEST( "Raptor Raaptor Raaaptor", "R:aptor", 1 );
  NTEST( "Raptor Raaptor Raaaptor", "R[aeiou]+ptor", 3 );

  NTEST( "Raptor Test",     "<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "Raaaaptor TFest", "<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "CaptorTest",      "<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "Cap CaptorTest",  "<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "Rap Captor Fest", "<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "Raptor Test",     "#^$<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "Raaaaptor TFest", "#^$<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "CaptorTest",      "#^$<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "Cap CaptorTest",  "#^$<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "Rap Captor Fest", "#^$<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "Raptor Test",     "#~?<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "Raaaaptor TFest", "#~?<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "CaptorTest",      "#~?<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "Cap CaptorTest",  "#~?<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );
  NTEST( "Rap Captor Fest", "#~?<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1 );

  NTEST( "a", ":a", 1 );
  NTEST( "a", ":A", 0 );
  NTEST( "a", ":d", 0 );
  NTEST( "a", ":D", 1 );
  NTEST( "a", ":w", 1 );
  NTEST( "a", ":W", 0 );
  NTEST( "a", ":s", 0 );
  NTEST( "a", ":S", 1 );
  NTEST( "A", ":a", 1 );
  NTEST( "A", ":A", 0 );
  NTEST( "A", ":d", 0 );
  NTEST( "A", ":D", 1 );
  NTEST( "A", ":w", 1 );
  NTEST( "A", ":W", 0 );
  NTEST( "A", ":s", 0 );
  NTEST( "A", ":S", 1 );
  NTEST( "4", ":a", 0 );
  NTEST( "4", ":A", 1 );
  NTEST( "4", ":d", 1 );
  NTEST( "4", ":D", 0 );
  NTEST( "4", ":w", 1 );
  NTEST( "4", ":W", 0 );
  NTEST( "4", ":s", 0 );
  NTEST( "4", ":S", 1 );
  NTEST( " ", ":a", 0 );
  NTEST( " ", ":A", 1 );
  NTEST( " ", ":d", 0 );
  NTEST( " ", ":D", 1 );
  NTEST( " ", ":w", 0 );
  NTEST( " ", ":W", 1 );
  NTEST( " ", ":s", 1 );
  NTEST( " ", ":S", 0 );
  NTEST( "\t", ":a", 0 );
  NTEST( "\t", ":A", 1 );
  NTEST( "\t", ":d", 0 );
  NTEST( "\t", ":D", 1 );
  NTEST( "\t", ":w", 0 );
  NTEST( "\t", ":W", 1 );
  NTEST( "\t", ":s", 1 );
  NTEST( "\t", ":S", 0 );

  NTEST( "eee", ":a", 3 );
  NTEST( "aaa", ":A", 0 );
  NTEST( "aaa", ":d", 0 );
  NTEST( "aaa", ":D", 3 );
  NTEST( "aaa", ":w", 3 );
  NTEST( "aaa", ":W", 0 );
  NTEST( "aaa", ":s", 0 );
  NTEST( "aaa", ":S", 3 );
  NTEST( "AAA", ":a", 3 );
  NTEST( "AAA", ":A", 0 );
  NTEST( "AAA", ":d", 0 );
  NTEST( "AAA", ":D", 3 );
  NTEST( "AAA", ":w", 3 );
  NTEST( "AAA", ":W", 0 );
  NTEST( "AAA", ":s", 0 );
  NTEST( "AAA", ":S", 3 );
  NTEST( "444", ":a", 0 );
  NTEST( "444", ":A", 3 );
  NTEST( "444", ":d", 3 );
  NTEST( "444", ":D", 0 );
  NTEST( "444", ":w", 3 );
  NTEST( "444", ":W", 0 );
  NTEST( "444", ":s", 0 );
  NTEST( "444", ":S", 3 );
  NTEST( "   ", ":a", 0 );
  NTEST( "   ", ":A", 3 );
  NTEST( "   ", ":d", 0 );
  NTEST( "   ", ":D", 3 );
  NTEST( "   ", ":w", 0 );
  NTEST( "   ", ":W", 3 );
  NTEST( "   ", ":s", 3 );
  NTEST( "   ", ":S", 0 );
  NTEST( "\t\t\t", ":a", 0 );
  NTEST( "\t\t\t", ":A", 3 );
  NTEST( "\t\t\t", ":d", 0 );
  NTEST( "\t\t\t", ":D", 3 );
  NTEST( "\t\t\t", ":w", 0 );
  NTEST( "\t\t\t", ":W", 3 );
  NTEST( "\t\t\t", ":s", 3 );
  NTEST( "\t\t\t", ":S", 0 );

  NTEST( "eee", ":a+", 1 );
  NTEST( "aaa", ":A+", 0 );
  NTEST( "aaa", ":d+", 0 );
  NTEST( "aaa", ":D+", 1 );
  NTEST( "aaa", ":w+", 1 );
  NTEST( "aaa", ":W+", 0 );
  NTEST( "aaa", ":s+", 0 );
  NTEST( "aaa", ":S+", 1 );
  NTEST( "AAA", ":a+", 1 );
  NTEST( "AAA", ":A+", 0 );
  NTEST( "AAA", ":d+", 0 );
  NTEST( "AAA", ":D+", 1 );
  NTEST( "AAA", ":w+", 1 );
  NTEST( "AAA", ":W+", 0 );
  NTEST( "AAA", ":s+", 0 );
  NTEST( "AAA", ":S+", 1 );
  NTEST( "444", ":a+", 0 );
  NTEST( "444", ":A+", 1 );
  NTEST( "444", ":d+", 1 );
  NTEST( "444", ":D+", 0 );
  NTEST( "444", ":w+", 1 );
  NTEST( "444", ":W+", 0 );
  NTEST( "444", ":s+", 0 );
  NTEST( "444", ":S+", 1 );
  NTEST( "   ", ":a+", 0 );
  NTEST( "   ", ":A+", 1 );
  NTEST( "   ", ":d+", 0 );
  NTEST( "   ", ":D+", 1 );
  NTEST( "   ", ":w+", 0 );
  NTEST( "   ", ":W+", 1 );
  NTEST( "   ", ":s+", 1 );
  NTEST( "   ", ":S+", 0 );
  NTEST( "\t\t\t", ":a+", 0 );
  NTEST( "\t\t\t", ":A+", 1 );
  NTEST( "\t\t\t", ":d+", 0 );
  NTEST( "\t\t\t", ":D+", 1 );
  NTEST( "\t\t\t", ":w+", 0 );
  NTEST( "\t\t\t", ":W+", 1 );
  NTEST( "\t\t\t", ":s+", 1 );
  NTEST( "\t\t\t", ":S+", 0 );

  NTEST( "eee", "#~:a+", 3 );
  NTEST( "aaa", "#~:A+", 0 );
  NTEST( "aaa", "#~:d+", 0 );
  NTEST( "aaa", "#~:D+", 3 );
  NTEST( "aaa", "#~:w+", 3 );
  NTEST( "aaa", "#~:W+", 0 );
  NTEST( "aaa", "#~:s+", 0 );
  NTEST( "aaa", "#~:S+", 3 );
  NTEST( "AAA", "#~:a+", 3 );
  NTEST( "AAA", "#~:A+", 0 );
  NTEST( "AAA", "#~:d+", 0 );
  NTEST( "AAA", "#~:D+", 3 );
  NTEST( "AAA", "#~:w+", 3 );
  NTEST( "AAA", "#~:W+", 0 );
  NTEST( "AAA", "#~:s+", 0 );
  NTEST( "AAA", "#~:S+", 3 );
  NTEST( "444", "#~:a+", 0 );
  NTEST( "444", "#~:A+", 3 );
  NTEST( "444", "#~:d+", 3 );
  NTEST( "444", "#~:D+", 0 );
  NTEST( "444", "#~:w+", 3 );
  NTEST( "444", "#~:W+", 0 );
  NTEST( "444", "#~:s+", 0 );
  NTEST( "444", "#~:S+", 3 );
  NTEST( "   ", "#~:a+", 0 );
  NTEST( "   ", "#~:A+", 3 );
  NTEST( "   ", "#~:d+", 0 );
  NTEST( "   ", "#~:D+", 3 );
  NTEST( "   ", "#~:w+", 0 );
  NTEST( "   ", "#~:W+", 3 );
  NTEST( "   ", "#~:s+", 3 );
  NTEST( "   ", "#~:S+", 0 );
  NTEST( "\t\t\t", "#~:a+", 0 );
  NTEST( "\t\t\t", "#~:A+", 3 );
  NTEST( "\t\t\t", "#~:d+", 0 );
  NTEST( "\t\t\t", "#~:D+", 3 );
  NTEST( "\t\t\t", "#~:w+", 0 );
  NTEST( "\t\t\t", "#~:W+", 3 );
  NTEST( "\t\t\t", "#~:s+", 3 );
  NTEST( "\t\t\t", "#~:S+", 0 );

  NTEST( "aeiou", ":a", 5 );

  NTEST( "(((", ":(", 3 );
  NTEST( ")))", ":)", 3 );
  NTEST( "<<<", ":<", 3 );
  NTEST( ">>>", ":>", 3 );
  NTEST( ":::", "::", 3 );
  NTEST( "|||", ":|", 3 );
  NTEST( "###", ":#", 3 );
  NTEST( ":#()|<>", ":::#:(:):|:<:>", 1 );
  NTEST( ":#()|<>", "(::|:#|:(|:)|:||:<|:>)", 7 );
  NTEST( "(test1)(test2)", ":(test:d:)", 2 );

  NTEST( "",  ".",  0 );
  NTEST( "a", ".",  1 );
  NTEST( "aaa", ".", 3 );
  NTEST( "a", "...", 0 );
  NTEST( "a aaa aaa", ".", 9 );
  NTEST( "a aaa aaa", "...", 3 );
  NTEST( "a aaa aaa", ".aa", 2 );
  NTEST( "a aaa aaa", "aa.", 2 );
  NTEST( "Raptor Test", ".a", 1 );
  NTEST( "Raptor Test", ".t", 2 );
  NTEST( "Raptor Test", ".z", 0 );
  NTEST( "Raptor Test", "a.", 1 );
  NTEST( "Raptor Test", " .", 1 );
  NTEST( "Raptor Test", "z.", 0 );
  NTEST( "a", ".?", 1 );
  NTEST( "a", ".+", 1 );
  NTEST( "a", ".*", 1 );
  NTEST( "a", ".{1}", 1 );
  NTEST( "a aaa aaa", ".?", 9 );
  NTEST( "a aaa aaa", ".+", 1 );
  NTEST( "a aaa aaa", ".*", 1 );
  NTEST( "a aaa aaa", ".{1}", 9 );
  NTEST( "a", "a.?", 1 );
  NTEST( "a", "a.+", 0 );
  NTEST( "a", "a.*", 1 );
  NTEST( "a", "a.{1}", 0 );
  NTEST( "aeiou", "a|.", 5 );
  NTEST( "aeiou", "a|.?", 5 );
  NTEST( "aeiou", "a|.+", 2 );
  NTEST( "aeiou", "a|.*", 2 );
  NTEST( "aeiou", ".|a", 5 );
  NTEST( "aeiou", ".?|a", 5 );
  NTEST( "aeiou", ".+|a", 1 );
  NTEST( "aeiou", ".*|a", 1);
  NTEST( "aeiou", "(a|.)", 5 );
  NTEST( "aeiou", "(a|.?)", 5 );
  NTEST( "aeiou", "(a|.+)", 2 );
  NTEST( "aeiou", "(a|.*)", 2 );
  NTEST( "aeiou", "(.|a)", 5 );
  NTEST( "aeiou", "(.?|a)", 5 );
  NTEST( "aeiou", "(.+|a)", 1 );
  NTEST( "aeiou", "(.*|a)", 1);
  NTEST( "aeiou", "a|(.)", 5 );
  NTEST( "aeiou", "a|(.?)", 5 );
  NTEST( "aeiou", "a|(.+)", 2 );
  NTEST( "aeiou", "a|(.*)", 2 );
  NTEST( "aeiou", "(.)|a", 5 );
  NTEST( "aeiou", "(.?)|a", 5 );
  NTEST( "aeiou", "(.+)|a", 1 );
  NTEST( "aeiou", "(.*)|a", 1);
  NTEST( "aeiou", "a|(.)", 5 );
  NTEST( "aeiou", "a|(.)?", 5 );
  NTEST( "aeiou", "a|(.)+", 2 );
  NTEST( "aeiou", "a|(.)*", 2 );
  NTEST( "aeiou", "(.)|a", 5 );
  NTEST( "aeiou", "(.)?|a", 5 );
  NTEST( "aeiou", "(.)+|a", 1 );
  NTEST( "aeiou", "(.)*|a", 1);
  NTEST( "abababababababababababababababababababab", "(a.)" , 20 );
  NTEST( "abababababababababababababababababababab", "(.a)" , 19 );
  NTEST( "abababababababababababababababababababab", "(:a.)" , 20 );
  NTEST( "abababababababababababababababababababab", "(.:a)" , 20 );
  NTEST( "abababababababababababababababababababab", "(.{5}:a{5})" , 4 );

  NTEST( "",  "a-z",  0 );
  NTEST( "a", "a-z",  1 );
  NTEST( "a", "A-Z",  0 );
  NTEST( "A", "A-Z",  1 );
  NTEST( "1", "a-z",  0 );
  NTEST( "1", "0-9",  1 );
  NTEST( "a", "0-9",  0 );
  NTEST( "A", "0-9",  0 );
  NTEST( "aaa", "a-z", 3 );
  NTEST( "a", "a-za-za-z", 0 );
  NTEST( "a aaa aaa", "a-z", 7 );
  NTEST( "a aaa aaa", "a-za-za-z", 2 );
  NTEST( "a aaa aaa", "a-zaa", 2 );
  NTEST( "a aaa aaa", "aaa-z", 2 );
  NTEST( "Raptor Test", "A-Za", 1 );
  NTEST( "Raptor Test", "a-zt", 2 );
  NTEST( "Raptor Test", "a-zz", 0 );
  NTEST( "Raptor Test", "aa-z", 1 );
  NTEST( "Raptor Test", " A-Z", 1 );
  NTEST( "Raptor Test", "za-z", 0 );
  NTEST( "a", "a-z?", 1 );
  NTEST( "a", "a-z+", 1 );
  NTEST( "a", "a-z*", 1 );
  NTEST( "a", "a-z{1}", 1 );
  NTEST( "a aaa aaa", "a-z?", 9 );
  NTEST( "a aaa aaa", "a-z+", 3 );
  NTEST( "a aaa aaa", "a-z*", 5 );
  NTEST( "a aaa aaa", "a-z{1}", 7 );
  NTEST( "a", "aa-z?", 1 );
  NTEST( "a", "aa-z+", 0 );
  NTEST( "a", "aa-z*", 1 );
  NTEST( "a", "aa-z{1}", 0 );
  NTEST( "aeiou", "a|a-z", 5 );
  NTEST( "aeiou", "a|a-z?", 5 );
  NTEST( "aeiou", "a|a-z+", 2 );
  NTEST( "aeiou", "a|a-z*", 2 );
  NTEST( "aeiou", "a-z|a", 5 );
  NTEST( "aeiou", "a-z?|a", 5 );
  NTEST( "aeiou", "a-z+|a", 1 );
  NTEST( "aeiou", "a-z*|a", 1);
  NTEST( "aeiou", "(a|a-z)", 5 );
  NTEST( "aeiou", "(a|a-z?)", 5 );
  NTEST( "aeiou", "(a|a-z+)", 2 );
  NTEST( "aeiou", "(a|a-z*)", 2 );
  NTEST( "aeiou", "(a-z|a)", 5 );
  NTEST( "aeiou", "(a-z?|a)", 5 );
  NTEST( "aeiou", "(a-z+|a)", 1 );
  NTEST( "aeiou", "(a-z*|a)", 1);
  NTEST( "aeiou", "a|(a-z)", 5 );
  NTEST( "aeiou", "a|(a-z?)", 5 );
  NTEST( "aeiou", "a|(a-z+)", 2 );
  NTEST( "aeiou", "a|(a-z*)", 2 );
  NTEST( "aeiou", "(a-z)|a", 5 );
  NTEST( "aeiou", "(a-z?)|a", 5 );
  NTEST( "aeiou", "(a-z+)|a", 1 );
  NTEST( "aeiou", "(a-z*)|a", 1);
  NTEST( "aeiou", "a|(a-z)", 5 );
  NTEST( "aeiou", "a|(a-z)?", 5 );
  NTEST( "aeiou", "a|(a-z)+", 2 );
  NTEST( "aeiou", "a|(a-z)*", 2 );
  NTEST( "aeiou", "(a-z)|a", 5 );
  NTEST( "aeiou", "(a-z)?|a", 5 );
  NTEST( "aeiou", "(a-z)+|a", 1 );
  NTEST( "aeiou", "(a-z)*|a", 1);
  NTEST( "1a2a3a4a5a6a", "1-6a", 6 );
  NTEST( "1a2a3a4a5a6a", "1-3a", 3 );
  NTEST( "1a2b3c4d5e6f", "1-6a-z", 6 );
  NTEST( "1a2b3c4d5e6f", "1-3a-z", 3 );
  NTEST( "abababababababababababababababababababab", "(aa-z)" , 20 );
  NTEST( "abababababababababababababababababababab", "(a-za)" , 19 );
  NTEST( "abababababababababababababababababababab", "(:aa-z)" , 20 );
  NTEST( "abababababababababababababababababababab", "(a-z:a)" , 20 );
  NTEST( "abababababababababababababababababababab", "(a-z{5}:a{5})" , 4 );

  NTEST( "",  "[a]",  0 );
  NTEST( "a", "[a]",  1 );
  NTEST( "a", "[.]",  1 );
  NTEST( ".", "[.]",  1 );
  NTEST( "a", "[A]",  0 );
  NTEST( "A", "[A]",  1 );
  NTEST( "1", "[A]",  0 );
  NTEST( "1", "[1]",  1 );
  NTEST( "a", "[:a]", 1 );
  NTEST( "A", "[:D]", 1 );
  NTEST( "aaa", "[a-z]", 3 );
  NTEST( "a", "[a-z][a-z][a-z]", 0 );
  NTEST( "a aaa aaa", "[a-z]", 7 );
  NTEST( "a aaa aaa", "[ a-z]", 9 );
  NTEST( "a aaa aaa", "[a-z][a-z][a-z]", 2 );
  NTEST( "a aaa aaa", "[a-z]aa", 2 );
  NTEST( "a aaa aaa", "aa[a-z]", 2 );
  NTEST( "Raptor Test", "[:w]a", 1 );
  NTEST( "Raptor Test", "[:w]t", 2 );
  NTEST( "Raptor Test", "[a-z]z", 0 );
  NTEST( "Raptor Test", "a[a-z]", 1 );
  NTEST( "Raptor Test", " [A-Z]", 1 );
  NTEST( "Raptor Test", "z[a-z]", 0 );
  NTEST( "a", "[a]?", 1 );
  NTEST( "a", "[a]+", 1 );
  NTEST( "a", "[a]*", 1 );
  NTEST( "a", "[a]{1}", 1 );
  NTEST( "a aaa aaa", "[a-z]?", 9 );
  NTEST( "a aaa aaa", "[a-z]+", 3 );
  NTEST( "a aaa aaa", "[a-z]*", 5 );
  NTEST( "a aaa aaa", "[a-z]{1}", 7 );
  NTEST( "a", "a[a-z]?", 1 );
  NTEST( "a", "a[a-z]+", 0 );
  NTEST( "a", "a[a-z]*", 1 );
  NTEST( "a", "a[a-z]{1}", 0 );
  NTEST( "aeiou", "a|[aeiou]", 5 );
  NTEST( "aeiou", "a|[aeiou]?", 5 );
  NTEST( "aeiou", "a|[aeiou]+", 2 );
  NTEST( "aeiou", "a|[aeiou]*", 2 );
  NTEST( "aeiou", "[aeiou]|a", 5 );
  NTEST( "aeiou", "[aeiou]?|a", 5 );
  NTEST( "aeiou", "[aeiou]+|a", 1 );
  NTEST( "aeiou", "[aeiou]*|a", 1);
  NTEST( "aeiou", "(a|[aeiou])", 5 );
  NTEST( "aeiou", "(a|[aeiou]?)", 5 );
  NTEST( "aeiou", "(a|[aeiou]+)", 2 );
  NTEST( "aeiou", "(a|[aeiou]*)", 2 );
  NTEST( "aeiou", "([aeiou]|a)", 5 );
  NTEST( "aeiou", "([aeiou]?|a)", 5 );
  NTEST( "aeiou", "([aeiou]+|a)", 1 );
  NTEST( "aeiou", "([aeiou]*|a)", 1);
  NTEST( "aeiou", "a|([aeiou])", 5 );
  NTEST( "aeiou", "a|([aeiou]?)", 5 );
  NTEST( "aeiou", "a|([aeiou]+)", 2 );
  NTEST( "aeiou", "a|([aeiou]*)", 2 );
  NTEST( "aeiou", "([aeiou])|a", 5 );
  NTEST( "aeiou", "([aeiou]?)|a", 5 );
  NTEST( "aeiou", "([aeiou]+)|a", 1 );
  NTEST( "aeiou", "([aeiou]*)|a", 1);
  NTEST( "aeiou", "a|([aeiou])", 5 );
  NTEST( "aeiou", "a|([aeiou])?", 5 );
  NTEST( "aeiou", "a|([aeiou])+", 2 );
  NTEST( "aeiou", "a|([aeiou])*", 2 );
  NTEST( "aeiou", "([aeiou])|a", 5 );
  NTEST( "aeiou", "([aeiou])?|a", 5 );
  NTEST( "aeiou", "([aeiou])+|a", 1 );
  NTEST( "aeiou", "([aeiou])*|a", 1);
  NTEST( "1a2a3a4a5a6a", "[1-6]a", 6 );
  NTEST( "1a2a3a4a5a6a", "[1-3]a", 3 );
  NTEST( "1a2b3c4d5e6f", "[123456][abcdef]", 6 );
  NTEST( "1a2b3c4d5e6f", "[123][abcdef]", 3 );
  NTEST( ".b.b.b.b.b.b.b.b.b.b.b.b.b.b.b.b.b.b.b.b", "[:.]",  20 );
  NTEST( ".b.b.b.b.b.b.b.b.b.b.b.b.b.b.b.b.b.b.b.b", "[:.b]",  40 );
  NTEST( "abababababababababababababababababababab", "(a[ab])" , 20 );
  NTEST( "abababababababababababababababababababab", "([ab]a)" , 19 );
  NTEST( "abababababababababababababababababababab", "(:a[ab])" , 20 );
  NTEST( "abababababababababababababababababababab", "([ab]:a)" , 20 );
  NTEST( "abababababababababababababababababababab", "([ab]{5}:a{5})" , 4 );
  NTEST( "()<>[]{}*?+", "[:(:):<:>:[:]:{:}:*:?:+]", 11 );
  NTEST( ")>}", "[)>}]", 3 );

  NTEST( "",  "[^a]",  0 );
  NTEST( "a", "[^1]",  1 );
  NTEST( "a", "[^a]",  0 );
  NTEST( "A", "[^a]",  1 );
  NTEST( "1", "[^1]",  0 );
  NTEST( "1", "[^A]",  1 );
  NTEST( "a", "[^:a]", 0 );
  NTEST( "A", "[^:A]", 1 );
  NTEST( "aaa", "[^z]", 3 );
  NTEST( "a", "[^z][^z][^z]", 0 );
  NTEST( "a aaa aaa", "[^ ]", 7 );
  NTEST( "a aaa aaa", "[^ a]", 0 );
  NTEST( "a aaa aaa", "[^:d]", 9 );
  NTEST( "a aaa aaa", "[^:d:s]", 7 );
  NTEST( "a aaa aaa", "[^:d:s][^:d:s][^:d:s]", 2 );
  NTEST( "a aaa aaa", "[^:d:s]aa", 2 );
  NTEST( "a aaa aaa", "aa[^:d:s]", 2 );
  NTEST( "Raptor Test", "[^:d:s]a", 1 );
  NTEST( "Raptor Test", "[^A-Z]t", 2 );
  NTEST( "Raptor Test", "[^:s]z", 0 );
  NTEST( "Raptor Test", "a[^ ]", 1 );
  NTEST( "Raptor Test", " [^t]", 1 );
  NTEST( "Raptor Test", "z[^a]", 0 );
  NTEST( "a", "[^z]?", 1 );
  NTEST( "a", "[^z]+", 1 );
  NTEST( "a", "[^z]*", 1 );
  NTEST( "a", "[^z]{1}", 1 );
  NTEST( "a aaa aaa", "[^ ]?", 9 );
  NTEST( "a aaa aaa", "[^ ]+", 3 );
  NTEST( "a aaa aaa", "[^ ]*", 5 );
  NTEST( "a aaa aaa", "[^ ]{1}", 7 );
  NTEST( "a", "a[^ ]?", 1 );
  NTEST( "a", "a[^ ]+", 0 );
  NTEST( "a", "a[^ ]*", 1 );
  NTEST( "a", "a[^ ]{1}", 0 );
  NTEST( "aeiou", "a|[^ ]", 5 );
  NTEST( "aeiou", "a|[^ ]?", 5 );
  NTEST( "aeiou", "a|[^ ]+", 2 );
  NTEST( "aeiou", "a|[^ ]*", 2 );
  NTEST( "aeiou", "[^ ]|a", 5 );
  NTEST( "aeiou", "[^ ]?|a", 5 );
  NTEST( "aeiou", "[^ ]+|a", 1 );
  NTEST( "aeiou", "[^ ]*|a", 1);
  NTEST( "aeiou", "(a|[^ ])", 5 );
  NTEST( "aeiou", "(a|[^ ]?)", 5 );
  NTEST( "aeiou", "(a|[^ ]+)", 2 );
  NTEST( "aeiou", "(a|[^ ]*)", 2 );
  NTEST( "aeiou", "([^ ]|a)", 5 );
  NTEST( "aeiou", "([^ ]?|a)", 5 );
  NTEST( "aeiou", "([^ ]+|a)", 1 );
  NTEST( "aeiou", "([^ ]*|a)", 1);
  NTEST( "aeiou", "a|([^ ])", 5 );
  NTEST( "aeiou", "a|([^ ]?)", 5 );
  NTEST( "aeiou", "a|([^ ]+)", 2 );
  NTEST( "aeiou", "a|([^ ]*)", 2 );
  NTEST( "aeiou", "([^ ])|a", 5 );
  NTEST( "aeiou", "([^ ]?)|a", 5 );
  NTEST( "aeiou", "([^ ]+)|a", 1 );
  NTEST( "aeiou", "([^ ]*)|a", 1);
  NTEST( "aeiou", "a|([^ ])", 5 );
  NTEST( "aeiou", "a|([^ ])?", 5 );
  NTEST( "aeiou", "a|([^ ])+", 2 );
  NTEST( "aeiou", "a|([^ ])*", 2 );
  NTEST( "aeiou", "([^ ])|a", 5 );
  NTEST( "aeiou", "([^ ])?|a", 5 );
  NTEST( "aeiou", "([^ ])+|a", 1 );
  NTEST( "aeiou", "([^ ])*|a", 1);
  NTEST( "1a2a3a4a5a6a", "[^:a]a", 6 );
  NTEST( "1a2a3a4a5a6a", "[^4-6]a", 3 );
  NTEST( "1a2b3c4d5e6f", "[^:a][^:d]", 6 );
  NTEST( "1a2b3c4d5e6f", "[^4-6][^:d]", 3 );
  NTEST( "abababababababababababababababababababab", "(a[^a])" , 20 );
  NTEST( "abababababababababababababababababababab", "([^a]a)" , 19 );
  NTEST( "abababababababababababababababababababab", "(:a[^a])" , 20 );
  NTEST( "abababababababababababababababababababab", "([^b]:a)" , 20 );
  NTEST( "abababababababababababababababababababab", "([^x]{5}:a{5})" , 4 );
  NTEST( "()<>[]{}*?+", "[^:w]", 11 );

  NTEST( "",  "@1",  0 );
  NTEST( "a", "@1",  0 );
  NTEST( "a", "@a",  0 );
  NTEST( "A", "@100", 0 );
  NTEST( "1", "@1",  0 );
  NTEST( "",  "[@1]",  0 );
  NTEST( "a", "[@1]",  0 );
  NTEST( "a", "[@a]",  1 );
  NTEST( "A", "[@100]", 0 );
  NTEST( "1", "[@1]",  1 );
  NTEST( "@", "[@1]",  1 );
  NTEST( "1@@a", "[a@1]",  4 );
  NTEST( "",  "[^@1",  0 );
  NTEST( "a", "[^@1]",  1 );
  NTEST( "a", "[^@a]",  0 );
  NTEST( "A", "[^@100]", 1 );
  NTEST( "1", "[^@1]",  0 );
  NTEST( "@", "[^2@]",  0 );

  NTEST( "",  "(@1)",  0 );
  NTEST( "a", "(@1)",  0 );
  NTEST( "a", "(@a)",  0 );
  NTEST( "A", "(@100)", 0 );
  NTEST( "1", "(@1)",  0 );
  NTEST( "",  "([@1])",  0 );
  NTEST( "a", "([@1])",  0 );
  NTEST( "a", "([@a])",  1 );
  NTEST( "A", "([@100])", 0 );
  NTEST( "1", "([@1])",  1 );

  NTEST( "a", "<a>@1",  0 );
  NTEST( "a", "<a>@1?", 1 );
  NTEST( "a", "<a>@1*", 1 );
  NTEST( "a", "<a>@1+", 0 );
  NTEST( "a", "<a>@1{1}", 0 );
  NTEST( "a", "<x?>@1", 0 );
  NTEST( "a", "<x?>@1?", 1 );
  NTEST( "a", "<x?>@1*", 1 );
  NTEST( "a", "<x?>@1+", 0 );
  NTEST( "aa", "<a>@1",  1 );
  NTEST( "aa", "<a>@1?", 1 );
  NTEST( "aa", "<a>@1+", 1 );
  NTEST( "aa", "<a>@1*", 1 );
  NTEST( "aa", "<a>@1{1}", 1 );
  NTEST( "aaaaa", "<a>@1",  2 );
  NTEST( "aaaaa", "<a>@1?", 3 );
  NTEST( "aaaaa", "<a>@1+", 1 );
  NTEST( "aaaaa", "<a>@1*", 1 );
  NTEST( "aaaaa", "<a>@1{1}", 2 );

  NTEST( "a-a", "<a|:d|o_O!>-@1",  1 );
  NTEST( "1-1", "<a|:d|o_O!>-@1", 1 );
  NTEST( "o_O!-o_O!", "<a|:d|o_O!>-@1", 1 );

  NTEST( "ae_ea", "<a><e>_@2@1", 1 );
  NTEST( "ae_ea", "<<a><e>>_@2@1", 0 );
  NTEST( "ae_aae", "<<a><e>>_@2@1", 1 );
  NTEST( "ae_eaae_ea", "<a><e>_@2@1", 2 );
  NTEST( "ae_eaae_ea", "<<a><e>>_@2@1", 0 );
  NTEST( "ae_aaeae_aae", "<<a><e>>_@2@1", 2 );
  NTEST( "ae_aaeae_aa1", "<<a><e>>_@2@1", 1 );

  NTEST( "aaaaa", "@1<a>", 0 );
  NTEST( "aaaaa", "@1<a>|<a>", 3 );
  NTEST( "xxaxa", "@1<a>|<x>", 3 );
  NTEST( "xxax", "@1<a>|<x>", 3 );
  NTEST( "x1xax", "@1<a>|<x>1", 2 );
  NTEST( "x1xaxa", "@1<a>|<x>1", 2 );
  NTEST( "x1xaaa", "@1<a>|<x>1", 3 );

  NTEST( "012345678910_012345678910_012345678910", "<0><1><2><3><4><5><6><7><8><9><10>_<@1@2@3@4@5@6@7@8@9@10@11>_@12", 1 );
  NTEST( "012345678910_012345678910_012345678919", "<0><1><2><3><4><5><6><7><8><9><10>_<@1@2@3@4@5@6@7@8@9@10@11>_@12", 0 );

  NTEST( "a", "a?#*", 1 );
  NTEST( "a", "b?#*", 1 );
  NTEST( "a", "a+#*", 1 );
  NTEST( "a", "a*#*", 1 );
  NTEST( "a", "b*#*", 1 );
  NTEST( "aaa", "a+#*", 1 );
  NTEST( "aaa", "a*#*", 1 );
  NTEST( "aaa", "a+#*", 1 );
  NTEST( "aaa", "a?#*", 3 );
  NTEST( "aaab", "a+#*", 1 );
  NTEST( "aaab", "a*#*", 2 );
  NTEST( "aaab", "a?#*", 4 );
  NTEST( "aaab", "a+#*?^$~b", 1 );
  NTEST( "aaab", "a*#*?^$~b", 1 );
  NTEST( "aaab", "a?#*?^$~b", 1 );
  NTEST( "aaab", "a+#*?^$~b?", 1 );
  NTEST( "aaab", "a*#*?^$~b?", 1 );
  NTEST( "aaab", "a?#*?^$~b?", 3 );
  NTEST( "aaab", "a+#*?^$~b+", 1 );
  NTEST( "aaab", "a*#*?^$~b+", 1 );
  NTEST( "aaab", "a?#*?^$~b+", 1 );
  NTEST( "aaab", "a+#*?^$~b*", 1 );
  NTEST( "aaab", "a*#*?^$~b*", 1 );
  NTEST( "aaab", "a?#*?^$~b*", 3 );

  NTEST( "a", "a#*", 1 );
  NTEST( "a", "A#*", 1 );
  NTEST( "a", "#*A", 1 );
  NTEST( "a", "#*a", 1 );
  NTEST( "a", "#*(A)", 1 );
  NTEST( "a", "#*(a)", 1 );
  NTEST( "a", "#*[A]", 1 );
  NTEST( "a", "#*[a]", 1 );
  NTEST( "a", "#*A-Z", 1 );
  NTEST( "a", "A-Z#*", 1 );
  NTEST( "a", "(a)#*", 1 );
  NTEST( "a", "(A)#*", 1 );
  NTEST( "a", "[a]#*", 1 );
  NTEST( "a", "[A]#*", 1 );
  NTEST( "a", "#*[A-Z]", 1 );
  NTEST( "a", "[A-Z]#*", 1 );

  NTEST( "aAaA", "a#*", 4 );
  NTEST( "aAaA", "A#*", 4 );
  NTEST( "aAaA", "#*A", 4 );
  NTEST( "aAaA", "#*a", 4 );
  NTEST( "aAaA", "#*(A)", 4 );
  NTEST( "aAaA", "#*(a)", 4 );
  NTEST( "aAaA", "#*[A]", 4 );
  NTEST( "aAaA", "#*[a]", 4 );
  NTEST( "aAaA", "#*A-Z", 4 );
  NTEST( "aAaA", "A-Z#*", 4 );
  NTEST( "aAaA", "(a)#*", 4 );
  NTEST( "aAaA", "(A)#*", 4 );
  NTEST( "aAaA", "[a]#*", 4 );
  NTEST( "aAaA", "[A]#*", 4 );
  NTEST( "aAaA", "#*[A-Z]", 4 );
  NTEST( "aAaA", "[A-Z]#*", 4 );
  NTEST( "aAaA", "(a#*)", 4 );
  NTEST( "aAaA", "(A#*)", 4 );
  NTEST( "aAaA", "(a)#*", 4 );
  NTEST( "aAaA", "(A)#*", 4 );
  NTEST( "aAbB", "#*a|b", 4 );
  NTEST( "aAbB", "#*A|B", 4 );
  NTEST( "aAbB", "#*(a|b)", 4 );
  NTEST( "aAbB", "#*(A|B)", 4 );
  NTEST( "aAbB", "(a#*|b#*)", 4 );
  NTEST( "aAbB", "(A#*|B#*)", 4 );
  NTEST( "aAbB", "(a|b)#*", 4 );
  NTEST( "aAbB", "(A|B)#*", 4 );
  NTEST( "TesT", "test", 0 );
  NTEST( "TesT", "test#*", 0 );
  NTEST( "TesT", "t#*est#*", 1 );
  NTEST( "TesT", "#*test", 1 );
  NTEST( "TesT", "#*tESt", 1 );
  NTEST( "TesT", "#*(tESt)", 1 );
  NTEST( "TesT", "(tESt)#*", 1 );

  NTEST( "Raptor TesT Fest", "RapTor (tESt)#* fEST", 0 );
  NTEST( "Raptor TesT Fest", "#*rapTor (tESt) fEST", 1 );
  NTEST( "Raptor TesT Fest", "(RapTor)#* (tESt)#* (fEST)#*", 1 );
  NTEST( "Raptor TesT Fest", "((Rap#*Tor)#* (t#*ESt)#* (fEST)#*)#*", 1 );
  NTEST( "Raptor TesT Fest", "#*a-z#*apTor (tESt) A-Z#*EST", 1 );

  NTEST( "a", "a#/", 1 );
  NTEST( "a", "A#/", 0 );
  NTEST( "a", "#/A", 0 );
  NTEST( "a", "#/a", 1 );
  NTEST( "a", "#/(A)", 0 );
  NTEST( "a", "#/(a)", 1 );
  NTEST( "a", "#/[A]", 0 );
  NTEST( "a", "#/[a]", 1 );
  NTEST( "a", "#/A-Z", 0 );
  NTEST( "a", "A-Z#/", 0 );
  NTEST( "a", "(a)#/", 1 );
  NTEST( "a", "(A)#/", 0 );
  NTEST( "a", "[a]#/", 1 );
  NTEST( "a", "[A]#/", 0 );
  NTEST( "a", "#/[A-Z]", 0 );
  NTEST( "a", "[A-Z]#/", 0 );

  NTEST( "aAaA", "a#/", 2 );
  NTEST( "aAaA", "A#/", 2 );
  NTEST( "aAaA", "#/A", 2 );
  NTEST( "aAaA", "#/a", 2 );
  NTEST( "aAaA", "#/(A)", 2 );
  NTEST( "aAaA", "#/(a)", 2 );
  NTEST( "aAaA", "#/[A]", 2 );
  NTEST( "aAaA", "#/[a]", 2 );
  NTEST( "aAaA", "#/A-Z", 2 );
  NTEST( "aAaA", "A-Z#/", 2 );
  NTEST( "aAaA", "(a)#/", 2 );
  NTEST( "aAaA", "(A)#/", 2 );
  NTEST( "aAaA", "[a]#/", 2 );
  NTEST( "aAaA", "[A]#/", 2 );
  NTEST( "aAaA", "#/[A-Z]", 2 );
  NTEST( "aAaA", "[A-Z]#/", 2 );
  NTEST( "aAaA", "(a#/)", 2 );
  NTEST( "aAaA", "(A#/)", 2 );
  NTEST( "aAaA", "(a)#/", 2 );
  NTEST( "aAaA", "(A)#/", 2 );
  NTEST( "aAbB", "#/a|b", 2 );
  NTEST( "aAbB", "#/A|B", 2 );
  NTEST( "aAbB", "#/(a|b)", 2 );
  NTEST( "aAbB", "#/(A|B)", 2 );
  NTEST( "aAbB", "(a#/|b#/)", 2 );
  NTEST( "aAbB", "(A#/|B#/)", 2 );
  NTEST( "aAbB", "(a|b)#/", 2 );
  NTEST( "aAbB", "(A|B)#/", 2 );

  NTEST( "Raptor TesT Fest", "#*rapTor (tESt)#/ fEST", 0 );
  NTEST( "Raptor tESt Fest", "#*rapTor (tESt)#/ fEST", 1 );
  NTEST( "Raptor TesT Fest", "#*rapTor (tE#/S#/t)#* fEST", 0 );
  NTEST( "Raptor tESt Fest", "#*rapTor (tE#/S#/t)#* fEST", 1 );

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

  CATCH_TEST( "a", "<a>", 1, "a" );
  CATCH_TEST( "aa", "<aa>", 1, "aa" );
  CATCH_TEST( "a a a", "<a>", 3, "a" );
  CATCH_TEST( "abcd", "<a|b|c|d>", 1, "a" );
  CATCH_TEST( "abcd", "<a|b|c|d>", 2, "b" );
  CATCH_TEST( "abcd", "<a|b|c|d>", 3, "c" );
  CATCH_TEST( "abcd", "<a|b|c|d>", 4, "d" );
  CATCH_TEST( "abcd", "<a|b|c|d>", 5, "" );
  CATCH_TEST( "abc", "a<x>", 1, "" );
  CATCH_TEST( "abc", "<a>x", 1, "" );
  CATCH_TEST( "abc", "<a|b>x", 1, "" );
  CATCH_TEST( "abc", "<<a|b>x|abc>", 1, "abc" );
  CATCH_TEST( "abc", "<<a|b>x|abc>", 2, "" );
  CATCH_TEST( "abc", "<x<a|b>|abc>", 1, "abc" );
  CATCH_TEST( "abc", "<x<a|b>|abc>", 2, "" );
  CATCH_TEST( "abc abc abc", "<a|b|c>", 9, "c" );
  CATCH_TEST( "abc abc abc", "<(a|b|c)(a|b|c)(a|b|c)>", 3, "abc" );
  CATCH_TEST( "abc abc abc", "<(a|b|c)(a|b|c)(a|b|c)> ", 2, "abc" );
  CATCH_TEST( "abc abc abc", "#?<(a|b|c)(a|b|c)(a|b|c)>", 1, "abc" );
  CATCH_TEST( "abc abc abc", "#?<(a|b|c)(a|b|c)((a|b)|x)>", 1, "" );
  CATCH_TEST( "abc abc abx", "#?<(a|b|c)(a|b|c)((a|b)|x)>", 1, "abx" );
  CATCH_TEST( "abc iecc oeb", "<<(a|e)|(i|o)>e<b|c>|abc>", 1, "abc" );
  CATCH_TEST( "abc iecc oeb", "<<(a|e)|(i|o)>e<b|c>|abc>", 2, "iec" );
  CATCH_TEST( "abc iecc oeb", "<<(a|e)|(i|o)>e<b|c>|abc>", 3, "i" );
  CATCH_TEST( "abc iecc oeb", "<<(a|e)|(i|o)>e<b|c>|abc>", 4, "c" );
  CATCH_TEST( "abc iecc oeb", "<<(a|e)|(i|o)>e<b|c>|abc>", 5, "oeb" );
  CATCH_TEST( "abc iecc oeb", "<<(a|e)|(i|o)>e<b|c>|abc>", 6, "o" );
  CATCH_TEST( "abc iecc oeb", "<<(a|e)|(i|o)>e<b|c>|abc>", 7, "b" );
  CATCH_TEST( "abc iecc oeb", "<<(a|e)|(i|o)>e<b|c>|abc>", 8, "" );

  CATCH_TEST( "a", "<a>?", 1, "a" );
  CATCH_TEST( "a", "<b>?", 1, "" );
  CATCH_TEST( "a", "<a>+", 1, "a" );
  CATCH_TEST( "a", "<a>*", 1, "a" );
  CATCH_TEST( "a", "<b>*", 1, "" );
  CATCH_TEST( "aaa", "<a>+", 1, "aaa" );
  CATCH_TEST( "aaa", "<a>*", 1, "aaa" );
  CATCH_TEST( "aaa", "#~<a>+", 1, "aaa" );
  CATCH_TEST( "aaa", "#~<a>*", 1, "aaa" );
  CATCH_TEST( "aaab", "#~<a+>", 1, "aaa" );
  CATCH_TEST( "aaab", "#~<a*>", 1, "aaa" );
  CATCH_TEST( "aaab", "#~<a?>", 4, "" );
  CATCH_TEST( "aaab", "#~<a+b>", 1, "aaab" );
  CATCH_TEST( "aaab", "#~<a*b>", 1, "aaab" );
  CATCH_TEST( "aaab", "#~<a?b>", 1, "ab" );
  CATCH_TEST( "aaab", "#~<a+b?>", 1, "aaab" );
  CATCH_TEST( "aaab", "#~<a*b?>", 1, "aaab" );
  CATCH_TEST( "aaab", "#~<a?b?>", 3, "ab" );
  CATCH_TEST( "aaab", "#~<a+b+>", 1, "aaab" );
  CATCH_TEST( "aaab", "#~<a*b+>", 1, "aaab" );
  CATCH_TEST( "aaab", "#~<a?b+>", 1, "ab" );
  CATCH_TEST( "aaab", "#~<a+b*>", 1, "aaab" );
  CATCH_TEST( "aaab", "#~<a*b*>", 1, "aaab" );
  CATCH_TEST( "aaab", "#~<a?b*>", 3, "ab" );
  CATCH_TEST( "aaabaaa", "#~<a+>", 4, "aaa" );
  CATCH_TEST( "aaabaaa", "#~<a*>", 5, "aaa" );

  CATCH_TEST( "aaaaaaa", "<a+>", 1, "aaaaaaa" );
  CATCH_TEST( "aaaaaaa", "<:a+>", 1, "aaaaaaa" );
  CATCH_TEST( "aaaaaaa", "<:a*>", 1, "aaaaaaa" );
  CATCH_TEST( "aaaaaaa", "<:a{7}>", 1, "aaaaaaa" );
  CATCH_TEST( "aaaaaaa", "<:a{1,}>", 1, "aaaaaaa" );
  CATCH_TEST( "aaaaaaa", "<:a{1,7}>", 1, "aaaaaaa" );

  CATCH_TEST( "aaaaaaa", "<a>+", 1, "aaaaaaa" );
  CATCH_TEST( "aaaaaaa", "<:a>+", 1, "aaaaaaa" );
  CATCH_TEST( "aaaaaaa", "<:a>*", 1, "aaaaaaa" );
  CATCH_TEST( "aaaaaaa", "<:a>{7}", 1, "aaaaaaa" );
  CATCH_TEST( "aaaaaaa", "<:a>{1,}", 1, "aaaaaaa" );
  CATCH_TEST( "aaaaaaa", "<:a>{1,7}", 1, "aaaaaaa" );

  CATCH_TEST( "caaaaaaa", "<a+>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<a*>", 1, "" );
  CATCH_TEST( "caaaaaaa", "<a{7}>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<a{1,}>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<a{1,7}>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<a>+", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<a>*", 1, "" );
  CATCH_TEST( "caaaaaaa", "<a>{7}", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<a>{1,}", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<a>{1,7}", 1, "aaaaaaa" );

  CATCH_TEST( "caaaaaaa", "<:a+>", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<:a*>", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<:a{7}>", 1, "caaaaaa" );
  CATCH_TEST( "caaaaaaa", "<:a{1,}>", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<:a{1,7}>", 1, "caaaaaa" );

  CATCH_TEST( "caaaaaaa", "<:a>+", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<:a>*", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<:a>{7}", 1, "caaaaaa" );
  CATCH_TEST( "caaaaaaa", "<:a>{1,}", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<:a>{1,7}", 1, "caaaaaa" );

  CATCH_TEST( "caaaaaaa", "c<a+>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<a*>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<a{7}>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<a{1,}>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<a{1,7}>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<a>+", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<a>*", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<a>{7}", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<a>{1,}", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<a>{1,7}", 1, "aaaaaaa" );

  CATCH_TEST( "caaaaaaa", "c<:a+>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<:a*>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<:a{7}>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<:a{1,}>", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<:a{1,7}>", 1, "aaaaaaa" );

  CATCH_TEST( "caaaaaaa", "c<:a>+", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<:a>*", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<:a>{7}", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<:a>{1,}", 1, "aaaaaaa" );
  CATCH_TEST( "caaaaaaa", "c<:a>{1,7}", 1, "aaaaaaa" );

  CATCH_TEST( "caaaaaaa", "<ca+>", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<ca*>", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<ca{7}>", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<ca{1,}>", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<ca{1,7}>", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<ca>+", 1, "ca" );
  CATCH_TEST( "caaaaaaa", "<ca>*", 1, "ca" );
  CATCH_TEST( "caaaaaaa", "<ca>{7}", 1, "" );
  CATCH_TEST( "caaaaaaa", "<ca>{1,}", 1, "ca" );
  CATCH_TEST( "caaaaaaa", "<ca>{1,7}", 1, "ca" );

  CATCH_TEST( "caaaaaaa", "<c:a+>", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<c:a*>", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<c:a{7}>", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<c:a{1,}>", 1, "caaaaaaa" );
  CATCH_TEST( "caaaaaaa", "<c:a{1,7}>", 1, "caaaaaaa" );

  CATCH_TEST( "caaaaaaa", "<c:a>+", 1, "ca" );
  CATCH_TEST( "caaaaaaa", "<c:a>*", 1, "ca" );
  CATCH_TEST( "caaaaaaa", "<c:a>{7}", 1, "" );
  CATCH_TEST( "caaaaaaa", "<c:a>{1,}", 1, "ca" );
  CATCH_TEST( "caaaaaaa", "<c:a>{1,7}", 1, "ca" );

  CATCH_TEST( "Raptor Test",     "<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1, "Raptor Test" );
  CATCH_TEST( "Raptor Test",     "<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 2, "T" );
  CATCH_TEST( "Raaaaptor TFest", "<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1, "Raaaaptor TFest" );
  CATCH_TEST( "Raaaaptor TFest", "<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 2, "TF" );
  CATCH_TEST( "CaptorTest",      "<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1, "CaptorTest" );
  CATCH_TEST( "Cap CaptorTest",  "<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1, "Cap CaptorTest" );
  CATCH_TEST( "Cap CaptorTest",  "#~<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 3, "CaptorTest" );
  CATCH_TEST( "Rap Captor Fest", "<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 1, "Rap Captor Fest" );
  CATCH_TEST( "Rap Captor Fest", "#~<((C|R)ap C|C|R)(a+p{1}tor) ?((<T|F>+e)(st))>", 3, "Captor Fest" );

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

  REPLACE_TEST( "a", "<a>"   , 1, "", "" );
  REPLACE_TEST( "a", "<a>?"  , 1, "", "" );
  REPLACE_TEST( "a", "<a>+"  , 1, "", "" );
  REPLACE_TEST( "a", "<a>*"  , 1, "", "" );
  REPLACE_TEST( "a", "<a>{1}", 1, "", "" );
  REPLACE_TEST( "a", "<a?>"  , 1, "", "" );
  REPLACE_TEST( "a", "<a+>"  , 1, "", "" );
  REPLACE_TEST( "a", "<a*>"  , 1, "", "" );
  REPLACE_TEST( "a", "<a{1}>", 1, "", "" );

  REPLACE_TEST( "a", "<a>"   , 1, "e", "e" );
  REPLACE_TEST( "a", "<a>?"  , 1, "e", "e" );
  REPLACE_TEST( "a", "<a>+"  , 1, "e", "e" );
  REPLACE_TEST( "a", "<a>*"  , 1, "e", "e" );
  REPLACE_TEST( "a", "<a>{1}", 1, "e", "e" );
  REPLACE_TEST( "a", "<a?>"  , 1, "e", "e" );
  REPLACE_TEST( "a", "<a+>"  , 1, "e", "e" );
  REPLACE_TEST( "a", "<a*>"  , 1, "e", "e" );
  REPLACE_TEST( "a", "<a{1}>", 1, "e", "e" );

  REPLACE_TEST( "a", "<x>"   , 1, "z", "a" );
  REPLACE_TEST( "a", "<x>?"  , 1, "z", "za" );
  REPLACE_TEST( "a", "<x>+"  , 1, "z", "a" );
  REPLACE_TEST( "a", "<x>*"  , 1, "z", "za" );
  REPLACE_TEST( "a", "<x>{1}", 1, "z", "a" );
  REPLACE_TEST( "a", "<x?>"  , 1, "z", "za" );
  REPLACE_TEST( "a", "<x+>"  , 1, "z", "a" );
  REPLACE_TEST( "a", "<x*>"  , 1, "z", "za" );
  REPLACE_TEST( "a", "<x{1}>", 1, "z", "a" );

  REPLACE_TEST( "aaa", "<a>"   , 1, "", "" );
  REPLACE_TEST( "aaa", "<a>?"  , 1, "", "" );
  REPLACE_TEST( "aaa", "<a>+"  , 1, "", "" );
  REPLACE_TEST( "aaa", "<a>*"  , 1, "", "" );
  REPLACE_TEST( "aaa", "<a>{1}", 1, "", "" );
  REPLACE_TEST( "aaa", "<a?>"  , 1, "", "" );
  REPLACE_TEST( "aaa", "<a+>"  , 1, "", "" );
  REPLACE_TEST( "aaa", "<a*>"  , 1, "", "" );
  REPLACE_TEST( "aaa", "<a{1}>", 1, "", "" );

  REPLACE_TEST( "aaa", "#~<a>+", 1, "ke", "kekeke" );
  REPLACE_TEST( "aaa", "#~<:a>+", 1, "12345", "123451234512345" );
  REPLACE_TEST( "Emacs Rocks", "#~<.+>"   , 1, "1", "11111111111" );
  REPLACE_TEST( "Aya Test", "#~<Test>+"   , 1, "II", "Aya II" );
  NTEST( "Raptor Test Raptor", "Ra+pto+r", 2 );

  REPLACE_TEST( "aaa", "<a>"   , 1, "e", "eee" );
  REPLACE_TEST( "aaa", "<a>?"  , 1, "e", "eee" );
  REPLACE_TEST( "aaa", "<a>+"  , 1, "e", "e" );
  REPLACE_TEST( "aaa", "<a>*"  , 1, "e", "e" );
  REPLACE_TEST( "aaa", "<a>{1}", 1, "e", "eee" );
  REPLACE_TEST( "aaa", "<a?>"  , 1, "e", "eee" );
  REPLACE_TEST( "aaa", "<a+>"  , 1, "e", "e" );
  REPLACE_TEST( "aaa", "<a*>"  , 1, "e", "e" );
  REPLACE_TEST( "aaa", "<a{1}>", 1, "e", "eee" );

  REPLACE_TEST( "aaa", "<x>"   , 1, "z", "aaa" );
  REPLACE_TEST( "aaa", "<x>?"  , 1, "z", "zazaza" );
  REPLACE_TEST( "aaa", "<x>+"  , 1, "z", "aaa" );
  REPLACE_TEST( "aaa", "<x>*"  , 1, "z", "zazaza" );
  REPLACE_TEST( "aaa", "<x>{1}", 1, "z", "aaa" );
  REPLACE_TEST( "aaa", "<x?>"  , 1, "z", "zazaza" );
  REPLACE_TEST( "aaa", "<x+>"  , 1, "z", "aaa" );
  REPLACE_TEST( "aaa", "<x*>"  , 1, "z", "zazaza" );
  REPLACE_TEST( "aaa", "<x{1}>", 1, "z", "aaa" );

  REPLACE_TEST( "aaab", "<a>"   , 1, "e", "eeeb" );
  REPLACE_TEST( "aaab", "<a>?"  , 1, "e", "eeeeb" );
  REPLACE_TEST( "aaab", "<a>+"  , 1, "e", "eb" );
  REPLACE_TEST( "aaab", "<a>*"  , 1, "e", "eeb" );
  REPLACE_TEST( "aaab", "<a>{1}", 1, "e", "eeeb" );
  REPLACE_TEST( "aaab", "<a?>"  , 1, "e", "eeeeb" );
  REPLACE_TEST( "aaab", "<a+>"  , 1, "e", "eb" );
  REPLACE_TEST( "aaab", "<a*>"  , 1, "e", "eeb" );
  REPLACE_TEST( "aaab", "<a{1}>", 1, "e", "eeeb" );

  REPLACE_TEST( "aaab", "<x>"   , 1, "z", "aaab" );
  REPLACE_TEST( "aaab", "<x>?"  , 1, "z", "zazazazb" );
  REPLACE_TEST( "aaab", "<x>+"  , 1, "z", "aaab" );
  REPLACE_TEST( "aaab", "<x>*"  , 1, "z", "zazazazb" );
  REPLACE_TEST( "aaab", "<x>{1}", 1, "z", "aaab" );
  REPLACE_TEST( "aaab", "<x?>"  , 1, "z", "zazazazb" );
  REPLACE_TEST( "aaab", "<x+>"  , 1, "z", "aaab" );
  REPLACE_TEST( "aaab", "<x*>"  , 1, "z", "zazazazb" );
  REPLACE_TEST( "aaab", "<x{1}>", 1, "z", "aaab" );

  REPLACE_TEST( "aaabaaa", "<a>"   , 1, "e", "eeebeee" );
  REPLACE_TEST( "aaabaaa", "<a>?"  , 1, "e", "eeeebeee" );
  REPLACE_TEST( "aaabaaa", "<a>+"  , 1, "e", "ebe" );
  REPLACE_TEST( "aaabaaa", "<a>*"  , 1, "e", "eebe" );
  REPLACE_TEST( "aaabaaa", "<a>{1}", 1, "e", "eeebeee" );
  REPLACE_TEST( "aaabaaa", "<a?>"  , 1, "e", "eeeebeee" );
  REPLACE_TEST( "aaabaaa", "<a+>"  , 1, "e", "ebe" );
  REPLACE_TEST( "aaabaaa", "<a*>"  , 1, "e", "eebe" );
  REPLACE_TEST( "aaabaaa", "<a{1}>", 1, "e", "eeebeee" );

  REPLACE_TEST( "aaabaaa", "<x>"   , 1, "z", "aaabaaa" );
  REPLACE_TEST( "aaabaaa", "<x>?"  , 1, "z", "zazazazbzazaza" );
  REPLACE_TEST( "aaabaaa", "<x>+"  , 1, "z", "aaabaaa" );
  REPLACE_TEST( "aaabaaa", "<x>*"  , 1, "z", "zazazazbzazaza" );
  REPLACE_TEST( "aaabaaa", "<x>{1}", 1, "z", "aaabaaa" );
  REPLACE_TEST( "aaabaaa", "<x?>"  , 1, "z", "zazazazbzazaza" );
  REPLACE_TEST( "aaabaaa", "<x+>"  , 1, "z", "aaabaaa" );
  REPLACE_TEST( "aaabaaa", "<x*>"  , 1, "z", "zazazazbzazaza" );
  REPLACE_TEST( "aaabaaa", "<x{1}>", 1, "z", "aaabaaa" );


  REPLACE_TEST( "Raptor Test", "<Raptor>", 1, "Captor", "Captor Test"   );
  REPLACE_TEST( "Raptor Test", "<Raptor>", 0, "Captor", "Raptor Test"   );
  REPLACE_TEST( "Raptor Test", "<Raptor|Test>", 0, "Captor", "Raptor Test"   );
  REPLACE_TEST( "Raptor Test", "<Raptor|Test>", 1, "Captor", "Captor Captor"   );
  REPLACE_TEST( "Raptor Test", "<Raptor|Test>", 2, "Captor", "Raptor Test"   );
  REPLACE_TEST( "Raptor Test", "<Raptor|<Test>>", 2, "Fest", "Raptor Fest"   );
  REPLACE_TEST( "Raptor Raptors Raptoring", "<Raptor:w*>", 1, "Test", "Test Test Test" );
  REPLACE_TEST( "Raptor Raptors Raptoring", "<Raptor>:w*", 1, "Test", "Test Tests Testing" );
  REPLACE_TEST( "Raptor Raptors Raptoring", "<<<R>a>ptor>:w*", 3, "C", "Captor Captors Captoring" );
  REPLACE_TEST( "Raptor Raptors Raptoring", "<<<R>a>ptor>:w*", 2, "4", "4ptor 4ptors 4ptoring" );

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

  PUT_TEST( "a", "<a>", "#1", "a" );
  PUT_TEST( "a", "<a>", "#x", "x" );
  PUT_TEST( "a", "<a>", "#xx", "xx" );
  PUT_TEST( "a", "<a>", "###1##", "#a#" );
  PUT_TEST( "a", "<a>", "[#0][#1][#2#3#1000000]", "[][a][]" );
  PUT_TEST( "aa", "<aa>", "#1", "aa" );
  PUT_TEST( "a a a", "<a>", "#1#2#3", "aaa" );
  PUT_TEST( "abcd", "<a|b|c|d>", "#4 #3 #2 #1", "d c b a" );
  PUT_TEST( "1 2 3 4 5 6 7 8 9", "<1|2|3|4|5|6|7|8|9>", "#5 #6 #7 #8 #9 #1 #2 #3 #4", "5 6 7 8 9 1 2 3 4" );
  PUT_TEST( "Raptor Test", "<aptor|est>", "C#1 F#2", "Captor Fest" );
  PUT_TEST( "Raptor Test", "<aptor|est>", "C#5 F#2", "C Fest" );
  PUT_TEST( "Raptor Test", "<aptor|est>", "C#a F#2", "Ca Fest" );
  PUT_TEST( "Raptor Test", "<aptor|est>", "C#0 F#2", "C Fest" );
  PUT_TEST( "Raptor Test", "<aptor|est>", "C#43 F#43", "C F" );
  PUT_TEST( "Raptor Test", "<aptor|est>", "C##43 ##F#43##", "C#43 #F#" );
  PUT_TEST( "Raptor Test", "<aptor|est>", "C##43 ##1##2", "C#43 #1#2" );
  PUT_TEST( "Raptor Test", "<aptor|est>", "##Raptor ##Test", "#Raptor #Test" );
  PUT_TEST( "Raptor Test Fest", "<Raptor> <Test>", "#1_#2", "Raptor_Test" );

  PUT_TEST( "123456789", "#$<<:d><:d>>", "[#2] [#3] [#1]", "[8] [9] [89]" );

  PUT_TEST( "aa0123aa", "<aa><[(...)]><aa>", "[#1][#2][#3]", "[aa][0123][aa]" );
  PUT_TEST( "aa0123aa", "<aa><[(.)]>{2}<aa>", "[#1][#2][#3]", "[aa][0123][aa]" );
  PUT_TEST( "aa0123aa", "<aa><[(.)]{2}><aa>", "[#1][#2][#3]", "[aa][0123][aa]" );
  PUT_TEST( "aa0123aa", "<aa><[(.{3})]><aa>", "[#1][#2][#3]", "[aa][0123][aa]" );
  PUT_TEST( "aa0123aa", "<aa><[(.)*]><23aa>", "[#1][#2][#3]", "[aa][01][23aa]" );
  PUT_TEST( "aaxy0aa", "<aa><[(xy)]><aa>", "[#1][#2][#3]", "[aa][xy0][aa]" );

  PUT_TEST( "a123456789b", "<a>([^(b)])*<b>", "[#1][#2]", "[a][b]" );
  PUT_TEST( "a123456789b", "<a>([^(b)])+<b>", "[#1][#2]", "[a][b]" );
  PUT_TEST( "a123456789b", "<a><[^(b)]>*<b>", "[#1][#2][#3]", "[a][123456789][b]" );
  PUT_TEST( "a123456789b", "<a><[^(b)]>+<b>", "[#1][#2][#3]", "[a][123456789][b]" );
  PUT_TEST( "aa0123aa", "<aa>([^(aa)])*<aa>", "[#1][#2]", "[aa][aa]" );
  PUT_TEST( "aa0123aa", "<aa><[^(aa)]>*<aa>", "[#1][#2][#3]", "[aa][0123][aa]" );
  PUT_TEST( "aa0123aa", "<aa><[^(aa)]>+<aa>", "[#1][#2][#3]", "[aa][0123][aa]" );
  PUT_TEST( "aa0123aa", "<aa><[^(aa|(B|C)B)]>*<aa|(B|C)B>", "[#1][#2][#3]", "[aa][0123][aa]" );
  PUT_TEST( "aa0123BB", "<aa><[^(aa|(B|C)B)]>*<aa|(B|C)B>", "[#1][#2][#3]", "[aa][0123][BB]" );
  PUT_TEST( "aa0123CB", "<aa><[^(aa|(B|C)B)]>*<aa|(B|C)B>", "[#1][#2][#3]", "[aa][0123][CB]" );


  PUT_TEST( "make a new Line", "#^<[^n]+>", "#1raptor", "make a raptor" );
  PUT_TEST( "07-07-1777", "<0?[1-9]|[12][0-9]|3[01]>[/:-\\]<0?[1-9]|1[012]>[/:-\\]<[12][0-9]{3}>", "d:#1 m:#2 y:#3", "d:07 m:07 y:1777" );
  PUT_TEST( "fecha: 07-07-1777", "<0?[1-9]|[12][0-9]|3[01]>[/:-\\]<0?[1-9]|1[012]>[/:-\\]<[12][0-9]{3}>", "d:#1 m:#2 y:#3", "d:07 m:07 y:1777" );
  FALSE_TEST( "31-13-1331", "<0?[1-9]|[12][0-9]|3[01]>[/:-\\]<0?[1-9]|1[012]>[/:-\\]<[12][0-9]{3}>" );
  FALSE_TEST( "71-17-1177", "<0?[1-9]|[12][0-9]|3[01]>[/:-\\]<0?[1-9]|1[012]>[/:-\\]<[12][0-9]{3}>" );
  PUT_TEST( "07-07-1777", "<0?[1-9]|[12][0-9]|3[01]><[/:-\\]><0?[1-9]|1[012]>@2<[12][0-9]{3}>", "d:#1 m:#3 y:#4", "d:07 m:07 y:1777" );
  PUT_TEST( "fecha: 07-07-1777", "<0?[1-9]|[12][0-9]|3[01]><[/:-\\]><0?[1-9]|1[012]>@2<[12][0-9]{3}>", "d:#1 m:#3 y:#4", "d:07 m:07 y:1777" );
  FALSE_TEST( "31/13-1331", "<0?[1-9]|[12][0-9]|3[01]><[/:-\\]><0?[1-9]|1[012]>@2<[12][0-9]{3}>" );
  FALSE_TEST( "71-17/1177", "<0?[1-9]|[12][0-9]|3[01]><[/:-\\]><0?[1-9]|1[012]>@2<[12][0-9]{3}>" );
  PUT_TEST( "nasciiboy@gmail.com", "<[_A-Za-z0-9:-]+(:.[_A-Za-z0-9:-]+)*>:@<[A-Za-z0-9]+>:.<[A-Za-z0-9]+><:.[A-Za-z0-9]{2}>*", "[#1][#2][#3]", "[nasciiboy][gmail][com]" );
  PUT_TEST( "<mail>nasciiboy@gmail.com</mail>", "<[_A-Za-z0-9:-]+(:.[_A-Za-z0-9:-]+)*>:@<[A-Za-z0-9]+>:.<[A-Za-z0-9]+><:.[A-Za-z0-9]{2}>*", "[#1][#2][#3]", "[nasciiboy][gmail][com]" );
  PUT_TEST( "u.s.r_43@ru.com.jp", "<[_A-Za-z0-9:-]+(:.[_A-Za-z0-9:-]+)*>:@<[A-Za-z0-9]+>:.<[A-Za-z0-9]+><:.[A-Za-z0-9]{2}>*", "[#1][#2][#3]", "[u.s.r_43][ru][com]" );
  PUT_TEST( "https://en.wikipedia.org/wiki/Regular_expression", "(https?|ftp):://<[^:s/:<:>]+></[^:s:.:<:>,/]+>*<.>*", "[#1][#2][#3]", "[en.wikipedia.org][/wiki/Regular_expression][]" );
  PUT_TEST( "https://en.wikipedia.org/wiki/Regular_expression.test", "(https?|ftp):://<[^:s/:<:>]+></[^:s:.:<:>,/]+>*<.>*", "[#1][#2][#3]", "[en.wikipedia.org][/wiki/Regular_expression][.test]" );
  PUT_TEST( "https://en.wikipedia.org/wiki/Regular_expression,test", "(https?|ftp):://<[^:s/:<:>]+></[^:s:.:<:>,/]+>*<.>*", "[#1][#2][#3]", "[en.wikipedia.org][/wiki/Regular_expression][,test]" );
  PUT_TEST( "<url>https://en.wikipedia.org/wiki/Regular_expression<url>", "(https?|ftp):://<[^:s/:<:>]+></[^:s:.:<:>,/]+>*<.>*", "[#1][#2][#3]", "[en.wikipedia.org][/wiki/Regular_expression][<url>]" );

  printf( "TEST %d *** ERRS %d\n\n", total, errs );
  return !errs;
}

int main(){
  if( raptorTest() ) puts ("I am the Raptor" );

  return 0;
}
