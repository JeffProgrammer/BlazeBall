/*
Copyright (C) 2014 insane coder (http://insanecoding.blogspot.com/, http://asprintf.insanecoding.org/)

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _ASPRINTF_H_
#define _ASPRINTF_H_

#ifdef _WIN32

#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define insane_free(ptr) { free(ptr); ptr = 0; }

static int vasprintf(char **strp, const char *fmt, va_list ap)
{
   int r = -1, size = _vscprintf(fmt, ap);

   if ((size >= 0) && (size < INT_MAX))
   {
      *strp = (char *)malloc(size + 1); //+1 for null
      if (*strp)
      {
         r = vsnprintf(*strp, size + 1, fmt, ap);  //+1 for null
         if ((r < 0) || (r > size))
         {
            insane_free(*strp);
            r = -1;
         }
      }
   }
   else { *strp = 0; }

   return(r);
}

static int asprintf(char **strp, const char *fmt, ...)
{
   int r;
   va_list ap;
   va_start(ap, fmt);
   r = vasprintf(strp, fmt, ap);
   va_end(ap);
   return(r);
}

#endif // _WIN32

#endif // _ASPRINTF_H_