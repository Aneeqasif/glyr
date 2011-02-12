#include <stdlib.h>
#include <string.h>

#include "directlyrics.h"

#include "../types.h"
#include "../core.h"
#include "../stringop.h"

// directlyrics only offers google/javascipt powered search
// Unable to use this via CGI without lots of pain..
// So only use dump URL scheme

const char * lyrics_directlyrics_url(glyr_settings_t * settings)
{
    char * url   = NULL;

    char * esc_a = ascii_strdown(settings->artist,-1);
    if(esc_a)
    {
        char * esc_t = ascii_strdown(settings->title, -1);
	if(esc_t)
	{
		char * rep_a = strreplace(esc_a," ","-");
		if(rep_a)
		{
			char * rep_t = strreplace(esc_t," ","-");
			if(rep_t)
			{
    			     url = strdup_printf("http://www.directlyrics.com/%s-%s-lyrics.html",rep_a,rep_t);
			     free(rep_t);
			}
			free(rep_a);
		}
		free(esc_t);
	}
	free(esc_a);
    }
    return url;
}

memCache_t * lyrics_directlyrics_parse(cb_object * capo)
{
    char * f_entry;
    memCache_t * result = NULL;

    if( (f_entry = strstr(capo->cache->data,"<div id=\"lyricsContent\"><p>")) )
    {
	    char * f_end = NULL;
            f_entry += strlen("<div id=\"lyricsContent\"><p>");
	    if( (f_end = strstr(f_entry,"</div>")) )
	    {
		    char * buf = copy_value(f_entry,f_end);
		    if(buf)
		    {
			// replace nonsense brs that glyr would expand to newlines
			char * brtagged = strreplace(buf,"<br>","");
			if(brtagged)
			{
			    result = DL_init();
			    result->data = brtagged;
			    result->size = strlen(brtagged);
			}
			free(buf);
		    }
	    }
    }
    return result;
}
