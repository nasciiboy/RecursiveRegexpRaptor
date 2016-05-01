int isDigit( int c ){ return c >= '0' && c <= '9'; }
int isUpper( int c ){ return c >= 'a' && c <= 'z'; }
int isLower( int c ){ return c >= 'A' && c <= 'Z'; }
int isAlpha( int c ){ return isLower( c ) || isUpper( c ); }
int isAlnum( int c ){ return isAlpha( c ) || isDigit( c ); }
int isSpace( int c ){ return c == ' ' || (c >= '\t' && c <= '\r'); }

int strLen( char *s ){
  char *p = s;
  while( *p ) p++;

  return p - s;
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

#include "regexp3.h"

#define TRUE       1
#define FALSE      0
#define INF    65536
#define CATCHS    24

struct CATch {
  char *ptr[CATCHS];
  int   len[CATCHS];
  int   id [CATCHS];
  int   idx;
  int   index;
} Catch;

struct PathLine {
  char *line;
  int   pos;
  int   len;
};

enum PTYPE { PATH, HOOK, GROUP, SIMPLE, BRACKET, RANGEAB, META, POINT, UTF8 };

struct Path {
  int   len;
  char *ptr;
  enum  PTYPE type;
  int   loopsMin, loopsMax;
};

int   walker      ( struct Path   path,   struct PathLine *pathLine              );
int   cutTrack    ( struct Path  *path,   struct Path     *track,     int type   );
int   trekking    ( struct Path  *path,   struct PathLine *pathLine              );
int   tracker     ( struct Path  *path,   struct Path     *track                 );
int   walkMeta    (                              char     *str                   );
int   walkBracket (                              char     *str                   );
int   isPath      ( struct Path *track                                           );
void  setLoops    ( struct Path *track,   struct Path     *path                  );
void  openCatch   ( struct Path *track,   struct PathLine *pathLine,  int *index );
void  closeCatch  ( struct Path *track,   struct PathLine *pathLine,  int  index );
void  delCatch    ( struct Path *track,                               int  index );
int   CatchIndex  (                                                              );
char *getCatch    (                              char     *lineCatch, int  index );
char *replaceCatch(        char *newLine,        char     *str,       int  index );
char *newLineCatch(        char *newLine,        char     *str                   );
int   match       ( struct Path *text,    struct PathLine *pathLine              );
int   matchBracket( struct Path *text,    struct PathLine *pathLine              );
int   matchMeta   ( struct Path *text,           char     *line                  );
int   matchText   ( struct Path *text,           char     *line                  );

int regexp3( char *line, char *exp ){
  struct Path     path;
  struct PathLine pathLine;
  int atTheEnd    = FALSE;
  int result      = 0;
  int loops       = strLen( line );
  path.ptr        = exp;
  path.len        = strLen( exp );
  path.type       = PATH;
  Catch.index     = 1;
  Catch.ptr[0]    = line;
  Catch.len[0]    = loops;

  if( *(exp + path.len - 1) == '$' ){
    atTheEnd = TRUE;
    path.len--;
  }

  if( *exp == '^' ){
    path.ptr++;
    path.len--;
    loops = 1;
  }

  for( int i = 0; i < loops; i += utf8meter( line + i ) ){
    Catch.idx     = 1;
    pathLine.pos  = 0;
    pathLine.line = line + i;
    pathLine.len  = Catch.len[0] - i;

    if( atTheEnd ){
      result = walker( path, &pathLine );

      if( result && pathLine.pos == pathLine.len ) i = loops;
      else result = 0;
    } else result += walker( path, &pathLine );
  }

  return result;
}

int walker( struct Path path, struct PathLine *pathLine ){
  struct Path track;
  while( cutTrack( &path, &track, PATH ) )
    if( track.len && trekking( &track, pathLine ) ) return TRUE;

  return FALSE;
}

int cutTrack( struct Path *path, struct Path *track, int type ){
  track->ptr  = path->ptr + (type == PATH ? 0 : 1);
  track->len  = path->len;
  track->type = path->type;

  for( int cut, i = 0, deep = 0; i < path->len; i++ ){
    i += walkMeta   ( path->ptr + i );
    i += walkBracket( path->ptr + i );

    switch( path->ptr[i] ){
    case '<': case '(': deep++; break;
    case '>': case ')': deep--; break;
    }

    switch( type ){
    case HOOK    : cut = deep == 0; break;
    case GROUP   : cut = deep == 0; break;
    case PATH    : cut = deep == 0 && path->ptr[i] == '|'; break;
    case BRACKET : cut = path->ptr[i] == ']'; break;
    }

    if( cut && i < path->len){
      track->len  = &path->ptr[i] - track->ptr;
      path->ptr  += i + 1;
      path->len  -= i + 1;
      track->type = type;
      return TRUE;
    }
  }

  path->ptr += path->len;
  path->len = 0;

  return track->len ? TRUE : FALSE;
}

int trekking( struct Path *path, struct PathLine *pathLine ){
  struct Path track;
  int loops, len, iCatch, oPos = pathLine->pos;

  while( tracker( path, &track ) ){
    openCatch( &track, pathLine, &iCatch );

    if( isPath( &track ) )
      for( loops = 0; loops < track.loopsMax && walker( track, pathLine ); )
        loops++;
    else
      for( loops = 0; loops < track.loopsMax && pathLine->pos < pathLine->len
                                             && (len = match( &track, pathLine )); ){
        pathLine->pos += len;
        loops++;
      }

    if( loops < track.loopsMin ){
      pathLine->pos = oPos;
      delCatch( &track, iCatch );
      return FALSE;
    } else closeCatch( &track, pathLine, iCatch );
  }

  return TRUE;
}

void trackByLen( struct Path *path, struct Path *track, int len, int type ){
  track->ptr   = path->ptr;
  track->type  = type;
  track->len   = len;
  path->ptr   += len;
  path->len   -= len;
}

char * trackerPoint( char *ct, int len ){
  for( int i = 0; i < len && ct[ i ]; i++ )
    if( strChr( ".[(<\\?+*{-", ct[ i ] ) || ct[ i ] & xooooooo ) return ct + i;

  return 0;
}

int tracker( struct Path *path, struct Path *track ){
  if( path->len ){
    char *point;

    switch( *path->ptr & xooooooo ? UTF8 : *path->ptr ){
    case '\\': trackByLen( path, track,                    2, META    ); break;
    case '.' : trackByLen( path, track,                    1, POINT   ); break;
    case '(' : cutTrack  ( path, track,                       GROUP   ); break;
    case '<' : cutTrack  ( path, track,                       HOOK    ); break;
    case '[' : cutTrack  ( path, track,                       BRACKET ); break;
    case UTF8: trackByLen( path, track, utf8meter(path->ptr), UTF8    ); break;
    default:
      if( (point = trackerPoint( path->ptr + 1, path->len - 1 )) ){
        switch( *point ){
        default: trackByLen( path, track, point - path->ptr, SIMPLE  ); break;
        case '?': case '+': case '*': case '{': case '-':
          if( point - path->ptr == 1 ){
            if( *point == '-' ) trackByLen( path, track, 3, RANGEAB );
            else                trackByLen( path, track, 1, SIMPLE  );
          } else trackByLen( path, track, (point - path->ptr) - 1, SIMPLE  );
        }
      } else trackByLen( path, track, path->len, SIMPLE  );
    }

    setLoops( track, path );
    return TRUE;
  }

  return FALSE;
}

int walkMeta( char *str ){
  int i = 0;
  while( str[i] == '\\' ) i += 2;

  return i;
}

int walkBracket( char *str ){
  int i = 0;
  if( *str == '[' )
    while( str[i] != ']' ) i++;

  return i;
}

int isPath( struct Path *track ){
  switch( track->type ){
  case PATH: case HOOK: case GROUP:
    for( int i = 0; i < track->len; i++ )
      if( strChr( "|(<[?+*{-\\", track->ptr[i] ) )
        return TRUE;
  default: return FALSE;
  }
}

void setLoops( struct Path *track, struct Path *path ){
  track->loopsMin = 1; track->loopsMax = 1;
  int len = 0;

  if( path->len )
    switch( *path->ptr ){
    case '?' : len = 1; track->loopsMin = 0; track->loopsMax =   1; break;
    case '+' : len = 1; track->loopsMin = 1; track->loopsMax = INF; break;
    case '*' : len = 1; track->loopsMin = 0; track->loopsMax = INF; break;
    case '{' :
      track->loopsMin = aToi( path->ptr + 1 ) ;
      if( path->ptr[ 1 + countDigits( track->loopsMin ) ] == ',' )
        track->loopsMax = aToi( strChr( path->ptr, ',' ) + 1 );
      else
        track->loopsMax = track->loopsMin;

      len = strChr( path->ptr, '}' ) - path->ptr + 1;
    }

  path->ptr += len;
  path->len -= len;
}

void openCatch( struct Path *track, struct PathLine *pathLine, int *index ){
  if( track->type == HOOK && Catch.index < CATCHS ){
    *index = Catch.index++;
    Catch.ptr[ *index ] = pathLine->line + pathLine->pos;
    Catch.len[ *index ] = 0;
    Catch.id [ *index ] = Catch.idx++;
  } else *index = CATCHS;
}

void closeCatch( struct Path *track, struct PathLine *pathLine, int index ){
  if( track->type == HOOK && index < CATCHS )
    Catch.len[ index ] = &pathLine->line[ pathLine->pos ] - Catch.ptr[ index ];
}

void delCatch( struct Path *track, int index ){
  if( track->type == HOOK && index < CATCHS ){
    Catch.index = index;
    Catch.ptr[ index ] = 0;
    Catch.len[ index ] = 0;
    Catch.idx = Catch.id[ index ];
  }
}

int indexCatch(){ return Catch.index - 1; }

char * getCatch( char * lineCatch, int index ){
  if( index > 0 && index < Catch.index )
    strnCpy( lineCatch, Catch.ptr[ index ], Catch.len[ index ] );
  else *lineCatch = '\0';

  return lineCatch;
}

char * replaceCatch( char * newLine, char * str, int index ){
  char *origin = newLine, *line = Catch.ptr[ 0 ];
  strCpy( origin, Catch.ptr[0] );

  for( int iCatch = 1; iCatch < Catch.index; iCatch++ )
    if( Catch.id[ iCatch ] == index ){
      strCpy( newLine, line );
      newLine += Catch.ptr[ iCatch ] - line;
      strCpy( newLine, str );
      newLine += strLen( str );
      line =  Catch.ptr[ iCatch ] + Catch.len[ iCatch ];
      strCpy( newLine, line );
    }

  return origin;
}

char * newLineCatch( char * newLine, char * str ){
  char index, *pos, *origin = newLine;
  strCpy( origin, str );

  while( (pos = strChr( str, '\\' )) ){
    if( pos[ 1 ] == '\\' ){
      newLine += pos + 1 - str;
      str      = pos + 2;
    } else {
      index = aToi( pos + 1 );
      strCpy( newLine, str );
      newLine += pos - str;
      newLine  = getCatch( newLine, index );
      newLine += strLen( newLine );
      str      = pos + 1 + countDigits( index );
    }

    strCpy( newLine, str );
  }

  return origin;
}

int match( struct Path *text, struct PathLine *pathLine ){
  switch( text->type ){
  case POINT  : return utf8meter( pathLine->line + pathLine->pos );
  case RANGEAB: return pathLine->line[ pathLine->pos ] >= text->ptr[ 0 ] &&
                       pathLine->line[ pathLine->pos ] <= text->ptr[ 2 ];
  case BRACKET: return matchBracket( text, pathLine );
  case META   : return matchMeta( text, pathLine->line + pathLine->pos );
  default     : return matchText( text, pathLine->line + pathLine->pos );
  }
}


int matchBracket( struct Path *text, struct PathLine *pathLine ){
  struct Path track, path = *text;
  int result= 0;
  int reverse = *text->ptr == '^';
  if( reverse ){ path.len--; path.ptr++; }
  while( tracker( &path, &track ) ){
    switch( track.type ){
    case POINT: case RANGEAB: case META: case UTF8:
      result = match( &track, pathLine ); break;
    default:
      result = strnChr( track.ptr, pathLine->line[pathLine->pos], track.len  ) != 0;
    }

    if( result ) return reverse ? FALSE : result;
  }

  return reverse ? utf8meter( pathLine->line + pathLine->pos ) : FALSE;
}

int matchMeta( struct Path *text, char *line ){
  switch( text->ptr[1] ){
  case 'd' : return  isDigit(*line);
  case 'D' : return  isDigit(*line) == 0 ? utf8meter( line ) : FALSE;
  case 'w' : return  isAlnum(*line);
  case 'W' : return  isAlnum(*line) == 0 ? utf8meter( line ) : FALSE;
  case 's' : return  isSpace(*line);
  case 'S' : return  isSpace(*line) == 0 ? utf8meter( line ) : FALSE;
  default  : return *line == text->ptr[1];
  }
}

int matchText( struct Path *text, char *line ){
  return strnCmp( line, text->ptr, text->len )  == 0 ? text->len : 0;
}
