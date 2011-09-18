#include "../../core.h"
#include "../../stringlib.h"

#define AJAX_URL "http://www.metal-archives.com/search/ajax-advanced/searching/songs/" \
		         "?songTitle=${title}&bandName=${artist}"                                \
		         "&_=1313668588182&sEcho=1&iColumns=5&sColumns=&iDisplayStart=0"       \
		         "&iDisplayLength=100&sNames=%%2C%%2C%%2C%%2C"

#define SUBST_URL "http://www.metal-archives.com/release/ajax-view-lyrics/id/%s"
#define BAD_STRING "(lyrics not available)"

/*--------------------------------------------------------*/

static const gchar * lyrics_metallum_url(GlyrQuery * s)
{
    return AJAX_URL;
}

/*--------------------------------------------------------*/

#define ID_START "id=\\\"lyricsLink_"
#define ID_END   "\\\""

/*--------------------------------------------------------*/

static GList * lyrics_metallum_parse(cb_object * capo)
{
    GList * result_items = NULL;
    gchar * id_start = strstr(capo->cache->data,ID_START);
    if(id_start != NULL)
    {
	    id_start += strlen(ID_START);
	    gchar * ID_string = copy_value(id_start,strstr(id_start,ID_END));
	    if(ID_string != NULL)
	    {
		    gchar * content_url = g_strdup_printf(SUBST_URL,ID_string);
		    if(content_url != NULL)
		    {
			    GlyrMemCache * content_cache = download_single(content_url,capo->s,NULL);
			    if(content_cache != NULL && strstr(content_cache->data,BAD_STRING) == NULL)
			    {
				    result_items = g_list_prepend(result_items, content_cache);
			    }
			    g_free(content_url);
		    }
		    g_free(ID_string);
	    }
    }
    return result_items;
}

/*--------------------------------------------------------*/

MetaDataSource lyrics_metallum_src =
{
	.name = "metallum",
	.key  = 'u',
	.parser    = lyrics_metallum_parse,
	.get_url   = lyrics_metallum_url,
	.endmarker = NULL,
	.quality   = 55,
	.speed     = 70,
	.free_url  = false,
	.type      = GLYR_GET_LYRICS
};

