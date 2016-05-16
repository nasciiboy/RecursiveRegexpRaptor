/*************************************************************************
 * <Recursive Regexp Raptor (regexp3) regexp library.>
 * https://github.com/nasciiboy/RecursiveRegexpRaptor
 * Copyright (C) <2016>  <nasciiboy>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

int    regexp3 ( char *line      , char *exp );
char * cpyCatch( char * lineCatch, int index );
int    totCatch(                             );
char * gpsCatch(                   int index );
int    lenCatch(                   int index );
char * replaceCatch( char * newLine, char * str, int index );
char * newLineCatch( char * newLine, char * str );
