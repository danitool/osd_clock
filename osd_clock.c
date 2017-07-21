/*
 * osd_clock
 *
 * Copyright (C) 2001, Jon Beckham <leftorium@leftorium.net>
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
  {"offset",   1, NULL, 'o'},
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

  char *font = "fixed";
  char *color = "red";
  int delay = 3; 
  int offset = 0;
  int shadow = 2;
  int interval = 1;

  while ((c = getopt_long(argc ,argv,"f:c:d:F:i:s:o:tbh",
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
      case 'o':
	offset = atoi(optarg);
	break;
      case 't':
	pos = XOSD_top;
	break;
      case 'b':
	pos = XOSD_bottom;
	break;
      case 'h':
	printf("USAGE: %s [-flag args]\n"
		"\t-f\tfully qualified font.  default: fixed\n"
		"\t-c\tcolor.  default: red\n"
		"\t-s\tdrop shadow offset.  default: 2\n"
		"\t-t\tlocate clock at top left (default: bottom left)\n"
		"\t-b\tlocate clock at bottom left (default)\n"
		"\t-o\toffset value to raise or lower around panels.  def: 0\n"
		"\t-F\tSpecify time/date format (in strftime(3) style)\n"
		"\t-d\tDelay (time the clock stays on screen when it's updated)\n"
	        "\t\tin seconds\n"
		"\t-i\tInterval (time between updates) in seconds\n"
		"\t-h\tthis help message\n",
		argv[0]);
	return EXIT_SUCCESS;
	break;
    }
  }
  
  osd = xosd_init (font, color, delay, pos, offset, shadow, 2);
  if (!osd)
  {
    fprintf (stderr, "Error initializing osd\n");
    return EXIT_FAILURE;
  }

  /* If no format is specified, we revert to ctime-ish display */ 
  if(!format) format = "%a %b %e %H:%M:%S %G";

  while (1)
  {
    time_t curr_time = time(t);
    char output[255];
   
    strftime(output, 255, format, localtime(&curr_time));

    xosd_display (osd, 1, XOSD_string, output);
    sleep(interval);
  }

  xosd_destroy (osd);
  
  return EXIT_SUCCESS;
}
