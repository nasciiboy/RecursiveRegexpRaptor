#ifndef CHARUTILS_H
#define CHARUTILS_H

static int isDigit( const int c ){ return c >= '0' && c <= '9'; }
static int isUpper( const int c ){ return c >= 'a' && c <= 'z'; }
static int isLower( const int c ){ return c >= 'A' && c <= 'Z'; }
static int isAlpha( const int c ){ return isLower( c ) || isUpper( c ); }
static int isAlnum( const int c ){ return isAlpha( c ) || isDigit( c ); }
static int isSpace( const int c ){ return c == ' ' || (c >= '\t' && c <= '\r'); }
static int toLower( const int c ){ return isLower( c ) ? c + 32 : c; }

static int strLen( const char *str ){
  for( const char *i = str; ; i++ )
    if( *i == 0 ) return i - str;
}

static void strCpy( char *dest, const char *src ){
 while( (*dest++ = *src++) );
}

static void strnCpy( char *dest, const char *src, int n ){
  while( n > 0 && (*dest++ = *src++) ) n--;

  *dest = '\0';
}

static const char * strnChr( const char *str, const int chr, const int n ){
  for( int i = 0; i < n && str[ i ]; i++ )
    if( str[ i ] == chr ) return str + i;

  return 0;
}

static int strnEql( const char *s, const char *t, int n ){
  for( ; *s == *t; s++, t++ )
    if( *s == '\0' || --n <= 0 ) return 1;

  return 0;
}

static int cmpChrCommunist( const char a, const char b){
  return toLower(a) == toLower(b);
}

static const char * strnChrCommunist( const char *str, const int chr, const int n ){
  for( int i = 0; i < n && str[ i ]; i++ )
    if( cmpChrCommunist( str[i], chr ) ) return str + i;

  return 0;
}

static int strnEqlCommunist( const char *s, const char *t, int n ){
  for( ; cmpChrCommunist( *s, *t ); s++, t++ )
    if( *s == '\0' || --n <= 0 ) return 1;

  return 0;
}

static int aToi( const char *str ){
  int uNumber = 0;
  while( isDigit( *str ) )
    uNumber = 10 * uNumber + ( *str++ - '0' );

  return uNumber;
}

static int countCharDigits( const char *str ){
  for( int digits = 0; ; digits++ )
    if( isDigit( *str++ ) == 0 ) return digits;
}

#endif
