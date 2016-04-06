#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "regexp3.h"

#define TRUE      1
#define FALSE     0
#define INF    1024
#define CATCHS   24

struct CATch {
  char *ptr[CATCHS];
  int   len[CATCHS];
  int   index;
} Catch;

struct LoopsRange {
  int a, b;
};

enum PTYPE { PATH, HOOK, SIMPLE, BRACKET, RANGEAB, META, POINT };

struct Path {
  int   len;
  char *ptr;
  enum   PTYPE type;
  struct LoopsRange loopsRange;
};

struct PathLine {
  char *line;
  int   pos;
  int   len;
};

int   walker      ( struct Path   path, struct PathLine *pathLine             );
int   cutTrack    ( struct Path  *path, struct Path     *track,    int type   );
int   trekking    ( struct Path  *path, struct PathLine *pathLine             );
int   tracker     ( struct Path  *path, struct Path     *track                );
int   walkMeta    (                            char     *str                  );
int   walkBracket (                            char     *str                  );
int   isMeta      (                            char     *str                  );
int   isPath      ( struct Path *track                                        );
void  setLoops    ( struct Path *track, struct Path     *path                 );
void  openCatch   ( struct Path *track, struct PathLine *pathLine, int *index );
void  closeCatch  ( struct Path *track, struct PathLine *pathLine, int  index );
void  delCatch    ( struct Path *track,                            int  index );
int   CatchIndex  (                                                           );
char *getCatch    (                           char     *lineCatch, int  index );
int   match       ( struct Path *text, struct PathLine *pathLine              );
int   matchBracket( struct Path *text, struct PathLine *pathLine              );
int   matchMeta   ( struct Path *text,        char     *line                  );
int   matchText   ( struct Path *text,        char     *line                  );

int regexp3( char *line, char *exp ){
  struct Path     path;
  struct PathLine pathLine;
  int atTheEnd  = FALSE;
  int result    = FALSE;
  int loops     = strlen( line );

  path.ptr      = exp;
  path.len      = strlen( exp );
  path.type     = PATH;

  if( *(exp + path.len - 1) == '$' ){
    path.len--;
    atTheEnd = TRUE;
  }

  if( *exp == '^' ){
    path.ptr++;
    path.len--;
    loops = 1;
  }

  for( int i = 0; !result && i < loops; i++ ){
    Catch.index   = 0;
    pathLine.pos  = 0;
    pathLine.line = line + i;
    pathLine.len  = strlen( line ) - i;
    result = walker( path, &pathLine );

    if( atTheEnd && result && pathLine.pos != pathLine.len )
      result = 0;
  }

  return result;
}

int walker( struct Path path, struct PathLine *pathLine ){
  struct Path track;

  while( cutTrack( &path, &track, PATH ) )
    if( walker( track,  pathLine ) )
      return TRUE;

  return trekking( &track, pathLine );
}

int cutTrack( struct Path *path, struct Path *track, int type ){
  int i, deep, cut;
  track->ptr  = path->ptr + (type == PATH ? 0 : 1);
  track->len  = path->len;
  track->type = path->type;

  for( i = deep = 0; i < path->len; i++ ){
    i += walkMeta ( path->ptr + i );
    i += type == BRACKET ? 0 : walkBracket( path->ptr + i );

    if( path->ptr[i] == '(' ) deep++;
    if( path->ptr[i] == ')' ) deep--;

    switch( type ){
    case HOOK    : cut = deep == 0; break;
    case PATH    : cut = deep == 0 && path->ptr[i] == '|'; break;
    case BRACKET : cut = path->ptr[i] == ']'; break;
    }

    if( cut && i < path->len){
      track->len = &path->ptr[i] - track->ptr;
      path->ptr = path->ptr + i + 1;
      path->len = path->len - i - 1;
      track->type = type;
      return TRUE;
    }
  }

  return FALSE;
}

int trekking( struct Path *path, struct PathLine *pathLine ){
  struct Path track;
  int npos = 0, loop , len, iCatch;
  while( tracker( path, &track ) ){
    openCatch( &track, pathLine, &iCatch);

    if( isPath( &track ) ){
      for( loop = 0; loop < track.loopsRange.b && walker( track, pathLine ); ) loop++;
    } else {
      len = (track.type == SIMPLE || track.type == HOOK) ? track.len : 1;

      for( loop = 0; loop < track.loopsRange.b     &&
                     pathLine->pos < pathLine->len && match( &track, pathLine ); ){
        pathLine->pos += len;
        npos          += len;
        loop++;
      }
    }

    if( loop < track.loopsRange.a ){
      pathLine->pos -= npos;
      delCatch( &track, iCatch );
      return FALSE;
    } else closeCatch( &track, pathLine, iCatch);
  }

  return TRUE;
}

void trackByLen( struct Path *path, struct Path *track, int len, int type ){
  track->type  = type;
  track->len   = len;
  path->ptr   += len;
  path->len   -= len;
}

int tracker( struct Path *path, struct Path *track ){
  int i;
  track->ptr  = path->ptr;
  track->len  = path->len;
  track->type = SIMPLE;

  for( i = 0; i < path->len; i++ ){
    if( strchr( ".[(", path->ptr[i] ) || isMeta( path->ptr + i ) ){
      if( i > 0 ){
        trackByLen( path, track, i, SIMPLE );
      } else if( isMeta( path->ptr ) ){
        trackByLen( path, track, 2, META );
      } else if( *path->ptr == '.' ){
        trackByLen( path, track, 1, POINT );
      } else if( *path->ptr == '(' ){
        cutTrack( path, track, HOOK );
      } else if( *path->ptr == '[' ){
        cutTrack( path, track, BRACKET );
      }

      setLoops( track, path );
      return TRUE;
    }

    if( strchr( "?+*{-", path->ptr[i] ) ){
      if( i > 1 ){
        track->len = &path->ptr[i] - track->ptr - 1;
        path->ptr = path->ptr + i - 1;
        path->len = path->len - i + 1;
      } else if( i == 1 ){
        if( path->ptr[i] == '-' ){
          trackByLen( path, track, 3, RANGEAB );
        } else {
          trackByLen( path, track, 1, SIMPLE );
        }
      }

      setLoops( track, path );
      return TRUE;
    }
  }

  if( track->len > 0 ){
    path->ptr += i;
    path->len  = 0;
    setLoops( track, path );
    return TRUE;
  }

  return FALSE;
}

int walkMeta( char *str ){
  int i = 0;
  while( str[i] == '\\' )
    if( isMeta( str + i ) ) i += 2;
    else return i;

  return i;
}

int walkBracket( char *str ){
  int i = 0;
  if( *str == '[' )
    while( str[i] != ']' )
      i++;

  return i;
}

int isMeta( char *str ){
  if( *str == '\\' )
    return strchr( "|^$()[].?+*{}-dDwWsS", str[ 1 ] ) != 0;

  return FALSE;
}

int isPath( struct Path *track ){
  if( track->type == META || track->type == BRACKET || track->type == RANGEAB ) return FALSE;

  for( int i = 0; i < track->len; i++ )
    if(  isMeta( track->ptr + i ) || strchr( "|(?+*{[-", track->ptr[i] ) )
      return TRUE;

  return FALSE;
}

int countDigits( int number ){
  int i;
  for( i = 0; number; i++ )
    number /= 10;

  return i;
}

void setLoops( struct Path *track, struct Path *path ){
  int len = 0;
  track->loopsRange.a = 1; track->loopsRange.b = 1;
  if( path->len )
    if( strchr( "?+*{", *path->ptr ))
      switch( *path->ptr ){
      case '?' : len = 1; track->loopsRange.a = 0; track->loopsRange.b =   1; break;
      case '+' : len = 1; track->loopsRange.a = 1; track->loopsRange.b = INF; break;
      case '*' : len = 1; track->loopsRange.a = 0; track->loopsRange.b = INF; break;
      case '{' :
        track->loopsRange.a = atoi( path->ptr + 1 ) ;
        if( path->ptr[ countDigits( track->loopsRange.a ) + 1 ] == ',' )
          track->loopsRange.b = atoi( strchr( path->ptr, ',' ) + 1 );
        else track->loopsRange.b = track->loopsRange.a;
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
  }
}

int CatchIndex(){
  return Catch.index;
}

char * getCatch( char * lineCatch, int index ){
  if( index > 0 && index <= Catch.index ){
    index--;
    strncpy( lineCatch, Catch.ptr[ index ], Catch.len[ index ] );
    lineCatch[ Catch.len[ index ] ] = '\0';
  } else *lineCatch = '\0';

  return lineCatch;
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

  return FALSE;
}

int matchBracket( struct Path *text, struct PathLine *pathLine ){
  struct Path track, path = *text;
  int result= 0;
  int reverse = *text->ptr == '^';
  if( reverse ){ path.len--; path.ptr++; }
  while( tracker( &path, &track ) ){
    switch( track.type ){
    case POINT    :
    case RANGEAB  :
    case META     : result = match( &track, pathLine ); break;
    default       :
      result = strnchr( track.ptr, pathLine->line[pathLine->pos], track.len  ) != 0;
      break;
    }

    if( result ) return reverse ? FALSE : TRUE;
  }

  return reverse ? TRUE : FALSE;
}

int matchMeta( struct Path *text, char *line ){
  if( strchr( "|^$()[].?+*{}-", text->ptr[1] ) )
    return *line == text->ptr[1];
  else
    switch( text->ptr[1] ){
    case 'd' : return  isdigit(*line); break;
    case 'D' : return !isdigit(*line); break;
    case 'w' : return  isalnum(*line); break;
    case 'W' : return !isalnum(*line); break;
    case 's' : return  isspace(*line); break;
    case 'S' : return !isspace(*line); break;
    }
}

int matchText( struct Path *text, char *line ){
  return strncmp( line, text->ptr, text->len )  == 0;
}
