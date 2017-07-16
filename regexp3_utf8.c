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

enum TYPE { PATH, GROUP, HOOK, SET, BACKREF, META, RANGEAB, UTF8, POINT, SIMPLE };

struct RE {
  const    char *ptr;
  int            len;
  enum     TYPE  type;
  unsigned char  mods;
  unsigned int   loopsMin, loopsMax;
};

static const unsigned char xooooooo = 0x80; // instead `isUTF8( c )` use `c & xooooooo`

#define	t1 0x00   // 0000 0000
#define	tx 0x80   // 1000 0000
#define	t2 0xC0   // 1100 0000
#define	t3 0xE0   // 1110 0000
#define	t4 0xF0   // 1111 0000
#define	t5 0xF8   // 1111 1000

#define	locb 0x80 // 1000 0000
#define	hicb 0xBF // 1011 1111

#define	xx 0xF1   // invalid: size 1
#define	as 0xF0   // ASCII: size 1
#define	s1 0x02   // accept 0, size 2
#define	s2 0x13   // accept 1, size 3
#define	s3 0x03   // accept 0, size 3
#define	s4 0x23   // accept 2, size 3
#define	s5 0x34   // accept 3, size 4
#define	s6 0x04   // accept 0, size 4
#define	s7 0x44   // accept 4, size 4

static const unsigned char first[256] = {
  as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, // 0x00-0x0F
  as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, // 0x10-0x1F
  as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, // 0x20-0x2F
  as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, // 0x30-0x3F
  as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, // 0x40-0x4F
  as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, // 0x50-0x5F
  as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, // 0x60-0x6F
  as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, as, // 0x70-0x7F
  xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, // 0x80-0x8F
  xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, // 0x90-0x9F
  xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, // 0xA0-0xAF
  xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, // 0xB0-0xBF
  xx, xx, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, // 0xC0-0xCF
  s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, s1, // 0xD0-0xDF
  s2, s3, s3, s3, s3, s3, s3, s3, s3, s3, s3, s3, s3, s4, s3, s3, // 0xE0-0xEF
  s5, s6, s6, s6, s7, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx  // 0xF0-0xFF
};

struct acceptRange {
  unsigned char lo; // lowest value for second byte.
  unsigned char hi; // highest value for second byte.
} acceptRanges[5] = {
  { locb, hicb },
  { 0xA0, hicb },
  { locb, 0x9F },
  { 0x90, hicb },
  { locb, 0x8F },
};

static int utf8meter( const char *s ){
  int n = 0;
  for( const char *a = s; *a && n < 5; n++, a++ );

  if( n < 1 ){ return 0; }

  unsigned const char S0 = s[0];
  unsigned const char x  = first[S0];

  if( x >= as ){ return 1; }

  unsigned const char sz = x & 7;
  if( n < sz ){ return 1; }

  unsigned const char S1 = s[1];
  const struct acceptRange accept = acceptRanges[x>>4];

  if( S1 < accept.lo || accept.hi < S1 ){ return 1; }

  if( sz == 2 ){ return 2; }

  unsigned const char S2 = s[2];
  if( S2 < locb || hicb < S2 ){ return 1; }

  if( sz == 3 ){ return 3; }

  unsigned const char S3 = s[3];
  if( S3 < locb || hicb < S3 ){ return 1; }

  return 4;
}

static int  walker       ( struct RE  rexp );
static int  trekking     ( struct RE *rexp );
static int  looper       ( struct RE *rexp );
static int  loopGroup    ( struct RE *rexp );
static int  tracker      ( struct RE *rexp, struct RE *track );
static int  trackerSet   ( struct RE *rexp, struct RE *track );

static void cutSimple    ( struct RE *rexp, struct RE *track );
static int  cutByType    ( struct RE *rexp, struct RE *track, const int type );
static void cutByLen     ( struct RE *rexp, struct RE *track, const int len, const int type );
static void cutRexp      ( struct RE *rexp, const int len );

static int  walkMeta     ( const char *str, const int len );
static int  walkSet      ( const char *str, const int len );

static void getMods      ( struct RE *rexp, struct RE *track );
static void getLoops     ( struct RE *rexp, struct RE *track );

static int  match        ( struct RE *rexp );
static int  matchSet     ( struct RE  rexp );
static int  matchBackRef ( struct RE *rexp );
static int  matchRange   ( struct RE *rexp, int chr );
static int  matchMeta    ( struct RE *rexp, const char *txt );
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
    forward    = utf8meter( txt + i );
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
       cutByType( &rexp, &track, PATH );
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
    case BACKREF: case META: case RANGEAB: case UTF8: case POINT: case SIMPLE:
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
      text.pos += utf8meter( text.ptr + text.pos );
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

  switch( *rexp->ptr & xooooooo ? UTF8 : *rexp->ptr ){
  case ':' : cutByLen ( rexp, track, 2, META    ); break;
  case '.' : cutByLen ( rexp, track, 1, POINT   ); break;
  case '@' : cutByLen ( rexp, track, 1 +
                       countCharDigits( rexp->ptr + 1 ),
                                        BACKREF ); break;
  case '(' : cutByType( rexp, track,    GROUP   ); break;
  case '<' : cutByType( rexp, track,    HOOK    ); break;
  case '[' : cutByType( rexp, track,    SET     ); break;
  case UTF8: cutByLen ( rexp, track, utf8meter(rexp->ptr),
                                        UTF8    ); break;
  default  : cutSimple( rexp, track             ); break;
  }

  getLoops( rexp, track );
  getMods ( rexp, track );
  return TRUE;
}

static void cutSimple( struct RE *rexp, struct RE *track ){
  for( int i = 1; i < rexp->len; i++ )
    switch( rexp->ptr[ i ] & xooooooo ? UTF8 : rexp->ptr[ i ] ){
    case '(': case '<': case '[': case '@': case ':': case '.': case UTF8:
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

static int cutByType( struct RE *rexp, struct RE *track, const int type ){
  if( rexp->len == 0 ) return FALSE;

  *track = *rexp;
  track->type = type;
  for( int cut, i = 0, deep = 0; (i += walkMeta( rexp->ptr + i, rexp->len - i )) < rexp->len; i++ ){
    switch( rexp->ptr[ i ] ){
    case '(': case '<': deep++; break;
    case ')': case '>': deep--; break;
    case '[': i += walkSet( rexp->ptr + i, rexp->len - i ); break;
    }

    switch( type ){
    case HOOK    : cut = deep == 0; break;
    case GROUP   : cut = deep == 0; break;
    case SET     : cut = rexp->ptr[i] == ']'; break;
    case PATH    : cut = deep == 0 && rexp->ptr[i] == '|'; break;
    }

    if( cut ){
      track->len  = i;
      cutRexp( rexp, i + 1 );
      if( type != PATH ) cutRexp( track, 1 );
      return TRUE;
    }
  }

  cutRexp( rexp, rexp->len );
  return TRUE;
}

static void cutRexp( struct RE *rexp, const int len ){
  rexp->ptr += len; rexp->len -= len;
}

static int walkSet( const char *str, const int len ){
  for( int i = 0; (i += walkMeta( str + i, len - i )) < len; i++ )
    if( str[i] == ']' ) return i;

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
  case POINT  : return utf8meter( text.ptr + text.pos );
  case SET    : return matchSet    ( *rexp );
  case BACKREF: return matchBackRef(  rexp );
  case RANGEAB: return matchRange( rexp, text.ptr[text.pos]  );
  case META   : return matchMeta ( rexp, text.ptr + text.pos );
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

static int matchMeta( struct RE *rexp, const char *txt ){
  switch( rexp->ptr[1] ){
  case 'a' : return  isAlpha( *txt );
  case 'A' : return !isAlpha( *txt ) ? utf8meter( txt ) : FALSE;
  case 'd' : return  isDigit( *txt );
  case 'D' : return !isDigit( *txt ) ? utf8meter( txt ) : FALSE;
  case 'w' : return  isAlnum( *txt );
  case 'W' : return !isAlnum( *txt ) ? utf8meter( txt ) : FALSE;
  case 's' : return  isSpace( *txt );
  case 'S' : return !isSpace( *txt ) ? utf8meter( txt ) : FALSE;
  case '&' : return *txt & xooooooo  ? utf8meter( txt ) : FALSE;
  default  : return *txt == rexp->ptr[1];
  }
}

static int matchSet( struct RE rexp ){
  struct RE track;
  for( int result = 0; trackerSet( &rexp, &track ); ){
    switch( track.type ){
    case RANGEAB: case META: case UTF8:
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

  switch( *rexp->ptr & xooooooo ? UTF8 : *rexp->ptr ){
  case ':' : cutByLen( rexp, track, 2,                    META ); break;
  case UTF8: cutByLen( rexp, track, utf8meter(rexp->ptr), UTF8 ); break;
  default  :
    for( int i = 1; i < rexp->len; i++ )
      switch( rexp->ptr[ i ] & xooooooo ? UTF8 : rexp->ptr[ i ] ){
      case ':': case UTF8:
        cutByLen( rexp, track, i, SIMPLE  ); goto setLM;
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
