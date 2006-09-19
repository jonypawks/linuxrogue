/*
 * message.c
 *
 * This source herein may be modified and/or distributed by anybody who
 * so desires, with the following restrictions:
 *    1.)  No portion of this notice shall be removed.
 *    2.)  Credit shall not be taken for the creation of this source.
 *    3.)  This code is not to be traded, sold, or used for personal
 *         gain or profit.
 *
 */

#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "rogue.h"
#include "init.h"
#include "keys.h"
#include "level.h"
#include "machdep.h"
#include "message.h"
#include "object.h"
#include "pack.h"

char msg_line[DCOLS] = "";
short msg_col = 0;
boolean msg_cleared = 1;
char hunger_str[8] = "";

extern boolean cant_int, did_int, interrupted, save_is_interactive;
extern short add_strength;
extern int cur_level;
extern fighter rogue;

void save_screen(void);
void pad(char *, short);

void message(char *msg, boolean intrpt)
{
	if (!save_is_interactive)
	{
		return;
	}
	if (intrpt)
	{
		interrupted = 1;
		md_slurp();
	}
	cant_int = 1;

	if (!msg_cleared)
	{
		mvaddstr(MIN_ROW - 1, msg_col, MORE);
		refresh();
		wait_for_ack();
		check_message();
	}
	(void) strcpy(msg_line, msg);
	mvaddstr(MIN_ROW - 1, 0, msg);
	addch(' ');
	refresh();
	msg_cleared = 0;
	msg_col = strlen(msg);

	cant_int = 0;
	if (did_int)
	{
		did_int = 0;
		onintr(0);
	}
}

void remessage(void)
{
	if (msg_line[0])
	{
		message(msg_line, 0);
	}
}

void check_message(void)
{
	if (msg_cleared)
	{
		return;
	}
	move(MIN_ROW - 1, 0);
	clrtoeol();
	refresh();
	msg_cleared = 1;
}

short get_input_line(char *prompt, char *buf, char *insert, char *if_cancelled,
                     boolean add_blank, boolean do_echo)
{
	short ch;
	short i = 0, n;

	message(prompt, 0);
	n = strlen(prompt);

	if (insert[0])
	{
		mvaddstr(0, n + 1, insert);
		(void) strcpy(buf, insert);
		i = strlen(insert);
		move(0, (n + i + 1));
		refresh();
	}

	while (((ch = rgetchar()) != '\r') && (ch != '\n') && (ch != ROGUE_KEY_CANCEL))
	{
		if ((ch >= ' ') && (ch <= '~') && (i < MAX_TITLE_LENGTH-2))
		{
			if ((ch != ' ') || (i > 0))
			{
				buf[i++] = ch;
				if (do_echo)
				{
					addch(ch);
				}
			}
		}
		if ((ch == '\b') && (i > 0))
		{
			if (do_echo)
			{
				mvaddch(0, i + n, ' ');
				move(MIN_ROW-1, i+n);
			}
			i--;
		}
		refresh();
	}
	check_message();
	if (add_blank)
	{
		buf[i++] = ' ';
	}
	else
	{
		while ((i > 0) && (buf[i-1] == ' '))
		{
			i--;
		}
	}

	buf[i] = 0;

	if ((ch == ROGUE_KEY_CANCEL) || (i == 0) || ((i == 1) && add_blank))
	{
		if (if_cancelled)
		{
			message(if_cancelled, 0);
		}
		return(0);
	}
	return(i);
}

int rgetchar(void)
{
	int ch;

	for(;;)
	{
		ch = getchar();

		switch(ch)
		{
		case ROGUE_KEY_REFRESH:
			wrefresh(curscr);
			break;
		case ROGUE_KEY_SAVE_SCREEN:
			save_screen();
			break;
	#ifdef NUMPAD
		case 27:
			//TODO:  Fix escape key to not have to be hit twice!
			ch=getchar();
			if (ch==91)
			{
				ch=getchar();
				if (ch=='A') return ROGUE_KEY_NORTH;
				else if (ch=='B') return ROGUE_KEY_SOUTH;
				else if (ch=='C') return ROGUE_KEY_EAST;
				else if (ch=='D') return ROGUE_KEY_WEST;
				else if (ch=='G') return ROGUE_KEY_SEARCH;
				else if (ch=='1') { ch=getchar(); return ROGUE_KEY_NORTHWEST; }
				else if (ch=='5') { ch=getchar(); return ROGUE_KEY_NORTHEAST; }
				else if (ch=='4') { ch=getchar(); return ROGUE_KEY_SOUTHWEST; }
				else if (ch=='6') { ch=getchar(); return ROGUE_KEY_SOUTHEAST; }
				else if (ch=='3') { ch=getchar(); return ROGUE_KEY_REST; }
				else if (ch=='2') { ch=getchar(); return ROGUE_KEY_INVENTORY; }
				else return ch;
			} else if (ch=='O') {
				ch=getchar();
				if (ch=='A') return ROGUE_KEY_NORTH_CTRL;
				else if (ch=='B') return ROGUE_KEY_SOUTH_CTRL;
				else if (ch=='C') return ROGUE_KEY_EAST_CTRL;
				else if (ch=='D') return ROGUE_KEY_WEST_CTRL;
				else if (ch=='G') return ROGUE_KEY_SEARCH;
			} else {
				return ch;
			}
			break;
	#endif
		default:
			return(ch);
		}
	}
}
/*
Level: 99 Gold: 999999 Hp: 999(999) Str: 99(99) Arm: 99 Exp: 21/10000000 Hungry
0    5    1    5    2    5    3    5    4    5    5    5    6    5    7    5
*/

void print_stats(int stat_mask)
{
	char buf[16];
	boolean label;
	int row = DROWS - 1;

	label = (stat_mask & STAT_LABEL) ? 1 : 0;

	if (stat_mask & STAT_LEVEL)
	{
		if (label)
		{
			mvaddstr(row, 0, "Level: ");
		}
		/* max level taken care of in make_level() */
		sprintf(buf, "%d", cur_level);
		mvaddstr(row, 7, buf);
		pad(buf, 2);
	}
	if (stat_mask & STAT_GOLD)
	{
		if (label)
		{
			if (rogue.gold > MAX_GOLD)
			{
				rogue.gold = MAX_GOLD;
			}
			mvaddstr(row, 10, "Gold: ");
		}
		sprintf(buf, "%ld", rogue.gold);
		mvaddstr(row, 16, buf);
		pad(buf, 6);
	}
	if (stat_mask & STAT_HP)
	{
		if (label)
		{
			mvaddstr(row, 23, "Hp: ");
			if (rogue.hp_max > MAX_HP)
			{
				rogue.hp_current -= (rogue.hp_max - MAX_HP);
				rogue.hp_max = MAX_HP;
			}
		}
		sprintf(buf, "%d(%d)", rogue.hp_current, rogue.hp_max);
		mvaddstr(row, 27, buf);
		pad(buf, 8);
	}
	if (stat_mask & STAT_STRENGTH)
	{
		if (label)
		{
			mvaddstr(row, 36, "Str: ");
		}
		if (rogue.str_max > MAX_STRENGTH)
		{
			rogue.str_current -= (rogue.str_max - MAX_STRENGTH);
			rogue.str_max = MAX_STRENGTH;
		}
		sprintf(buf, "%d(%d)", (rogue.str_current + add_strength), rogue.str_max);
		mvaddstr(row, 41, buf);
		pad(buf, 6);
	}
	if (stat_mask & STAT_ARMOR)
	{
		if (label)
		{
			mvaddstr(row, 48, "Arm: ");
		}
		if (rogue.armor && (rogue.armor->d_enchant > MAX_ARMOR))
		{
			rogue.armor->d_enchant = MAX_ARMOR;
		}
		sprintf(buf, "%d", get_armor_class(rogue.armor));
		mvaddstr(row, 53, buf);
		pad(buf, 2);
	}
	if (stat_mask & STAT_EXP)
	{
		if (label)
		{
			mvaddstr(row, 56, "Exp: ");
		}
		/*  Max exp taken care of in add_exp() */
		sprintf(buf, "%d/%ld", rogue.exp, rogue.exp_points);
		mvaddstr(row, 61, buf);
		pad(buf, 11);
	}
	if (stat_mask & STAT_HUNGER)
	{
		mvaddstr(row, 73, hunger_str);
		clrtoeol();
	}
	refresh();
}

void pad(char *s, short n)
{
	short i;

	for (i = strlen(s); i < n; i++)
	{
		addch(' ');
	}
}

void save_screen(void)
{
	FILE *fp;
	short i, j;
	char buf[DCOLS+2];
	boolean found_non_blank;


	if ((fp = fopen("rogue.screen", "w")) != NULL)
	{
		for (i = 0; i < DROWS; i++)
		{
			found_non_blank = 0;
			for (j = (DCOLS - 1); j >= 0; j--)
			{
				buf[j] = mvinch(i, j);
				if (!found_non_blank)
				{
					if ((buf[j] != ' ') || (j == 0))
					{
						buf[j + ((j == 0) ? 0 : 1)] = 0;
						found_non_blank = 1;
					}
				}
			}
			fputs(buf, fp);
			putc('\n', fp);
		}
		fclose(fp);
	}
	else
	{
		sound_bell();
	}
}

void sound_bell(void)
{
	putchar(7);
	fflush(stdout);
}

boolean is_digit(short ch)
{
	return((ch >= '0') && (ch <= '9'));
}

int r_index(char *str, int ch, boolean last)
{
	int i = 0;

	if (last)
	{
		for (i = strlen(str) - 1; i >= 0; i--)
		{
			if (str[i] == ch)
			{
				return(i);
			}
		}
	}
	else
	{
		for (i = 0; str[i]; i++)
		{
			if (str[i] == ch)
			{
				return(i);
			}
		}
	}
	return(-1);
}
