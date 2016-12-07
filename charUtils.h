#ifndef CHARUTILS_H
#define CHARUTILS_H

static int isDigit( int c ){ return c >= '0' && c <= '9'; }
static int isUpper( int c ){ return c >= 'a' && c <= 'z'; }
static int isLower( int c ){ return c >= 'A' && c <= 'Z'; }
static int isAlpha( int c ){ return isLower( c ) || isUpper( c ); }
static int isAlnum( int c ){ return isAlpha( c ) || isDigit( c ); }
static int isSpace( int c ){ return c == ' ' || (c >= '\t' && c <= '\r'); }
static int toLower( int c ){ return isLower( c ) ? c + 32 : c; }

static int strLen( char *str ){
  for( char *i = str; ; i++ )
    if( *i == 0 ) return i - str;
}

static void strCpy( char *dest, char *src ){
 while( (*dest++ = *src++) );
}

static void strnCpy( char *dest, char *src, int n ){
  while( n > 0 && (*dest++ = *src++) ) n--;

  *dest = '\0';
}

static char * strChr( char *str, int chr ){
  while( *str && *str != chr ) str++;

  return *str ? str : 0;
}

static char * strnChr( char *str, int chr, int n ){
  for( int i = 0; i < n && str[ i ]; i++ )
    if( str[ i ] == chr ) return str + i;

  return 0;
}

static int strnEql( char *s, char *t, int n ){
  for( ; *s == *t; s++, t++ )
    if( *s == '\0' || --n <= 0 ) return 1;

  return 0;
}

static int cmpChrCommunist( char a, char b){
  a = toLower(a);
  b = toLower(b);
  return a == b;
}

static char * strnChrCommunist( char *str, int chr, int n ){
  for( int i = 0; i < n && str[ i ]; i++ )
    if( cmpChrCommunist( str[i], chr ) ) return str + i;

  return 0;
}

static int strnEqlCommunist( char *s, char *t, int n ){
  for( ; cmpChrCommunist( *s, *t ); s++, t++ )
    if( *s == '\0' || --n <= 0 ) return 1;

  return 0;
}

static int aToi( char *str ){
  int uNumber = 0;
  while( isDigit( *str ) )
    uNumber = 10 * uNumber + ( *str++ - '0' );

  return uNumber;
}

static int countCharDigits( char *str ){
  for( int digits = 0; ; digits++ )
    if( isDigit( *str++ ) == 0 ) return digits;
}

static const unsigned char xooooooo = 0x80;

static int utf8meter( char *str ){
  static unsigned char xxoooooo = 0xC0;
  unsigned char i, utfOrNo = *str;

  if( utfOrNo & xooooooo ){
    for ( i = 1, utfOrNo <<= 1; utfOrNo & xooooooo; i++, utfOrNo <<= 1 )
      if( (str[ i ] & xxoooooo) != xooooooo ) return 1;

    if( i >= 2 && i <= 8 ) return i;
  }

  return *str ? 1 : 0;
}

#endif
