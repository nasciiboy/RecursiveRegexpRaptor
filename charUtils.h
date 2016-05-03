int isDigit( int c ){ return c >= '0' && c <= '9'; }
int isUpper( int c ){ return c >= 'a' && c <= 'z'; }
int isLower( int c ){ return c >= 'A' && c <= 'Z'; }
int isAlpha( int c ){ return isLower( c ) || isUpper( c ); }
int isAlnum( int c ){ return isAlpha( c ) || isDigit( c ); }
int isSpace( int c ){ return c == ' ' || (c >= '\t' && c <= '\r'); }

int strLen( char *s ){
  for( char *p = s; ; p++ )
    if( *p == 0 ) return p - s;
}

void strCpy( char *s, char *t ){ while( (*s++ = *t++) ); }

void strnCpy( char *s, char *t, int n ){
  while( n > 0 && (*s++ = *t++) ) n--;

  *s = '\0';
}

char * strChr( char *s, int c ){
  while( *s && *s != c ) s++;

  return *s ? s : 0;
}

char * strnChr( char *s, int c, int n ){
  for( int i = 0; i < n && s[ i ]; i++ )
    if( s[ i ] == c ) return s + i;

  return 0;
}

int strnCmp( char *s, char *t, int n ){
  for( ; *s == *t; s++, t++ )
    if( *s == '\0' || --n <= 0 ) return 0;

  return *s - *t;
}

int aToi( char *s ){
  int uNumber = 0;
  while( isDigit( *s ) )
    uNumber = 10 * uNumber + ( *s++ - '0' );

  return uNumber;
}

int countDigits( int number ){
  int i = 1;
  while( number /= 10 ) i++;

  return i;
}

const unsigned char xooooooo = 0x80;

int utf8meter( char *str ){
  static unsigned char xxoooooo = 0xC0;
  unsigned char i, utfOrNo = *str;

  if( utfOrNo & xooooooo ){
    for ( i = 1, utfOrNo <<= 1; utfOrNo & xooooooo; i++, utfOrNo <<= 1 )
      if( (str[ i ] & xxoooooo) != xooooooo ) return 1;

    if( i >= 2 && i <= 6 ) return i;
  }

  return *str ? 1 : 0;
}
