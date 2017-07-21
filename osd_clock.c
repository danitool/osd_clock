/*
 * osd_clock
 *
 * Copyright (C) 2001, Jon Beckham <leftorium@<leftorium@leftorium.net>
 *                     Martijn van de Streek <martijn@foodfight.org>
 *               2017, Daniel González Cabanelas <dgcbueu@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xosd.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

static struct option long_options[] = {
  {"font",     1, NULL, 'f'},
  {"color",    1, NULL, 'c'},
  {"delay",    1, NULL, 'd'},
  {"format",   1, NULL, 'F'},
  {"interval", 1, NULL, 'i'},
  {"shadow",   1, NULL, 's'},
  {"top",      0, NULL, 't'},
  {"bottom",   0, NULL, 'b'},
  {"right",    0, NULL, 'r'},
  {"xoffset",  1, NULL, 'x'},
  {"yoffset",  1, NULL, 'y'},
  {"help",     0, NULL, 'h'},
  {NULL,       0, NULL, 0}
};

int main (int argc, char *argv[])
{
  char c;

  static const char *format;
  time_t *t = NULL;

  xosd *osd;
  xosd_pos pos = XOSD_bottom;
  xosd_align align = XOSD_left;
  
  const char *font = "-*-freemono-*-r-*-*-*-300-*-*-*-*-*-*";
  const char *color = "red";
  int delay = -1; 
  int xoffset = 0;
  int yoffset = 0;
  int shadow = 2;
  int interval = 1;

  while ((c = getopt_long(argc ,argv,"f:c:d:F:i:s:x:y:tbrh",
			  long_options, NULL)) != -1)
  {
    switch(c)
    {
      case 'f':
	font = optarg;
	break;
      case 'F':
	format = optarg;
	break;
      case 'c':
	color = optarg;
	break;
      case 'd':
	delay = atoi(optarg);
	break;
      case 'i':
	interval = atoi(optarg);
       break;
      case 's':
	shadow = atoi(optarg);
	break;
      case 'x':
	xoffset = atoi(optarg);
	break;
      case 'y':
	yoffset = atoi(optarg);
	break;
      case 't':
	pos = XOSD_top;
	break;
      case 'b':
	pos = XOSD_bottom;
	break;
      case 'r':
	align = XOSD_right;
	break;
      case 'h':
	printf("USAGE: %s [-flag args]\n"
		"\t-f\tfully qualified font.  default: fixed\n"
		"\t-c\tcolor.  default: red\n"
		"\t-s\tdrop shadow offset.  default: 2\n"
		"\t-t\tlocate clock at top (default: bottom)\n"
		"\t-b\tlocate clock at bottom (default)\n"
		"\t-r\tlocate clock at right (default: left)\n"
		"\t-x\thorizontal offset (default: 0)\n"
		"\t-y\tvertical offset (default: 0)\n"
		"\t-F\tSpecify time/date format (in strftime(3) style)\n"
		"\t-d\tDelay (time the clock stays on screen when it's updated)\n"
	        "\t\tin seconds (default: -1 (never))\n"
		"\t-i\tInterval (time between updates) in seconds\n"
		"\t-h\tthis help message\n",
		argv[0]);
	return EXIT_SUCCESS;
	break;
    }
  }

  osd = xosd_create(1);

  /* Set the position of the display. */
  xosd_set_pos(osd, pos);
  xosd_set_align(osd, align);
  xosd_set_vertical_offset(osd,yoffset);
  xosd_set_horizontal_offset(osd,xoffset);
 
  /* Set timeout */
  xosd_set_timeout(osd, delay);
  
  /* Set the font and the colours. */
  xosd_set_font(osd, font);
  xosd_set_colour(osd, color);

  /* Set the font shadow */
  xosd_set_shadow_offset(osd, shadow);
  
  if (!osd)
  {
    fprintf (stderr, "Error initializing osd\n");
    return EXIT_FAILURE;
  }

  /* If no format is specified, we revert to ctime-ish display */ 
  if(!format) format = "%H:%M:%S";

  while (1)
  {
    time_t curr_time = time(t);
    char output[255];
   
    strftime(output, 255, format, localtime(&curr_time));

    xosd_display (osd, 0, XOSD_string, output);
    sleep(interval);
  }

  xosd_destroy (osd);
  
  return EXIT_SUCCESS;
}
