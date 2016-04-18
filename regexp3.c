#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

enum PTYPE { PATH, HOOK, GROUP, SIMPLE, BRACKET, RANGEAB, META, POINT };

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
int   match       ( struct Path *text,    struct PathLine *pathLine              );
int   matchBracket( struct Path *text,    struct PathLine *pathLine              );
int   matchMeta   ( struct Path *text,           char     *line                  );
int   matchText   ( struct Path *text,           char     *line                  );

int regexp3( char *line, char *exp ){
  struct Path     path;
  struct PathLine pathLine;
  int atTheEnd    = FALSE;
  int result      = 0;
  int loops       = strlen( line );
  path.ptr        = exp;
  path.len        = strlen( exp );
  path.type       = PATH;
  Catch.index     = 1;
  Catch.ptr[0]    = line;
  Catch.len[0]    = loops;

  if( *(exp + path.len - 1) == '$' ){
    path.len--;
    atTheEnd = TRUE;
  }

  if( *exp == '^' ){
    path.ptr++;
    path.len--;
    loops = 1;
  }

  for( int i = 0; i < loops; i++ ){
    Catch.idx     = 1;
    pathLine.pos  = 0;
    pathLine.line = line + i;
    pathLine.len  = Catch.len[0] - i;

    if( atTheEnd ){
      result = walker( path, &pathLine );

      if( result && pathLine.pos != pathLine.len )
        result = 0;
      if( result  ) i = loops;
    } else result += walker( path, &pathLine );
  }

  return result;
}

int walker( struct Path path, struct PathLine *pathLine ){
  struct Path track;
  while( cutTrack( &path, &track, PATH ) )
    if( walker( track,  pathLine ) )
      return TRUE;

  return track.len ? trekking( &track, pathLine ) : FALSE;
}

int cutTrack( struct Path *path, struct Path *track, int type ){
  track->ptr  = path->ptr + (type == PATH ? 0 : 1);
  track->len  = path->len;
  track->type = path->type;

  for( int cut, i = 0, deep = 0; i < path->len; i++ ){
    i += walkMeta ( path->ptr + i );
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

  return FALSE;
}

int trekking( struct Path *path, struct PathLine *pathLine ){
  struct Path track;
  int loops, len, iCatch, opos = pathLine->pos;

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
      pathLine->pos = opos;
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

char * strnpbrk( char *cs, char *ct, int n ){
  for( int i = 0; i < n && ct[ i ]; i++ )
    if( strchr( cs, ct[ i ] ) ) return ct + i;

  return 0;
}

int tracker( struct Path *path, struct Path *track ){
  if( path->len ){
    char *pchar;

    switch( *path->ptr ){
    case '\\': trackByLen( path, track, 2, META    ); break;
    case '.' : trackByLen( path, track, 1, POINT   ); break;
    case '(' : cutTrack  ( path, track,    GROUP   ); break;
    case '<' : cutTrack  ( path, track,    HOOK    ); break;
    case '[' : cutTrack  ( path, track,    BRACKET ); break;
    default:
      if( pchar = strnpbrk( ".[(<\\?+*{-", path->ptr + 1, path->len - 1) ){
        switch( *pchar ){
        case '.': case '[': case '(': case '<': case '\\':
          trackByLen( path, track, pchar - path->ptr, SIMPLE  ); break;
        case '?': case '+': case '*': case '{': case '-':
          if( pchar - path->ptr == 1 ){
            if( *pchar == '-' ) trackByLen( path, track, 3, RANGEAB );
            else                trackByLen( path, track, 1, SIMPLE  );
          } else trackByLen( path, track, (pchar - path->ptr) - 1, SIMPLE  );
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
    while( str[i] != ']' )
      i++;

  return i;
}

int isPath( struct Path *track ){
  switch( track->type ){
  case PATH: case HOOK: case GROUP:
    for( int i = 0; i < track->len; i++ )
      if( strchr( "|(<[?+*{-\\", track->ptr[i] ) )
        return TRUE;
  }

  return FALSE;
}

int countDigits( int number ){
  int i = 1;
  while( number /= 10 ) i++;

  return i;
}

void setLoops( struct Path *track, struct Path *path ){
  int len = 0;
  track->loopsMin = 1; track->loopsMax = 1;
  if( path->len )
    switch( *path->ptr ){
    case '?' : len = 1; track->loopsMin = 0; track->loopsMax =   1; break;
    case '+' : len = 1; track->loopsMin = 1; track->loopsMax = INF; break;
    case '*' : len = 1; track->loopsMin = 0; track->loopsMax = INF; break;
    case '{' :
      track->loopsMin = atoi( path->ptr + 1 ) ;
      if( path->ptr[ 1 + countDigits( track->loopsMin ) ] == ',' )
        track->loopsMax = atoi( strchr( path->ptr, ',' ) + 1 );
      else
        track->loopsMax = track->loopsMin;

      len = strchr( path->ptr, '}' ) - path->ptr + 1;
      break;
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

int indexCatch(){ return Catch.index - 1;}

char * getCatch( char * lineCatch, int index ){
  if( index > 0 && index < Catch.index ){
    strncpy( lineCatch, Catch.ptr[ index ], Catch.len[ index ] );
    lineCatch[ Catch.len[ index ] ] = '\0';
  } else *lineCatch = '\0';

  return lineCatch;
}

char * replaceCatch( char * newLine, char * str, int index ){
  char *origin = newLine, *line = Catch.ptr[ 0 ];
  strcpy( origin, Catch.ptr[0] );

  for( int iCatch = 1; iCatch < Catch.index; iCatch++ )
    if( Catch.id[ iCatch ] == index ){
      strncpy( newLine, line, Catch.ptr[ iCatch ] - line );
      newLine += Catch.ptr[ iCatch ] - line;
      strcpy( newLine, str );
      newLine += strlen( str );
      line =  Catch.ptr[ iCatch ] + Catch.len[ iCatch ];
      strcpy( newLine, line );
    }

  return origin;
}

int match( struct Path *text, struct PathLine *pathLine ){
  switch( text->type ){
  case POINT  : return pathLine->line[pathLine->pos] != 0;
  case RANGEAB: return pathLine->line[ pathLine->pos ] >= text->ptr[ 0 ] &&
                       pathLine->line[ pathLine->pos ] <= text->ptr[ 2 ];
  case BRACKET: return matchBracket( text, pathLine );
  case META   : return matchMeta( text, pathLine->line + pathLine->pos );
  default     : return matchText( text, pathLine->line + pathLine->pos );
  }
}

char * strnchr( char *s, int c, int n ){
  for( int i = 0; i < n && s[ i ]; i++ )
    if( s[ i ] == c ) return s + i;

  return 0;
}

int matchBracket( struct Path *text, struct PathLine *pathLine ){
  struct Path track, path = *text;
  int result= 0;
  int reverse = *text->ptr == '^';
  if( reverse ){ path.len--; path.ptr++; }
  while( tracker( &path, &track ) ){
    switch( track.type ){
    case POINT: case RANGEAB: case META:
      result = match( &track, pathLine ); break;
    default       :
      result = strnchr( track.ptr, pathLine->line[pathLine->pos], track.len  ) != 0;
      break;
    }

    if( result ) return reverse ? FALSE : TRUE;
  }

  return reverse ? TRUE : FALSE;
}

int matchMeta( struct Path *text, char *line ){
  switch( text->ptr[1] ){
  case 'd' : return  isdigit(*line) != 0;
  case 'D' : return  isdigit(*line) == 0;
  case 'w' : return  isalnum(*line) != 0;
  case 'W' : return  isalnum(*line) == 0;
  case 's' : return  isspace(*line) != 0;
  case 'S' : return  isspace(*line) == 0;
  default  : return *line == text->ptr[1];
  }
}

int matchText( struct Path *text, char *line ){
  return strncmp( line, text->ptr, text->len )  == 0 ? text->len : 0;
}
