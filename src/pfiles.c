 /*********************************************************************************** 
 *                                                                                  *
 *          _______.____    __    ____       _______                 _______        *
 *         /       )\   \  /  \  /   /  _   |   ____)         __    |   ____)       *
 *        (   (----` \   \/    \/   /  (_)  |  |__    ___   _/  |_  |  |__          *
 *         \   \      \            /    _   |   __)  / _ \ (_   __) |   __)         *
 *     .----)   )      \    /\    /    (_)  |  |    ( (_) )  |  |   |  |____        *
 *    (________/        \__/  \__/          |__|     \___/   |__|   |_______)       *
 *                                                                                  *
 * SWFotE v2.0 (FotE v1.1 cleaned up and considerably modded) by                    *
 * Greg (Keberus) Mosley                                                            *
 * Roman (Trelar) Arnold                                                            *
 *                                                                                  *
 * SWFotE v1 & v1.1 copyright (c) 2002 was created by                               *
 * Chris 'Tawnos' Dary (cadary@uwm.edu),                                            *
 * Korey 'Eleven' King (no email),                                                  *
 * Matt 'Trillen' White (mwhite17@ureach.com),                                      *
 * Daniel 'Danimal' Berrill (danimal924@yahoo.com),                                 *
 * Richard 'Bambua' Berrill (email unknown),                                        *
 * Stuart 'Ackbar' Unknown (email unknown)                                          *
 *                                                                                  *
 * SWR 1.0 copyright (c) 1997, 1998 was created by Sean Cooper                      *
 * based on a concept and ideas from the original SWR immortals:                    *
 * Himself (Durga), Mark Matt (Merth), Jp Coldarone (Exar), Greg Baily (Thrawn),    *
 * Ackbar, Satin, Streen and Bib as well as much input from our other builders      *
 * and players.                                                                     *
 *                                                                                  *
 * Original SMAUG 1.4a written by Thoric (Derek Snider) with Altrag,                *
 * Blodkai, Haus, Narn, Scryn, Swordbearer, Tricops, Gorog, Rennard,                *
 * Grishnakh, Fireblade, and Nivek.                                                 *
 *                                                                                  *
 * Original MERC 2.1 code by Hatchet, Furey, and Kahn.                              *
 *                                                                                  *
 * Original DikuMUD code by: Hans Staerfeldt, Katja Nyboe, Tom Madsen,              *
 * Michael Seifert, and Sebastian Hammer.                                           *
 *                                                                                  *
 ***********************************************************************************/

/****************************************************************************
 *                   ^     +----- |  / ^     ^ |     | +-\                  *
 *                  / \    |      | /  |\   /| |     | |  \                 *
 *                 /   \   +---   |<   | \ / | |     | |  |                 *
 *                /-----\  |      | \  |  v  | |     | |  /                 *
 *               /       \ |      |  \ |     | +-----+ +-/                  *
 ****************************************************************************
 * AFKMud Copyright 1997-2002 Alsherok. Contributors: Samson, Dwip, Whir,   *
 * Cyberfox, Karangi, Rathian, Cam, Raine, and Tarl.                        *
 *                                                                          *
 * Original SMAUG 1.4a written by Thoric (Derek Snider) with Altrag,        *
 * Blodkai, Haus, Narn, Scryn, Swordbearer, Tricops, Gorog, Rennard,        *
 * Grishnakh, Fireblade, and Nivek.                                         *
 *                                                                          *
 * Original MERC 2.1 code by Hatchet, Furey, and Kahn.                      *
 *                                                                          *
 * Original DikuMUD code by: Hans Staerfeldt, Katja Nyboe, Tom Madsen,      *
 * Michael Seifert, and Sebastian Hammer.                                   *
 ****************************************************************************
 *                          Pfile Pruning Module                            *
 ****************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include "mud.h"

/* Globals */
time_t pfile_time;
HOUR_MIN_SEC set_pfile_time_struct;
HOUR_MIN_SEC *set_pfile_time;
struct tm *new_pfile_time;
struct tm new_pfile_struct;
time_t new_pfile_time_t;
short num_pfiles; /* Count up number of pfiles */
sh_int jcount, scount;

void save_timedata( void )
{
   FILE *fp;
   char filename[MIL];

   sprintf( filename, "%stime.dat", SYSTEM_DIR );

   FCLOSE( fpReserve );
   if( ( fp = fopen( filename, "w" ) ) == NULL )
   {
      bug( "save_timedata: fopen" );
      perror( filename );
   }
   else
   {
      fprintf( fp, "%s", "#TIME\n" );
      fprintf( fp, "Purgetime %ld\n", new_pfile_time_t );
      fprintf( fp, "%s", "End\n\n" );
      fprintf( fp, "%s", "#END\n" );
   }
   FCLOSE( fp );
   fpReserve = fopen( NULL_FILE, "r" );
   return;
}






#ifdef KEY 
#undef KEY 
#endif 
#define KEY( literal, field, value )                              \
                                if ( !str_cmp( word, literal ) )  \
                                {                                 \
                                    field = value;                \
                                    fMatch = TRUE;                \
                                    break;                        \
                                }

/* Reads the actual time file from disk - Samson 1-21-99 */
void fread_timedata( FILE * fp )
{
   char *word = NULL;
   bool fMatch = FALSE;

   for( ;; )
   {
      word = feof( fp ) ? "End" : fread_word( fp );
      fMatch = FALSE;

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;

         case 'E':
            if( !str_cmp( word, "End" ) )
               return;
            break;

         case 'P':
            KEY( "Purgetime", new_pfile_time_t, fread_number( fp ) );
            break;
      }

      if( !fMatch )
      {
         bug( "Fread_timedata: no match: %s", word );
         fread_to_eol( fp );
      }
   }
}

bool load_timedata( void )
{
   char filename[MIL];
   FILE *fp;
   bool found;

   found = FALSE;
   sprintf( filename, "%stime.dat", SYSTEM_DIR );

   if( ( fp = fopen( filename, "r" ) ) != NULL )
   {

      found = TRUE;
      for( ;; )
      {
         char letter = '\0';
         char *word = NULL;

         letter = fread_letter( fp );
         if( letter == '*' )
         {
            fread_to_eol( fp );
            continue;
         }

         if( letter != '#' )
         {
            bug( "%s", "Load_timedata: # not found." );
            break;
         }

         word = fread_word( fp );
         if( !str_cmp( word, "TIME" ) )
         {
            fread_timedata( fp );
            break;
         }
         else if( !str_cmp( word, "END" ) )
            break;
         else
         {
            bug( "Load_timedata: bad section - %s.", word );
            break;
         }
      }
      FCLOSE( fp );
   }

   return found;
}

void init_pfile_scan_time( void )
{
   /*
    * Init pfile scan time.
    */
   set_pfile_time = &set_pfile_time_struct;

   new_pfile_time = update_time( localtime( &current_time ) );
   /*
    * Copies *new_pfile_time to new_pfile_struct, and then points
    * new_pfile_time to new_pfile_struct again. -- Alty 
    */
   new_pfile_struct = *new_pfile_time;
   new_pfile_time = &new_pfile_struct;
   new_pfile_time->tm_mday += 1;
   if( new_pfile_time->tm_hour > 12 )
      new_pfile_time->tm_mday += 1;
   new_pfile_time->tm_sec = 0;
   new_pfile_time->tm_min = 0;
   new_pfile_time->tm_hour = 3;

   /*
    * Update new_pfile_time (due to day increment) 
    */
   new_pfile_time = update_time( new_pfile_time );
   new_pfile_struct = *new_pfile_time;
   new_pfile_time = &new_pfile_struct;
   /*
    * Bug fix submitted by Gabe Yoder 
    */
   new_pfile_time_t = mktime( new_pfile_time );
   /*
    * check_pfiles(mktime(new_pfile_time)); 
    */

   if( !load_timedata(  ) )
      log_string( "Pfile scan time reset to default time of 3am." );
   return;
}

time_t now_time;
short deleted = 0;
short days = 0;

void fread_pfile( FILE * fp, time_t tdiff, char *fname, bool count )
{
   char *word;
   char *name = NULL;
   char *clan = NULL;
   short level = 0;
   short file_ver = 0;
   int pact2;
   bool fMatch;
   ROOM_INDEX_DATA *plr_home;
   char homebuf[MSL];

   for( ;; )
   {
      word = feof( fp ) ? "End" : fread_word( fp );
      fMatch = FALSE;

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;

         case 'A':
            KEY( "Act2", pact2, fread_number( fp ) );
            break;

         case 'C':
            KEY( "Clan", clan, fread_string( fp ) );
            break;

         case 'E':
            if( !strcmp( word, "End" ) )
               goto timecheck;
            break;

         case 'T':
            KEY( "Toplevel", level, fread_number( fp ) );
            break;

         case 'N':
            KEY( "Name", name, fread_string( fp ) );
            break;

		 case 'P':
            if ( !str_cmp( word, "PlrHome" ) )
            {
                plr_home = get_room_index( fread_number( fp ) );
                break;
            }
			break;

         case 'V':
            KEY( "Version", file_ver, fread_number( fp ) );
            break;
      }

      if( !fMatch )
         fread_to_eol( fp );
   }

 timecheck:

   if( count == FALSE && !IS_SET( pact2, ACT_EXEMPT ) )
   {
      if( (level < 10) && (tdiff > sysdata.newbie_purge) )
      {
         if( unlink( fname ) == -1 )
            perror( "Unlink" );
         else
         {
           sprintf( homebuf, "%s%c/%s.home", PLAYER_DIR, tolower(name[0]), name);

			if ( !remove(homebuf) )
			{
			   STRFREE( plr_home->name );
			   plr_home->name = STRALLOC( "An Empty Apartment" );
			   REMOVE_BIT( plr_home->room_flags , ROOM_PLR_HOME );
			   SET_BIT( plr_home->room_flags , ROOM_EMPTY_HOME );
			   fold_area( plr_home->area, plr_home->area->filename, FALSE );
			}

			 days = sysdata.newbie_purge;
            sprintf( log_buf, "Player %s was deleted. Exceeded time limit of %d days.", name, days );
            log_string( log_buf );
#ifdef AUTO_AUTH
            remove_from_auth( name );
#endif
            deleted++;
         }
      }

     if( (level < LEVEL_IMMORTAL) && (tdiff > sysdata.regular_purge) )
      {
         if( level < LEVEL_IMMORTAL )
         {
            if( unlink( fname ) == -1 )
               perror( "Unlink" );
            else
            {
              sprintf( homebuf, "%s%c/%s.home", PLAYER_DIR, tolower(name[0]), name);

			  if ( !remove(homebuf) )
			  {
			    STRFREE( plr_home->name );
			    plr_home->name = STRALLOC( "An Empty Apartment" );
			    REMOVE_BIT( plr_home->room_flags , ROOM_PLR_HOME );
			    SET_BIT( plr_home->room_flags , ROOM_EMPTY_HOME );
			    fold_area( plr_home->area, plr_home->area->filename, FALSE );
			  }
               days = sysdata.regular_purge;
               sprintf( log_buf, "Player %s was deleted. Exceeded time limit of %d days.", name, days );
               log_string( log_buf );
#ifdef AUTO_AUTH
               remove_from_auth( name );
#endif
               deleted++;
            }
         }
      }
   }

   if( clan != NULL )
   {
      CLAN_DATA *guild = get_clan( clan );

      if( guild )
         guild->members++;
   }

   if( name )
     STRFREE(name);
   if( clan )
     STRFREE(clan);

   return;
}

void read_pfile( char *dirname, char *filename, bool count )
{
   FILE *fp;
   char fname[MSL];
   struct stat fst;
   time_t tdiff;

   now_time = time( 0 );

   sprintf( fname, "%s/%s", dirname, filename );

   if( stat( fname, &fst ) != -1 )
   {
      tdiff = ( now_time - fst.st_mtime ) / 86400;

      if( ( fp = fopen( fname, "r" ) ) != NULL )
      {
         for( ;; )
         {
            char letter;
            char *word;

            letter = fread_letter( fp );

            if( ( letter != '#' ) && ( !feof( fp ) ) )
               continue;

            word = feof( fp ) ? "End" : fread_word( fp );

            if( !str_cmp( word, "End" ) )
               break;

            if( !str_cmp( word, "PLAYER" ) )
               fread_pfile( fp, tdiff, fname, count );
            else if( !str_cmp( word, "END" ) )  /* Done     */
               break;
         }
         FCLOSE( fp );
      }
   }
   return;
}

void pfile_scan( bool count )
{
   DIR *dp;
   struct dirent *dentry;
   CLAN_DATA *clan;
   char directory_name[100];

   short alpha_loop;
   short cou = 0;
   deleted = 0;

   now_time = time( 0 );
   nice( 20 );

   /*
    * Reset all clans to 0 members prior to scan - Samson 7-26-00 
    */
   if( !count )
      for( clan = first_clan; clan; clan = clan->next )
         clan->members = 0;

   for( alpha_loop = 0; alpha_loop <= 25; alpha_loop++ )
   {
      sprintf( directory_name, "%s%c", PLAYER_DIR, 'a' + alpha_loop );
      /*
       * log_string( directory_name ); 
       */
      dp = opendir( directory_name );
      dentry = readdir( dp );
      while( dentry )
      {
         /*
          * Added by Tarl 3 Dec 02 because we are now using CVS 
          */
         if( !str_cmp( dentry->d_name, "CVS" ) )
         {
            dentry = readdir( dp );
            continue;
         }
        if ( dentry->d_name[0] != '.' && str_suffix(".home", dentry->d_name) && str_suffix(".clone", dentry->d_name) )
         {
            if( !count )
               read_pfile( directory_name, dentry->d_name, count );
            cou++;
         }
         dentry = readdir( dp );
      }
      closedir( dp );
   }

   if( !count )
      log_string( "Pfile cleanup completed." );
   else
      log_string( "Pfile count completed." );

   sprintf( log_buf, "Total pfiles scanned: %d", cou );
   log_string( log_buf );

   if( !count )
   {
      sprintf( log_buf, "Total pfiles deleted: %d", deleted );
      log_string( log_buf );

      sprintf( log_buf, "Total pfiles remaining: %d", cou - deleted );
      num_pfiles = cou - deleted;
      log_string( log_buf );

      for( clan = first_clan; clan; clan = clan->next )
         save_clan( clan );
   }
   else
      num_pfiles = cou;

   return;
}

void fread_fpfile( CHAR_DATA *ch, FILE *fp, char *fname )
{
   char *word;
   char *name = NULL;
   bool fMatch;
   sh_int ftype, fstatus;
   char buftype[MSL], bufstatus[MSL], buffinal[MSL];
   for ( ; ; )
   {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	  case '*':
	      fMatch = TRUE;
	      fread_to_eol( fp );
	      break;

	 case 'F':
	      KEY( "ForceType", 		ftype, 	    fread_number( fp ) );
	      KEY( "ForceLvlStatus", 		fstatus, 	fread_number( fp ) );
	      break;

	   case 'N':
	      KEY( "Name", 		name, 	fread_string( fp ) );
	      break;

         case 'E':
              if ( !strcmp( word, "End" ) )
                goto forcecheck;
              break;

	}

	if ( !fMatch )
	   fread_to_eol( fp );
   }

forcecheck:

   if( ftype == 1 || ftype == 2 )
   {
	   if( ftype == 1 )
	   {
		   strcpy(buftype, "Jedi");
	   }
	   if( ftype == 2 )
	   {
			strcpy(buftype, "Sith");
	   }
	   if( fstatus == 1)
	   {
		   	strcpy(bufstatus, "Apprentice");
	   }
	   if( fstatus == 3)
	   {
		   	strcpy(bufstatus, "Master");
	   }
	   if( (ftype == 1) && (fstatus == 2) )
	   {
		   	strcpy(bufstatus, "Knight");
	   }
           if( (ftype == 2) && (fstatus == 2) )
	   {
		   	strcpy(bufstatus, "Lord");
	   }

	   if (ftype == 1)
	   {
	     sprintf(buffinal, "&w&W&W&B%s is a %s %s\n\r", name, buftype, bufstatus);
             send_to_char( buffinal, ch);
	     jcount++;
	   }
	   if (ftype == 2)
           {
             sprintf(buffinal, "&w&W&W&R%s is a %s %s\n\r", name, buftype, bufstatus);
	     send_to_char( buffinal, ch);
	     scount++;
	   }
   
   }
   return;
}

void read_fpfile( CHAR_DATA * ch, char *dirname, char *filename )
{
  FILE *fp;
  char fname[MSL];
  struct stat fst;

  sprintf( fname, "%s/%s", dirname, filename );

  if( !str_suffix(".home1", filename) || !str_suffix(".home2", filename) || !str_suffix(".home3", filename) || !str_suffix(".clone", filename) )
    return;


  if ( stat( fname, &fst ) != -1 )
  {

    if ( ( fp = fopen ( fname, "r" ) ) != NULL )
    {
      for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    
	    if ( ( letter != '#' ) && ( !feof( fp ) ) )
             continue;

	    word = feof( fp ) ? "End" : fread_word( fp );

            if ( !str_cmp( word, "End" ) )
		break;

	    if ( !str_cmp( word, "PLAYER" ) )
	    	fread_fpfile( ch, fp, fname );
	    else
	    if ( !str_cmp( word, "END" ) )	/* Done		*/
		break;
	}
	FCLOSE( fp );
    }
  }
  return;
}

void scan_forcers( CHAR_DATA *ch )
{
  DIR *dp;
  struct dirent *dentry;
  char directory_name[100];

  sh_int alpha_loop;

  ch_printf( ch, "&w&W&W&CForcers&w&W&W\n\r");
  ch_printf( ch, "&B-----------------------&w&W&W\n\r" );
  for ( alpha_loop = 0; alpha_loop <= 25; alpha_loop++ )
  {
      sprintf( directory_name, "%s%c", PLAYER_DIR, 'a' + alpha_loop );
      dp = opendir( directory_name );
      dentry = readdir( dp );
      while ( dentry )
      {
         if ( dentry->d_name[0] != '.' && str_suffix(".home1", dentry->d_name) && str_suffix(".home2", dentry->d_name) && str_suffix(".home3", dentry->d_name) && str_suffix(".clone", dentry->d_name) )
         {
	       read_fpfile( ch, directory_name, dentry->d_name);
          }
         dentry = readdir( dp );
      }
      closedir( dp );
  }
  ch_printf( ch, "&B-----------------------&w&W&W\n\r" );
  return;
}

void show_pfiles (CHAR_DATA *ch, char *x)
{
 DIR *dp;
 struct dirent *dentry;
 char directory_name[100];
 sh_int count = 0;

 
ch_printf( ch, "&w&W&W&CPfiles Starting With %s\n\r", capitalize(x) );
ch_printf( ch, "&B-----------------------\n\r" );
sprintf( directory_name, "%s%s", PLAYER_DIR, x );
 dp = opendir( directory_name );
 dentry = readdir( dp );
      while ( dentry )
      {
         if ( dentry->d_name[0] != '.' && str_suffix(".home", dentry->d_name) && str_suffix(".clone", dentry->d_name) ) 
         {
           ch_printf(ch, "&O%s&w&W&W\n\r\n\r", dentry->d_name); 
           count++;
          }
         dentry = readdir( dp );
      }
      closedir( dp );

  if ( count == 0 )
    {
     send_to_char("&RThere are no pfiles beginning with that letter!&w&W&W\n\r", ch);
    }
  else
    {
	ch_printf(ch, "&WTotal %s Pfiles: &R%d&w&W&W\n\r", capitalize(x), count);
    }
  ch_printf( ch, "&B-----------------------" );
  return;
}

void do_pfiles( CHAR_DATA * ch, char *argument )
{
   char buf[MSL];
   char arg1[MIL];
   char x[3];


   if( IS_NPC( ch ) )
   {
      send_to_char( "Mobs cannot use this command!\n\r", ch );
      return;
   }


   if ( !argument || argument[0] == '\0' )
   {
        send_to_char( "&RSyntax: pfiles <scan/settime/count/letter/force>\n\r", ch);
        return;
   }

   if( !str_cmp( argument, "scan") )
   {
      /*
       * Makes a backup copy of existing pfiles just in case - Samson 
       */
      sprintf( buf, "tar -cf %spfiles.tar %s*", PLAYER_DIR, PLAYER_DIR );

      /*
       * GAH, the shell pipe won't process the command that gets pieced
       * together in the preceeding lines! God only knows why. - Samson 
       */
      system( buf );

      sprintf( log_buf, "Manual pfile cleanup started by %s.", ch->name );
      log_string( log_buf );
      pfile_scan( FALSE );

      return;
   }

   if( !str_cmp( argument, "settime" ) )
   {
      new_pfile_time_t = current_time + 86400;
      save_timedata(  );
      send_to_char( "New cleanup time set for 24 hrs from now.\n\r", ch );
      return;
   }

   if( !str_cmp( argument, "count" ) )
   {
      sprintf( log_buf, "Pfile count started by %s.", ch->name );
      log_string( log_buf );
      pfile_scan( TRUE );
      return;
   }

	argument = one_argument( argument, arg1);

   if ( !str_cmp( arg1, "letter" ) )
   {

	if ( (argument[0] == '\0') || !argument )
	{
		send_to_char("&RSyntax: pfiles letter (letter A-Z)\n\r", ch );
                return;
        }

        if( !is_number( argument ) || ( argument[1] != '\0' ) ) 
        { 
           snprintf( x, 2, argument ); 
           x[0] = tolower(x[0]); 
           show_pfiles( ch, x ); 
        } 
        else
        {
          send_to_char("You can only use a letter in this scan!\n\r",ch);
          return;
        }

        return;
     }   

    if ( !str_cmp( arg1, "forcers" ) )
	{
	   scan_forcers( ch );
	   return;
	}

   send_to_char( "Invalid argument.\n\r", ch );
   return;
}

void check_pfiles( time_t reset )
{
   /*
    * This only counts them up on reboot if the cleanup isn't needed - Samson 1-2-00 
    */
   if( reset == 255 && new_pfile_time_t > current_time )
   {
      reset = 0;  /* Call me paranoid, but it might be meaningful later on */
      log_string( "Counting pfiles....." );
      pfile_scan( TRUE );
      return;
   }

   if( new_pfile_time_t <= current_time )
   {
      if( sysdata.CLEANPFILES == TRUE )
      {

         char buf[MSL];

         /*
          * Makes a backup copy of existing pfiles just in case - Samson 
          */
         sprintf( buf, "tar -cf %spfiles.tar %s*", PLAYER_DIR, PLAYER_DIR );

         /*
          * Would use the shell pipe for this, but alas, it requires a ch in order
          * to work, this also gets called during boot_db before the rare item
          * checks for the rent code - Samson 
          */
         system( buf );

         new_pfile_time_t = current_time + 86400;
         save_timedata(  );
         log_string( "Automated pfile cleanup beginning...." );
         pfile_scan( FALSE );
#ifdef SAMSONRENT
         if( reset == 0 )
            rent_update(  );
#endif
      }
      else
      {
         new_pfile_time_t = current_time + 86400;
         save_timedata(  );
         log_string( "Counting pfiles....." );
         pfile_scan( TRUE );
#ifdef SAMSONRENT
         if( reset == 0 )
            rent_update(  );
#endif
      }
   }
   return;
}

void add_member(char *name, char *shortname) {
  char buf[MAX_STRING_LENGTH];
  char fbuf[MAX_STRING_LENGTH];
 if(name[0] == '\0' || !name)
 {
   bug("add_member: No name!\n\r");
   return;
 }
 if(shortname[0] == '\0' || !shortname)
 {
   bug("add_member: No shortname!\n\r");
   return;
 }
 
 sprintf(fbuf, "%s%s.list", CLAN_DIR, shortname);
 sprintf(buf, "%s~", name);
 append_to_file(fbuf, buf); } void remove_member(char *name, char *shortname) {
 FILE *fpList;
 FILE *fpNew;
 char *buf;
 char list[MAX_STRING_LENGTH];
 char temp[MAX_STRING_LENGTH];
 if(name[0] == '\0')
 {
   bug("remove_member: No name!\n\r");
   return;
 }
 if(shortname[0] == '\0' || !shortname)
 {
   bug("remove_member: No shortname!\n\r");
   return;
 }
 
 sprintf(list, "%s%s.list", CLAN_DIR, shortname);
 sprintf(temp,"%s.temp", list);
 if ( ( fpList = fopen(list, "r" ) ) == NULL )
 {
    bug("Unable to open member list");
    return;
 }
 if ( ( fpNew = fopen(temp, "w" ) ) == NULL)
 {
    bug("remove_member: Unable to write temp list");
    return;
 }
 
 for ( ; ; )
 {
     if(feof(fpList))
	break;
     buf = feof( fpList ) ? "End" : fread_string( fpList );
     if(!str_cmp(buf, "End") || buf[0] == '\0')
	break;
     if(str_cmp(name, buf) && strlen(buf) > 2)
	fprintf(fpNew, "%s~\n", buf);
 }
 fclose( fpNew );
 fclose( fpList );
 rename(temp, list);
}

 void do_exempt(CHAR_DATA *ch, char *argument) 
{ 
   CHAR_DATA *victim; 

 if(argument[0] == '\0') 
 { 
   send_to_char("Syntax: exempt <char>\n\r", ch); 
   return; 
 } 

 if((victim = get_char_world(ch, argument)) == NULL) 
 { 
   send_to_char("They must be online to exempt them.\n\r", ch); 
   return; 
 } 

 if(IS_NPC(victim)) 
 { 
   send_to_char("You can not exempt mobs.\n\r", ch); 
   return; 
 } 

 if ( IS_SET(victim->pcdata->act2, ACT_EXEMPT) ) 
  { 
    REMOVE_BIT( victim->pcdata->act2, ACT_EXEMPT); 
    send_to_char("You now have the possibility of being deleted.\n\r", victim); 
    send_to_char("They now have the possiblity of being deleted.\n\r", ch); 
    return; 
  } 
    
  else 
  { 
    SET_BIT(victim->pcdata->act2, ACT_EXEMPT); 
    send_to_char("You have been exempt from deletion.\n\r", victim); 
    send_to_char("They have been exempt from deletion.\n\r", ch); 
    return; 
  } 
 return; 
} 
