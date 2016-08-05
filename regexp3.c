#include "regexp3.h"
#include "charUtils.h"

#define TRUE                  1
#define FALSE                 0
#define CATCHS               24
#define INF               65536

#define MOD_ALPHA             1
#define MOD_OMEGA             2
#define MOD_LONLEY            4
#define MOD_NEGATION          8

struct CATch {
  char *ptr[CATCHS];
  int   len[CATCHS];
  int   id [CATCHS];
  int   idx;
  int   index;
} Catch;

struct TEXT {
  char *ptr;
  int   pos;
  int   len;
};

enum TYPE { SIMPLE, PATH, HOOK, GROUP, BRACKET, RANGEAB, META, POINT, UTF8 };

struct RE {
  char          *ptr;
  int            len;
  enum     TYPE  type;
  unsigned char  mods;
  unsigned int   loopsMin, loopsMax;
};

static void openCatch( struct RE *rexp, struct TEXT *text, int *index ){
  if( rexp->type == HOOK && Catch.index < CATCHS ){
    *index = Catch.index++;
    Catch.ptr[ *index ] = text->ptr + text->pos;
    Catch.len[ *index ] = 0;
    Catch.id [ *index ] = Catch.idx++;
  } else *index = CATCHS;
}

static void closeCatch( struct RE *rexp, struct TEXT *text, int index ){
  if( rexp->type == HOOK && index < CATCHS )
    Catch.len[ index ] = &text->ptr[ text->pos ] - Catch.ptr[ index ];
}

static void delCatch( struct RE *rexp, int index ){
  if( rexp->type == HOOK && index < CATCHS ){
    Catch.index = index;
    Catch.ptr[ index ] = 0;
    Catch.len[ index ] = 0;
    Catch.idx = Catch.id[ index ];
  }
}

int totCatch(){ return Catch.index - 1; }

char * cpyCatch( char * str, int index ){
  if( index > 0 && index < Catch.index )
    strnCpy( str, Catch.ptr[ index ], Catch.len[ index ] );
  else *str = '\0';

  return str;
}

char * rplCatch( char * newTxt, char * rStr, int index ){
  char *origin = newTxt, *line = Catch.ptr[ 0 ];
  strCpy( origin, Catch.ptr[0] );

  for( int iCatch = 1; iCatch < Catch.index; iCatch++ )
    if( Catch.id[ iCatch ] == index ){
      strCpy( newTxt, line );
      newTxt += Catch.ptr[ iCatch ] - line;
      strCpy( newTxt, rStr );
      newTxt += strLen( rStr );
      line    = Catch.ptr[ iCatch ] + Catch.len[ iCatch ];
      strCpy( newTxt, line );
    }

  return origin;
}

char * modCatch( char * newTxt, char * mStr ){
  char index, *pos, *origin = newTxt;
  strCpy( origin, mStr );

  while( (pos = strChr( mStr, '\\' )) ){
    if( pos[ 1 ] == '\\' ){
      newTxt += pos + 1 - mStr;
      mStr    = pos + 2;
    } else {
      index = aToi( pos + 1 );
      strCpy( newTxt, mStr );
      newTxt += pos - mStr;
      newTxt  = cpyCatch( newTxt, index );
      newTxt += strLen( newTxt );
      mStr    = pos + 1 + countDigits( index );
    }

    strCpy( newTxt, mStr );
  }

  return origin;
}

char * gpsCatch( int index ){
  return ( index > 0 && index < Catch.index ) ? Catch.ptr[ index ] : 0;
}

int lenCatch( int index ){
  return ( index > 0 && index < Catch.index ) ? Catch.len[ index ] : 0;
}

static void fwrTrack( struct RE *track, int len ){
  track->ptr += len; track->len -= len;
}

static void setMods( struct RE *path, struct RE *rexp ){
  int len = 0, ok = TRUE;
  rexp->mods = 0;

  if( path->len )
    if( *path->ptr == '#' )
      while( ok )
        switch( path->ptr[ ++len ] ){
        case '^': rexp->mods |= MOD_ALPHA   ; break;
        case '$': rexp->mods |= MOD_OMEGA   ; break;
        case '!': rexp->mods |= MOD_NEGATION; break;
        case '?': rexp->mods |= MOD_LONLEY  ; break;
        default : ok          = FALSE       ; break;
        }

  fwrTrack( path, len );
}

static int walkMeta( char *str ){
  for( int i = 0; ; i += 2 )
    if( str[i] != '\\' ) return i;
}

static int walkBracket( char *str ){
  if( *str == '[' ){
    for( int i = 0; ; i++ )
      if( str[i] == ']' ) return i;
  } else return 0;
}

static void setLoops( struct RE *rexp, struct RE *track ){
  track->loopsMin = 1; track->loopsMax = 1;
  int len = 0;

  if( rexp->len )
    switch( *rexp->ptr ){
    case '?' : len = 1; track->loopsMin = 0; track->loopsMax =   1; break;
    case '+' : len = 1; track->loopsMin = 1; track->loopsMax = INF; break;
    case '*' : len = 1; track->loopsMin = 0; track->loopsMax = INF; break;
    case '{' :
      track->loopsMin = aToi( rexp->ptr + 1 ) ;
      if( rexp->ptr[ 1 + countDigits( track->loopsMin ) ] == ',' )
        track->loopsMax = aToi( strChr( rexp->ptr, ',' ) + 1 );
      else
        track->loopsMax = track->loopsMin;

      len = strChr( rexp->ptr, '}' ) - rexp->ptr + 1;
    }

  fwrTrack( rexp, len );
}

static int cutTrack( struct RE *rexp, struct RE *track, int type ){
  track->ptr  = rexp->ptr + (type == PATH ? 0 : 1);
  track->len  = rexp->len;
  track->type = rexp->type;

  for( int cut, i = 0, deep = 0; i < rexp->len; i++ ){
    i += walkMeta   ( rexp->ptr + i );
    i += walkBracket( rexp->ptr + i );

    switch( rexp->ptr[i] ){
    case '<': case '(': deep++; break;
    case '>': case ')': deep--; break;
    }

    switch( type ){
    case HOOK    : cut = deep == 0; break;
    case GROUP   : cut = deep == 0; break;
    case PATH    : cut = deep == 0 && rexp->ptr[i] == '|'; break;
    case BRACKET : cut = rexp->ptr[i] == ']'; break;
    }

    if( cut && i < rexp->len){
      track->len  = &rexp->ptr[i] - track->ptr;
      track->type = type;
      fwrTrack( rexp, i + 1 );
      return TRUE;
    }
  }

  fwrTrack( rexp, rexp->len );

  return track->len ? TRUE : FALSE;
}

static int isPath( struct RE *rexp ){
  switch( rexp->type ){
  case PATH: case HOOK: case GROUP:
    for( int i = 0; i < rexp->len; i++ )
      if( strChr( "|(<[.?+*{-\\#", rexp->ptr[i] ) )
        return TRUE;
  default: return FALSE;
  }
}

static void trackByLen( struct RE *path, struct RE *track, int len, int type ){
  track->ptr   = path->ptr;
  track->type  = type;
  track->len   = len;
  fwrTrack( path, len );
}

static char * trackerPoint( char *ct, int len ){
  for( int i = 0; i < len && ct[ i ]; i++ )
    if( strChr( "(<[.?+*{-\\#", ct[ i ] ) || ct[ i ] & xooooooo ) return ct + i;

  return 0;
}

static int tracker( struct RE *rexp, struct RE *track ){
  setMods( rexp, track );

  if( rexp->len ){
    char *point;

    switch( *rexp->ptr & xooooooo ? UTF8 : *rexp->ptr ){
    case '\\': trackByLen( rexp, track,                    2, META    ); break;
    case '.' : trackByLen( rexp, track,                    1, POINT   ); break;
    case '(' : cutTrack  ( rexp, track,                       GROUP   ); break;
    case '<' : cutTrack  ( rexp, track,                       HOOK    ); break;
    case '[' : cutTrack  ( rexp, track,                       BRACKET ); break;
    case UTF8: trackByLen( rexp, track, utf8meter(rexp->ptr), UTF8    ); break;
    default:
      if( (point = trackerPoint( rexp->ptr + 1, rexp->len - 1 )) ){
        switch( *point ){
        default: trackByLen( rexp, track, point - rexp->ptr, SIMPLE  ); break;
        case '?': case '+': case '*': case '{': case '-':
          if( point - rexp->ptr == 1 ){
            if( *point == '-' ) trackByLen( rexp, track, 3, RANGEAB );
            else                trackByLen( rexp, track, 1, SIMPLE  );
          } else trackByLen( rexp, track, (point - rexp->ptr) - 1, SIMPLE  );
        }
      } else trackByLen( rexp, track, rexp->len, SIMPLE  );
    }

    setLoops( rexp, track );
    return TRUE;
  }

  return FALSE;
}

static int match( struct RE *rexp, struct TEXT *text );

static int matchBracket( struct RE *rexp, struct TEXT *text ){
  struct RE track, path = *rexp;
  int result  = 0;
  int reverse = *rexp->ptr == '^';
  if( reverse ) fwrTrack( &path, 1 );
  while( tracker( &path, &track ) ){
    switch( track.type ){
    case POINT: case RANGEAB: case META: case UTF8:
      result = match( &track, text ); break;
    default:
      result = strnChr( track.ptr, text->ptr[text->pos], track.len  ) != 0;
    }

    if( result ) return reverse ? FALSE : result;
  }

  return reverse ? utf8meter( text->ptr + text->pos ) : FALSE;
}

static int matchMeta( struct RE *rexp, char *line ){
  switch( rexp->ptr[1] ){
  case 'd' : return isDigit(*line);
  case 'D' : return isDigit(*line) == 0 ? utf8meter( line ) : FALSE;
  case 'w' : return isAlnum(*line);
  case 'W' : return isAlnum(*line) == 0 ? utf8meter( line ) : FALSE;
  case 's' : return isSpace(*line);
  case 'S' : return isSpace(*line) == 0 ? utf8meter( line ) : FALSE;
  case '&' : return *line  &  xooooooo  ? utf8meter( line ) : FALSE;
  default  : return *line == rexp->ptr[1];
  }
}

static int matchText( struct RE *rexp, char *line ){
  return strnCmp( line, rexp->ptr, rexp->len )  == 0 ? rexp->len : 0;
}

static int match( struct RE *rexp, struct TEXT *text ){
  switch( rexp->type ){
  case POINT  : return utf8meter( text->ptr + text->pos );
  case RANGEAB: return text->ptr[ text->pos ] >= rexp->ptr[ 0 ] &&
                       text->ptr[ text->pos ] <= rexp->ptr[ 2 ];
  case BRACKET: return matchBracket( rexp, text );
  case META   : return matchMeta( rexp, text->ptr + text->pos );
  default     : return matchText( rexp, text->ptr + text->pos );
  }
}

static int walker( struct RE rexp, struct TEXT *text );

static int trueLoop( struct RE *track, struct TEXT *text ){
  int steps, loops = 0;

  if( isPath( track ) )
    while( loops < track->loopsMax && walker( *track, text ) )
      loops++;
  else
    while( loops < track->loopsMax && text->pos < text->len
                    && (steps = match( track, text )) ){
      text->pos += steps;
      loops++;
    }

  return loops < track->loopsMin ? FALSE : TRUE;
}

static int negLoop( struct RE *track, struct TEXT *text ){
  int steps = text->pos, loops = 0;

  if( isPath( track ) ){
    while( loops < track->loopsMax && !walker( *track, text ) ){
      steps += utf8meter( text->ptr + steps );
      text->pos = steps;
      loops++;
    }

    text->pos = steps;
  } else
    while( loops < track->loopsMax && text->pos < text->len && !match( track, text ) ){
      text->pos += utf8meter( text->ptr + text->pos  );
      loops++;
    }

  return loops < track->loopsMin ? FALSE : TRUE;
}

static int trekking( struct RE *rexp, struct TEXT *text ){
  struct RE track;
  int iCatch, oPos = text->pos;

  while( tracker( rexp, &track ) ){
    openCatch( &track, text, &iCatch );

    if( track.mods & MOD_NEGATION ? negLoop( &track, text ) : trueLoop( &track, text ) )
      closeCatch( &track, text, iCatch );
    else {
      text->pos = oPos;
      delCatch( &track, iCatch );
      return FALSE;
    }
  }

  return TRUE;
}

static int walker( struct RE rexp, struct TEXT *text ){
  struct RE track;
  while( cutTrack( &rexp, &track, PATH ) )
    if( track.len && trekking( &track, text ) ) return TRUE;

  return FALSE;
}

int regexp3( char *txt, char *re ){
  struct RE     rexp;
  struct TEXT   text;
  int result    = 0;
  int loops     = strLen( txt );
  rexp.len      = strLen( re );
  rexp.type     = PATH;
  rexp.ptr      = re;
  Catch.index   = 1;
  Catch.ptr[0]  = txt;
  Catch.len[0]  = loops;

  if( loops == 0 || rexp.len == 0 ) return 0;

  setMods( &rexp, &rexp );

  if( rexp.mods & MOD_ALPHA ) loops = 1;

  for( int i = 0, hit; i < loops; i += hit && text.pos ? text.pos : utf8meter( txt + i ) ){
    hit       = 0;
    Catch.idx = 1;
    text.pos  = 0;
    text.ptr  = txt + i;
    text.len  = Catch.len[0] - i;

    if( rexp.mods & MOD_OMEGA ){
      if( walker( rexp, &text ) && text.pos == text.len ) return TRUE;
    } else if( (hit = walker( rexp, &text )) && rexp.mods & MOD_LONLEY  ) return TRUE;
    else result += hit;
  }

  return result;
}
