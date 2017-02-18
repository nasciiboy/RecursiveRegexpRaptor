#include "regexp3.h"
#include "charUtils.h"

#define TRUE                 1
#define FALSE                0
#define MAX_CATCHS          16
#define INF         1073741824 // 2^30

#define MOD_ALPHA            1
#define MOD_OMEGA            2
#define MOD_LONLEY           4
#define MOD_FwrByChar        8
#define MOD_COMMUNISM       16
#define MOD_NEGATIVE       128

struct CATch {
  const char *ptr[ MAX_CATCHS ];
  int   len[ MAX_CATCHS ];
  int   id [ MAX_CATCHS ];
  int   idx;
  int   index;
} static Catch;

struct TEXT {
  const char *ptr;
  int   pos;
  int   len;
} static text;

enum TYPE { PATH, GROUP, HOOK, SET, BACKREF, META, RANGEAB, POINT, SIMPLE };

struct RE {
  const    char *ptr;
  int            len;
  enum     TYPE  type;
  unsigned char  mods;
  unsigned int   loopsMin, loopsMax;
};

static int  walker       ( struct RE  rexp );
static int  trekking     ( struct RE *rexp );
static int  looper       ( struct RE *rexp );
static int  loopGroup    ( struct RE *rexp );
static int  tracker      ( struct RE *rexp, struct RE *track );
static int  trackerSet   ( struct RE *rexp, struct RE *track );

static int  cutPath      ( struct RE *rexp, struct RE *track );
static void cutSimple    ( struct RE *rexp, struct RE *track );
static void cutPair      ( struct RE *rexp, struct RE *track, const int type );
static void cutByLen     ( struct RE *rexp, struct RE *track, const int len, const int type );
static void cutRexp      ( struct RE *rexp, const int len );

static int  walkMeta     ( const char *str, const int len );
static int  walkPair     ( const char *str, const int len, const char p[2] );

static void getMods      ( struct RE *rexp, struct RE *track );
static void getLoops     ( struct RE *rexp, struct RE *track );

static int  match        ( struct RE *rexp );
static int  matchSet     ( struct RE  rexp );
static int  matchBackRef ( struct RE *rexp );
static int  matchRange   ( struct RE *rexp, int chr );
static int  matchMeta    ( struct RE *rexp, const int chr );
static int  matchText    ( struct RE *rexp, const char *txt );

static void openCatch    ( int *index );
static void closeCatch   ( const int index );
static int  lastIdCatch  ( const int id    );

int regexp3( const char *txt, const char *re ){
  struct RE    rexp;
  int result   = 0;
  text.len     = strLen( txt );
  Catch.ptr[0] = txt;
  Catch.len[0] = text.len;
  Catch.id [0] = 0;
  Catch.index  = 1;
  rexp.ptr     = re;
  rexp.type    = PATH;
  rexp.len     = strLen( re );
  rexp.mods    = 0;

  if( text.len == 0 || rexp.len == 0 ) return 0;

  getMods( &rexp, &rexp );

  for( int forward, i = 0, loops = rexp.mods & MOD_ALPHA ? 1 : text.len; i < loops; i += forward ){
    forward    = 1;
    Catch.idx  = 1;
    text.pos   = 0;
    text.ptr   = txt          + i;
    text.len   = Catch.len[0] - i;

    if( walker( rexp ) ){
      if     (  rexp.mods & MOD_OMEGA    ){
        if( text.pos == text.len )                            return TRUE;
        else Catch.index = 1;
      }
      else if(  rexp.mods & MOD_LONLEY   )                    return TRUE;
      else if( (rexp.mods & MOD_FwrByChar) || text.pos == 0 ) result++;
      else   {  forward = text.pos;                           result++; }
    }
  }

  return result;
}

static int walker( struct RE rexp ){
  struct RE track;
  for( const int oTpos = text.pos, oCindex = Catch.index, oCidx = Catch.idx;
       cutPath( &rexp, &track );
       text.pos = oTpos, Catch.index = oCindex, Catch.idx = oCidx )
    if( trekking( &track ) ) return TRUE;

  return FALSE;
}

static int trekking( struct RE *rexp ){
  struct RE track;
  for( int result = FALSE, iCatch; tracker( rexp, &track ); ){
    switch( track.type ){
    case HOOK:
      openCatch( &iCatch );
      result = loopGroup( &track );
      if( result ) closeCatch( iCatch );
      break;
    case GROUP: case PATH:
      result = loopGroup( &track );
      break;
    case SET:
      if( track.ptr[0] == '^' ){
        cutRexp( &track, 1 );
        if( track.mods & MOD_NEGATIVE ) track.mods &= ~MOD_NEGATIVE;
        else                            track.mods |=  MOD_NEGATIVE;
      }
    case BACKREF: case META: case RANGEAB: case POINT: case SIMPLE:
      result = looper( &track );
    }

    if( result == FALSE ) return FALSE;
  }

  return TRUE;
}

static int looper( struct RE *rexp ){
  int forward, loops = 0;

  if( rexp->mods & MOD_NEGATIVE )
    while( loops < rexp->loopsMax && text.pos < text.len && !match( rexp ) ){
      text.pos += 1;
      loops++;
    }
  else
    while( loops < rexp->loopsMax && text.pos < text.len && (forward = match( rexp )) ){
      text.pos += forward;
      loops++;
    }

  return loops < rexp->loopsMin ? FALSE : TRUE;
}

static int loopGroup( struct RE *rexp ){
  int loops = 0, textPos = text.pos;

  if( rexp->mods & MOD_NEGATIVE ){
    while( loops < rexp->loopsMax && !walker( *rexp ) ){
      textPos++;
      text.pos = textPos;
      loops++;
    }
    text.pos = textPos;
  } else
    while( loops < rexp->loopsMax && walker( *rexp ) )
      loops++;

  return loops < rexp->loopsMin ? FALSE : TRUE;
}

static int tracker( struct RE *rexp, struct RE *track ){
  if( rexp->len == 0 ) return FALSE;

  switch( *rexp->ptr ){
  case ':': cutByLen ( rexp, track, 2, META  ); break;
  case '.': cutByLen ( rexp, track, 1, POINT ); break;
  case '@': cutByLen ( rexp, track, 1 +
                      countCharDigits( rexp->ptr + 1 ),
                                     BACKREF ); break;
  case '(': cutPair  ( rexp, track,  GROUP   ); break;
  case '<': cutPair  ( rexp, track,  HOOK    ); break;
  case '[': cutPair  ( rexp, track,  SET     ); break;
  default : cutSimple( rexp, track           ); break;
  }

  getLoops( rexp, track );
  getMods ( rexp, track );
  return TRUE;
}

static void cutSimple( struct RE *rexp, struct RE *track ){
  for( int i = 1; i < rexp->len; i++ )
    switch( rexp->ptr[ i ] ){
    case '(': case '<': case '[': case '@': case ':': case '.':
      cutByLen( rexp, track, i, SIMPLE  ); return;
    case '?': case '+': case '*': case '{': case '#':
      if( i == 1 ) cutByLen( rexp, track,     1, SIMPLE  );
      else         cutByLen( rexp, track, i - 1, SIMPLE  );
      return;
    }

  cutByLen( rexp, track, rexp->len, SIMPLE  );
}

static void cutByLen( struct RE *rexp, struct RE *track, const int len, const int type ){
  *track       = *rexp;
  track->type  = type;
  track->len   = len;
  cutRexp( rexp, len );
}

static int cutPath( struct RE *rexp, struct RE *track ){
  if( rexp->len == 0 ) return FALSE;

  *track      = *rexp;
  track->type = PATH;

  for( int i = 0; (i += walkMeta( rexp->ptr + i, rexp->len - i )) < rexp->len; i++ )
    switch( rexp->ptr[i] ){
    case '<': i += walkPair( rexp->ptr + i, rexp->len - i, "<>" ); break;
    case '(': i += walkPair( rexp->ptr + i, rexp->len - i, "()" ); break;
    case '[': i += walkPair( rexp->ptr + i, rexp->len - i, "[]" ); break;
    case '|':
      track->len = i;
      cutRexp( rexp, i + 1 );
      return TRUE;
    }

  cutRexp( rexp, rexp->len );
  return TRUE;
}

static void cutPair( struct RE *rexp, struct RE *track, const int type ){
  *track       = *rexp;
  track->type  = type;

  switch( type ){
  case HOOK : track->len = walkPair( rexp->ptr, rexp->len, "<>" ); break;
  case GROUP: track->len = walkPair( rexp->ptr, rexp->len, "()" ); break;
  case SET  : track->len = walkPair( rexp->ptr, rexp->len, "[]" ); break;
  }

  cutRexp( track, 1 );
  cutRexp( rexp, track->len + 2 );
}

static void cutRexp( struct RE *rexp, const int len ){
  rexp->ptr += len; rexp->len -= len;
}

static int walkPair( const char *rexp, const int len, const char p[2] ){
  for( int i = 0, deep = 0; (i += walkMeta( rexp + i, len + i )) < len; i++ ){
    if( rexp[i] == p[0] ) deep++;
    if( rexp[i] == p[1] ) deep--;
    if( deep == 0 ) return i;
  }

  return len;
}

static int walkMeta( const char *str, const int len ){
  for( int i = 0; i < len; i += 2 )
    if( str[i] != ':' ) return i;

  return len;
}

static void getMods( struct RE *rexp, struct RE *track ){
  int inMods = *rexp->ptr == '#', pos = 0;
  track->mods &= ~MOD_NEGATIVE;

  while( inMods )
    switch( rexp->ptr[ ++pos ] ){
    case '^': track->mods |=  MOD_ALPHA     ; break;
    case '$': track->mods |=  MOD_OMEGA     ; break;
    case '?': track->mods |=  MOD_LONLEY    ; break;
    case '~': track->mods |=  MOD_FwrByChar ; break;
    case '*': track->mods |=  MOD_COMMUNISM ; break;
    case '/': track->mods &= ~MOD_COMMUNISM ; break;
    case '!': track->mods |=  MOD_NEGATIVE  ; break;
    default : inMods       =  FALSE         ; break;
    }

  cutRexp( rexp, pos );
}

static void getLoops( struct RE *rexp, struct RE *track ){
  track->loopsMin = 1; track->loopsMax = 1;

  if( rexp->len )
    switch( *rexp->ptr ){
    case '?' : cutRexp( rexp, 1 ); track->loopsMin = 0; track->loopsMax =   1; return;
    case '+' : cutRexp( rexp, 1 ); track->loopsMin = 1; track->loopsMax = INF; return;
    case '*' : cutRexp( rexp, 1 ); track->loopsMin = 0; track->loopsMax = INF; return;
    case '{' : cutRexp( rexp, 1 );
      track->loopsMin = aToi( rexp->ptr );
      cutRexp( rexp, countCharDigits( rexp->ptr ) );
      if( *rexp->ptr == ',' ){
        cutRexp( rexp, 1 );
        if( *rexp->ptr == '}' )
          track->loopsMax = INF;
        else {
          track->loopsMax = aToi( rexp->ptr );
          cutRexp( rexp, countCharDigits( rexp->ptr  ) );
        }
      } else track->loopsMax = track->loopsMin;

      cutRexp( rexp, 1 );
    }
}

static int match( struct RE *rexp ){
  switch( rexp->type ){
  case POINT  : return text.pos < text.len;
  case SET    : return matchSet    ( *rexp );
  case BACKREF: return matchBackRef(  rexp );
  case RANGEAB: return matchRange( rexp, text.ptr[text.pos]  );
  case META   : return matchMeta ( rexp, text.ptr[text.pos]  );
  default     : return matchText ( rexp, text.ptr + text.pos );
  }
}

static int matchText( struct RE *rexp, const char *txt ){
  if( rexp->mods & MOD_COMMUNISM )
    return    strnEqlCommunist( txt, rexp->ptr, rexp->len ) ? rexp->len : 0;
  else return strnEql         ( txt, rexp->ptr, rexp->len ) ? rexp->len : 0;
}

static int matchRange( struct RE *rexp, int chr ){
  if( rexp->mods & MOD_COMMUNISM ){
    chr = toLower( chr );
    return chr >= toLower( rexp->ptr[ 0 ] ) && chr <= toLower( rexp->ptr[ 2 ] );
  } else
    return chr >=          rexp->ptr[ 0 ]   && chr <=          rexp->ptr[ 2 ];
}


static int matchMeta( struct RE *rexp, const int chr ){
  switch( rexp->ptr[1] ){
  case 'a' : return  isAlpha( chr );
  case 'A' : return !isAlpha( chr );
  case 'd' : return  isDigit( chr );
  case 'D' : return !isDigit( chr );
  case 'w' : return  isAlnum( chr );
  case 'W' : return !isAlnum( chr );
  case 's' : return  isSpace( chr );
  case 'S' : return !isSpace( chr );
  default  : return rexp->ptr[1] == chr;
  }
}

static int matchSet( struct RE rexp ){
  struct RE track;
  for( int result = 0; trackerSet( &rexp, &track ); ){
    switch( track.type ){
    case RANGEAB: case META:
      result = match( &track ); break;
    default:
      if( track.mods & MOD_COMMUNISM )
        result = strnChrCommunist( track.ptr, text.ptr[ text.pos ], track.len  ) != 0;
      else result = strnChr      ( track.ptr, text.ptr[ text.pos ], track.len  ) != 0;
    }

    if( result ) return result;
  }

  return FALSE;
}

static int trackerSet( struct RE *rexp, struct RE *track ){
  if( rexp->len == 0 ) return FALSE;

  if( *rexp->ptr == ':' ) cutByLen ( rexp, track, 2, META  );
  else {
    for( int i = 1; i < rexp->len; i++ )
      switch( rexp->ptr[ i ] ){
      case ':':      cutByLen( rexp, track,     i, SIMPLE  ); goto setLM;
      case '-':
        if( i == 1 ) cutByLen( rexp, track,     3, RANGEAB );
        else         cutByLen( rexp, track, i - 1, SIMPLE  );
        goto setLM;
      }

    cutByLen( rexp, track, rexp->len, SIMPLE  );
  }

 setLM:
  track->loopsMin = track->loopsMax = 1;
  track->mods    &= ~MOD_NEGATIVE;
  return TRUE;
}

static int matchBackRef( struct RE *rexp ){
  const int backRefId    = aToi( rexp->ptr + 1 );
  const int backRefIndex = lastIdCatch( backRefId );
  if( gpsCatch( backRefIndex ) == 0 ||
      strnEql( text.ptr + text.pos, gpsCatch( backRefIndex ), lenCatch( backRefIndex ) ) == FALSE )
    return FALSE;
  else return lenCatch( backRefIndex );
}

static int lastIdCatch( const int id ){
  for( int index = Catch.index - 1; index > 0; index-- )
    if( Catch.id[ index ] == id ) return index;

  return MAX_CATCHS;
}

static void openCatch( int *index ){
  if( Catch.index < MAX_CATCHS ){
    *index = Catch.index++;
    Catch.ptr[ *index ] = text.ptr + text.pos;
    Catch.id [ *index ] = Catch.idx++;
  } else *index = MAX_CATCHS;
}

static void closeCatch( const int index ){
  if( index < MAX_CATCHS )
    Catch.len[ index ] = &text.ptr[ text.pos ] - Catch.ptr[ index ];
}

int totCatch(){ return Catch.index - 1; }

const char * gpsCatch( const int index ){
  return ( index > 0 && index < Catch.index ) ? Catch.ptr[ index ] : 0;
}

int lenCatch( const int index ){
  return ( index > 0 && index < Catch.index ) ? Catch.len[ index ] : 0;
}

char * cpyCatch( char * str, const int index ){
  if( index > 0 && index < Catch.index )
    strnCpy( str, Catch.ptr[ index ], Catch.len[ index ] );
  else *str = '\0';

  return str;
}

char * rplCatch( char * newStr, const char * rplStr, const int id ){
  char *oNewStr = newStr;
  const char *last = Catch.ptr[ 0 ];

  for( int index = 1, rpLen = strLen( rplStr ); index < Catch.index; index++ )
    if( id == Catch.id[ index ] ){
      if( last > Catch.ptr[index] ) last = Catch.ptr[index];

      strnCpy( newStr, last, Catch.ptr[index] - last );
      newStr += Catch.ptr[index] - last;
      strCpy( newStr, rplStr );
      newStr += rpLen;
      last    = Catch.ptr[index] + Catch.len[index];
    }

  strnCpy( newStr, last, Catch.ptr[0] + Catch.len[0] - last );
  return oNewStr;
}

char * putCatch( char * newStr, const char * putStr ){
  char *oNewStr = newStr;

  while( *putStr )
    switch( *putStr ){
    case '#':
      if( *++putStr == '#' )
        *newStr++ = *putStr++;
      else {
        int index = aToi( putStr );
        cpyCatch( newStr, index );
        newStr += lenCatch( index );
        putStr += countCharDigits( putStr );
      } break;
    default : *newStr++ = *putStr++;
    }

  *newStr = '\0';

  return oNewStr;
}
